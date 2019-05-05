#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "kthread.h"
#include "spinlock.h"
#include "proc.h"



// struct threadTable {
//   struct thread threads [NTHREAD];
// };


struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;


struct {
    struct kthread_mutex mutex[MAX_MUTEXES];
    struct spinlock lock;
}mtable;

static struct proc *initproc;

int nextpid = 1;
int nexttid = 1;
extern void forkret(void);
extern void trapret(void);
static struct thread * allocthread(struct proc* p, uint stack_size);
static void clear_thread(struct  thread* t);
static void wakeup1(void *chan);

void
pinit(void)
{
    initlock(&ptable.lock, "ptable");

}

// Must be called with interrupts disabled
int
cpuid() {
    return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
    int apicid, i;

    if(readeflags()&FL_IF)
        panic("mycpu called with interrupts enabled\n");

    apicid = lapicid();
    // APIC IDs are not guaranteed to be contiguous. Maybe we should have
    // a reverse map, or reserve a register to store &cpus[i].
    for (i = 0; i < ncpu; ++i) {
        if (cpus[i].apicid == apicid)
            return &cpus[i];
    }
    panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct thread*
mythread(void) {
    struct cpu *c;
    struct thread *t;
    pushcli();
    c = mycpu();
    t = c->thread;
    popcli();
    return t;
}


// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
    struct cpu *c;
    struct proc *p;
    pushcli();
    c = mycpu();
    p = c->proc;
    popcli();
    return p;
}


struct  thread  * allocthread(struct proc * p, uint stack_size){
    struct thread * t;
    char *sp;
    acquire(p->ttlock);
    int indx=0;
    //look for an unused thread
    for(t = p->threads; t < &p->threads[NTHREAD]; t++){
        if(t->state!= UNUSED){
            indx++;
            continue;
        }

        else goto found;
    }

    release(p->ttlock);
    return 0;

    found:
    t->state = EMBRYO;
    t->tid = indx;
    t->myproc = p;
    release(p->ttlock);

// try to alloct espce in the stack
    if ((t->kstack = kalloc()) == 0) {
        t->state = UNUSED;
        return 0;
    }


    sp = t->kstack + stack_size;
    // Leave room for trap frame.
    sp -= sizeof *t->tf;
    t->tf = (struct trapframe*)sp;

    // Set up new context to start executing at forkret,
    // which returns to trapret.
    sp -= 4;
    *(uint*)sp = (uint)trapret;

    sp -= sizeof *t->context;
    t->context = (struct context*)sp;
    memset(t->context, 0, sizeof *t->context);
    t->context->eip = (uint)forkret;

    return t;

}
//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
    struct proc *p;
    //struct thread * t;
    //char *sp;

    acquire(&ptable.lock);

    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->state == UNUSED)
            goto found;

    }

    release(&ptable.lock);
    return 0;

    found:
    release(&ptable.lock);
    p->state = EMBRYO;
    for(int index  = 0; index < MAX_MUTEXES; index++){
        p->mid[index] = 0;
    }
    p->ttlock  = &ptable.lock;
    //initlock(&p->ttlock,"threads_lock");
    p->pid = nextpid++;

    //allocate first thread
    if(allocthread(p, KSTACKSIZE) == 0){
        p->state =UNUSED;
        return 0;
    }

    return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{

    struct thread *t;
    struct proc *p;
    extern char _binary_initcode_start[], _binary_initcode_size[];

    p = allocproc();

    initproc = p;
    if((p->pgdir = setupkvm()) == 0)
        panic("userinit: out of memory?");
    inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
    p->sz = PGSIZE;
    t = p->threads;
    memset(t->tf, 0, sizeof(*t->tf));
    t->tf->cs = (SEG_UCODE << 3) | DPL_USER;
    t->tf->ds = (SEG_UDATA << 3) | DPL_USER;
    t->tf->es = t->tf->ds;
    t->tf->ss = t->tf->ds;
    t->tf->eflags = FL_IF;
    t->tf->esp = PGSIZE;
    t->tf->eip = 0;  // beginning of initcode.S

    safestrcpy(p->name, "initcode", sizeof(p->name));
    p->cwd = namei("/");

    acquire(p->ttlock);

    t->state = RUNNABLE;

    release(p->ttlock);

    // this assignment to p->state lets other cores
    // run this process. the acquire forces the above
    // writes to be visible, and the lock is also needed
    // because the assignment might not be atomic.
    acquire(&ptable.lock);

    p->state = RUNNABLE;


    release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
    uint sz;
    struct proc *curproc = myproc();
    acquire(&ptable.lock);
    sz = curproc->sz;

    if(n > 0){
        if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0){
            release(&ptable.lock);
            return -1;
        }


    } else if(n < 0){
        if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0){
            release(&ptable.lock);
            return -1;
        }
    }
    curproc->sz = sz;
    release(&ptable.lock);
    switchuvm(curproc);

    return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
    int i, pid;
    struct proc *np;
    struct proc *curproc = myproc();
    struct thread * nt;
    struct thread * curthread = mythread();
    // Allocate process.
    if((np = allocproc()) == 0){
        return -1;
    }
    nt=np->threads;
    //at creation of a new process thread is allways first
    // Copy process and currthread state from proc.
    if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
        kfree(nt->kstack);
        nt->kstack = 0;
        nt->state = UNUSED;
        np->state = UNUSED;
        return -1;
    }
    np->sz = curproc->sz;


    *nt->tf = *curthread->tf;


    // Clear %eax so that fork returns 0 in the child.
    nt->tf->eax = 0;

    for(i = 0; i < NOFILE; i++)
        if(curproc->ofile[i])
            np->ofile[i] = filedup(curproc->ofile[i]);
    np->cwd = idup(curproc->cwd);

    safestrcpy(np->name, curproc->name, sizeof(curproc->name));

    pid = np->pid;



    acquire(np->ttlock);

    nt->state = RUNNABLE;

    release(np->ttlock);
    acquire(&ptable.lock);

    np->parent = curproc;
    nt->parent = curthread;
    np->state = RUNNABLE;

    release(&ptable.lock);

    return pid;
}
//has to happen under lock
//check if all threads are zombies
int allzombies(struct proc *curproc , struct thread * curthread){
    if(curproc->state== ZOMBIE)
        return 1;
    for(struct thread * t=curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if((curthread == 0 || t != curthread) && (t->state != UNUSED && t->state != ZOMBIE)){
            return 0;
        }
    }
    return 1;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void){
    struct proc *curproc = myproc();
    struct thread * t;
    struct proc *p;
    int fd;
    struct thread * curthread = mythread();


    //make all threads exit at trap
    acquire(&ptable.lock);
    for(t= curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if(t ==curthread)
            continue;
        if( t->state!=UNUSED){
            t->killed = 1;

        }
        if(t->state == RUNNABLE || t->state == SLEEPING)
            t->state = ZOMBIE;
    }
//wait until all threads are zombies
    for(t= curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if(t!=curthread && t->state != ZOMBIE && t->state != UNUSED)
            sleep(t, &ptable.lock);
    }

    release(&ptable.lock);
    if(curproc == initproc)
        panic("init exiting");

    // Close all open files.
    for(fd = 0; fd < NOFILE; fd++){
        if(curproc->ofile[fd]){
            fileclose(curproc->ofile[fd]);
            curproc->ofile[fd] = 0;
        }
    }

    begin_op();
    iput(curproc->cwd);
    end_op();
    curproc->cwd = 0;
    acquire(&ptable.lock);

    // Pass abandoned children to init.
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->parent == curproc){
            //  cprintf("proc %s has kids\n",p->name);
            p->parent = initproc;
            if(p->state == ZOMBIE)
                wakeup1(initproc);
        }

    }

    // Parent might be sleeping in wait().
    wakeup1(curproc->parent);
    curproc->state = ZOMBIE;
    curthread->state = ZOMBIE;

    // proc will be zombie when last child has exited
    // Jump into the scheduler, never to return.
    // current thread won't get back since it will exit at trap
    sched();
    panic("zombie exit");
}




// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void){

    struct proc *p;
    int havekids, pid;
    struct proc *curproc = myproc();
    struct thread *t;

    acquire(&ptable.lock);
    for(;;){
        // Scan through table looking for exited children.
        havekids = 0;
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->parent != curproc)
                continue;
            havekids = 1;
            //cprintf("N%d\n",p->pid);
            if(allzombies(p,0)==1){ // if all proc theards are zombies
                // Found one. clean threads
                // cprintf("z%d\n",p->pid);
                for(t=p->threads; t<&p->threads[NTHREAD]; t++){
                    if(t->state != UNUSED){
                        clear_thread(t);

                    }
                }
                pid = p->pid;
                // reset all mutexes that are owned by the proc
                for(int index  = 0; index < MAX_MUTEXES; index++){
                    p->mid[index] = 0;
                }
                p->pid = 0;
                p->parent = 0;
                p->name[0] = 0;
                p->killed = 0;
                freevm(p->pgdir);
                p->state = UNUSED;
                release(&ptable.lock);
                return pid;
            }
        }

        // No point waiting if we don't have any children.
        if(!havekids || curproc->killed){
            release(&ptable.lock);
            return -1;
        }

        // Wait for children to exit.  (See wakeup1 call in proc_exit.)
        sleep(curproc, &ptable.lock);  //DOC: wait-sleep

    }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
    struct proc *p;
    struct cpu *c = mycpu();
    struct thread * t;
    c->proc = 0;
    c->thread = 0;
    c->thread = 0;

    for(;;){
        // Enable interrupts on this processor.
        sti();

        // Loop over process table looking for process to run.
        acquire(&ptable.lock);
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->state != RUNNABLE)
                continue;
            for(t = p->threads ; t < &p->threads[NTHREAD]; t++){
                if(t->state != RUNNABLE)
                    continue;
                // cprintf("cpu %d chose proc %s and thread %d\n",c->apicid, p->name,t->tid);
                // Switch to chosen process.  It is the process's job
                // to release ptable.lock and then reacquire it
                // before jumping back to us.
                c->proc = p;
                c->thread = t;
                switchuvm(p);
                //  cprintf("cpu %d with proc %s returned from switchuvm\n",c->apicid,p->name);

                p->state = RUNNING;
                t->state = RUNNING;

                swtch(&(c->scheduler), t->context);
                switchkvm();

                c->thread = 0;
            }
            // cprintf("cpu %d  looking for another proc\n",mycpu()->apicid);
            // Process is done running for now.
            // It should have changed its p->state before coming back.
            c->proc = 0;
        }
        release(&ptable.lock);

    }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
    int intena;
    struct proc *p = myproc();
    struct thread *t = mythread();
    if(!holding(&ptable.lock))
        panic("sched ptable.lock");
    if(mycpu()->ncli != 1)
        panic("sched locks");
    if(p->state == RUNNING )
        panic("sched running");
    if(t->state == RUNNING)
        panic("sched thread running");

    if(readeflags()&FL_IF)
        panic("sched interruptible");
    intena = mycpu()->intena;

    swtch(&t->context, mycpu()->scheduler);
    mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
    acquire(&ptable.lock);  //DOC: yieldlock
    myproc()->state = RUNNABLE;
    mythread()->state = RUNNABLE;
    sched();
    release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
    static int first = 1;
    // Still holding ptable.lock from scheduler.
    release(&ptable.lock);

    if (first) {
        // Some initialization functions must be run in the context
        // of a regular process (e.g., they call sleep), and thus cannot
        // be run from main().
        first = 0;
        iinit(ROOTDEV);
        initlog(ROOTDEV);
    }

    // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{

    // cprintf("cpu %d with proc %s is starting sleep\n",mycpu()->apicid,myproc()->name);
    struct thread *curthread = mythread();
    struct proc * p = myproc();
    struct thread * t ;

    if(p == 0)
        panic("sleeping proc");

    if(curthread == 0)
        panic("sleeping thread");


    if(lk == 0)
        panic("sleep without lk");

    // Must acquire ptable.lock in order to
    // change p->state and then call sched.
    // Once we hold ptable.lock, we can be
    // guaranteed that we won't miss any wakeup
    // (wakeup runs with ptable.lock locked),
    // so it's okay to release lk.
    if(lk != &ptable.lock){  //DOC: sleeplock0
        acquire(&ptable.lock);  //DOC: sleeplock1
        release(lk);
    }
    // Go to sleep.
    curthread->chan = chan;
    int allSleeping = 1;
    // check if all threads are sleeping or so
    for(t = p->threads ; t < &p->threads[NTHREAD]; t++){
        if (t!= curthread && (t->state == RUNNABLE || t->state == RUNNING || t->state == EMBRYO)){
            allSleeping = 0;
            goto end;
        }

    }
    end:
    if (allSleeping == 1)
    {
        p->state = SLEEPING;
        curthread->state = SLEEPING;
    }
    else {
        curthread->state = SLEEPING;
        p->state = RUNNABLE;
    }
    sched();

    // Tidy up.
    curthread->chan = 0;

    // Reacquire original lock.
    if(lk != &ptable.lock){  //DOC: sleeplock2
        release(&ptable.lock);
        acquire(lk);
    }
}

//PAGEBREAK!
// find all procs that are ready or running and then find the threads sleeping on chan
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
    struct proc *p;
    struct thread *t;
    //cprintf("wakeup1:%d\n",myproc()->pid);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if( p->state == SLEEPING ||p->state == RUNNABLE || p->state == RUNNING ){
            for(t = p->threads ; t < &p->threads[NTHREAD]; t++)
                if(t->state == SLEEPING && t->chan == chan){
//                    if(p->state == SLEEPING)
//                        cprintf("woke%d\n",p->pid);
                    p->state = RUNNABLE;
                    t->state = RUNNABLE;



                }

        }
    }

}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
    acquire(&ptable.lock);
    wakeup1(chan);
    release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
    struct proc *p;
    struct thread *t;
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->pid == pid){
            p->killed = 1;
            // Wake process from sleep if necessary.
            if(p->state == SLEEPING){
                p->state = RUNNABLE;
// we wake the first  thread that is sleeping so that at trap the process could go to exit
                for(t=p->threads; t<&p->threads[NTHREAD]; t++){
                    if(t->state == SLEEPING){
                        t->state = RUNNABLE;
                        goto die;
                    }

                }
            }

            die:
            release(&ptable.lock);
            return 0;
        }
    }
    release(&ptable.lock);
    return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
    static char *states[] = {
            [UNUSED]    "unused",
            [EMBRYO]    "embryo",
            [SLEEPING]  "sleep ",
            [RUNNABLE]  "runble",
            [RUNNING]   "run   ",
            [ZOMBIE]    "zombie"
    };
    int i;
    struct proc *p;
    struct thread *t;
    char *state;
    uint pc[10];

    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->state == UNUSED)
            continue;
        for( t = p->threads; t < &p->threads[NTHREAD] ; t++){
            if(t->state == UNUSED)
                continue;
            if(t->state >= 0 && t->state < NELEM(states) && states[t->state])
                state = states[t->state];
            else
                state = "???";
            cprintf("%d %s %s", p->pid, state, p->name);
            if(p->state == SLEEPING){

                getcallerpcs((uint*)t->context->ebp+2, pc);
                for(i=0; i<10 && pc[i] != 0; i++)
                    cprintf(" %p", pc[i]);
            }
            //cprintf("\n");
        }
    }
}

void kthread_exit(){
    struct proc *curproc = myproc();
    struct thread * curthread = mythread();
    //  struct thread * t;
//    struct proc *p;

    acquire(curproc->ttlock);
    wakeup1(curthread);
    int areAllout = allzombies(curproc,curthread);


    if(areAllout == 1){
        //   cprintf("allout:%d\n",curproc->pid);
        release(curproc->ttlock);
        exit();
    }

    curthread->state = ZOMBIE;

    release(curproc->ttlock);

    acquire(&ptable.lock);

    curproc->state = RUNNABLE;
    //cprintf("e%d\n", curthread->tid);

    sched();
    panic("zombie exit");

}
int kthread_id(){
    acquire(myproc()->ttlock);
    int id =  mythread()->tid;
    release(myproc()->ttlock);
    return id;
}
//must be called under lock;
void clear_thread(struct thread* t){
    t->chan = 0;
    kfree(t->kstack);
    t->kstack = 0;
    t->tid = -1;
    t->killed = 0;
    t->blocked = 0;
    t->state = UNUSED; // make it available again
}


int kthread_join(int thread_id){


    struct proc *curproc = myproc();
    struct thread * currthread = mythread();

    //check that the id is valid
    if(thread_id == currthread->tid || thread_id < 0 || thread_id > 15)
        return -1;

    acquire(curproc->ttlock);
    struct thread *t = &curproc->threads[thread_id];
    if(t->tid != thread_id){
        release(curproc->ttlock);
        return -1;
    }

   // cprintf("wj%d\n", t->tid);
    //wait until requested thread is zombie
    for(;;){
        // Scan through table looking for exited thread with the tid.

        if (t->state == ZOMBIE) {
          //  cprintf("j%d\n", t->tid);
            clear_thread(t);
            release(curproc->ttlock);
            return 0;

        }
        // if there is no thread with that id its an error.

        if(currthread->killed){
            release(curproc->ttlock);
          //  cprintf("nj\n");
            return -1;
        }
        sleep(t,curproc->ttlock);  //DOC: wait-sleep
       // cprintf("nz%d\n", t->tid);
        // Wait for thread to exit.
    }
}
int kthread_create(void (*start_func)(), void* stack){

    struct proc * proc = myproc();
    struct thread  * curthread = mythread();
    struct thread * t = allocthread(proc, KSTACKSIZE);
    if(t == 0)
        return -1;

    *t->tf = *curthread->tf;
    t->tf->eip = (uint)start_func;
    t->tf->esp = (uint)(stack + MAX_STACK_SIZE); // start point in stack
    t->tf->eax = 0;
    t->myproc = proc;

    acquire(proc->ttlock);

    t->state = RUNNABLE;

    release(proc->ttlock);

    return t->tid;
}


int kthread_mutex_alloc(){

    struct proc* curproc = myproc();
    struct kthread_mutex* m;
    int i = 0;
    acquire(&mtable.lock);

    for(m = mtable.mutex; m < &mtable.mutex[MAX_MUTEXES]; m++){
        if(m->allocated == 0){
            m->allocated = 1;
            m->name = curproc->name;
            m->locked = 0;
            m->tid = -1;
            acquire(&ptable.lock);
            curproc->mid[i] = 1;
            release(&ptable.lock);
            release(&mtable.lock);
            return i;
        }
        i++;
    }
    release(&mtable.lock);
    //cprintf("all the mutex allocated\n");
    return -1;
}



int kthread_mutex_dealloc(int mutex_id){
    //cprintf("m%d starting Deallocation\n", mutex_id);
    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES)
        return -1;

    struct proc* curproc = myproc();

    acquire(&mtable.lock);
    struct kthread_mutex* m = &mtable.mutex[mutex_id];
    // if not allocated
    if(m->allocated == 0) {
        //cprintf("not allocated\n");
        release(&mtable.lock);
        return -1;
    }


    acquire(&ptable.lock);
    // if lock is not allocated by proc
    if (curproc->mid[mutex_id]==0){
        //cprintf("not of proc\n");
        release(&ptable.lock);
        release(&mtable.lock);
        return -1;
    }
    release(&ptable.lock);


    // if is locked
    if(m->locked == 1) {
        release(&mtable.lock);
        return -1;
    }
    //if not locked there is no other thread waiting for this mutex
    m->allocated = 0;

    m->name = "";
    m->locked = 0;

    m->tid = -1;

    acquire(&ptable.lock);
    curproc->mid[mutex_id] = 0;
    release(&ptable.lock);
    release(&mtable.lock);
    //cprintf("m%d Dealocated\n", mutex_id);
    return 0;
}
int kthread_mutex_lock(int mutex_id){
// validate id
    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES) {
        //cprintf("bad id");
        return -1;
    }

    struct proc* curproc = myproc();
    struct thread* curthread = mythread();

    acquire(&mtable.lock);
    struct kthread_mutex* m = &mtable.mutex[mutex_id];

    // if not allocated
    if(m->allocated == 0) {
      //  cprintf("not allocated\n");
        release(&mtable.lock);
        return -1;
    }


    acquire(&ptable.lock);
    if (curproc->mid[mutex_id]==0){
      //  cprintf("not of proc\n");
        release(&ptable.lock);
        release(&mtable.lock);
        return -1;
    }
    release(&ptable.lock);


    if (m->tid == curthread->tid) {
       // cprintf("allready locked\n");
        release(&mtable.lock);
        return -1;
    }
    if (m->locked == 1) {
        acquire(curproc->ttlock);
        curthread->blocked = 1;
        release(curproc->ttlock);
        sleep(m, &mtable.lock);
    }

    acquire(curproc->ttlock);
    if(curthread->blocked == 1){
        release(curproc->ttlock);
        release(&mtable.lock);
        panic("Thread is Blocked");
    }
    release(curproc->ttlock);

    m->locked = 1;
    m->tid = curthread->tid;
    release(&mtable.lock);
    //cprintf("I%d,M%d\n", curthread->tid, mutex_id);
    return 0;


}

int kthread_mutex_unlock(int mutex_id){
//valiate id
    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES)
        return -1;

    struct proc* curproc = myproc();
    struct thread* curthread = mythread();
    acquire(&mtable.lock);
    struct kthread_mutex* m = &mtable.mutex[mutex_id];

    // if not allocated
    if(m->allocated == 0) {
      //  cprintf("not allocated\n");
        release(&mtable.lock);
        return -1;
    }


    acquire(&ptable.lock);

    if (curproc->mid[mutex_id]==0){
       // cprintf("not of proc\n");
        release(&ptable.lock);
        release(&mtable.lock);
        return -1;
    }
    release(&ptable.lock);


    //make sure calling thread is holding thread
    if(curthread->tid !=m->tid ){
     //   cprintf("allready locked by other thread\n");
        release(&mtable.lock);
        return -1;
    }

    struct thread* t = curthread;
    t++;
    if(t == &curproc->threads[NTHREAD]){
        t=curproc->threads;
    }

    //look for a thread waiting for the mutex and give it the key
    acquire(curproc->ttlock);
    while(t != curthread) {

        if(t->blocked == 1 && m == t->chan){

            t->chan = 0;
            t->state = RUNNABLE;
            t->blocked = 0;
            m->tid = t->tid;
            release(curproc->ttlock);
            release(&mtable.lock);
           // cprintf("O%d,M%d\n", curthread->tid, mutex_id);
            return 0;
        }
        t++;
        if(t == &curproc->threads[NTHREAD]){
            t=curproc->threads;
        }

    }
    release(curproc->ttlock);

    m->locked = 0;
    m->tid = -1;
    release(&mtable.lock);
    //cprintf("O%d,M%d\n", curthread->tid, mutex_id);
    return 0;
}

//debug sys call

int mutex_tid(int mid){
    return mtable.mutex[mid].tid;
}
