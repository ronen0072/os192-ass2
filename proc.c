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

    //look for a thread (should be first)
    for(t = p->threads; t < &p->threads[NTHREAD]; t++){
        if(t->state!= UNUSED)
            continue;
        else goto found;
    }

    release(p->ttlock);
    return 0;

    found:
    t->state = EMBRYO;
    t->tid = nexttid++;
    t->myproc = p;
    release(p->ttlock);


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
    p->state = EMBRYO;
    for(int index  = 0; index < MAX_MUTEXES; index++){
        p->mid[index] = 0;
    }
    p->ttlock  = &ptable.lock;
    //initlock(&p->ttlock,"threads_lock");
    p->pid = nextpid++;
    release(&ptable.lock);

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

    sz = curproc->sz;
    if(n > 0){
        if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
            return -1;
    } else if(n < 0){
        if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
            return -1;
    }
    curproc->sz = sz;
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
    // Copy process state from proc.
    if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
        kfree(nt->kstack);
        nt->kstack = 0;
        nt->state = UNUSED;
        np->state = UNUSED;
        return -1;
    }
    np->sz = curproc->sz;
    np->parent = curproc;
    nt->parent = curthread;

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

    np->state = RUNNABLE;

    release(&ptable.lock);

    return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{

    //  cprintf("Got to Exit\n");
    struct proc *curproc = myproc();
    struct thread * t;
    struct proc *p;
    int fd;
    struct thread * curthread = mythread();
    int numZombies =0;
    //cprintf("cpu %d with proc %s is starting exit\n",mycpu()->apicid,myproc()->name);
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

    // Parent might be sleeping in wait().
    wakeup1(curproc->parent);
    // Pass abandoned children to init.
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if(p->parent == curproc){
            cprintf("proc %s has kids\n",p->name);
            p->parent = initproc;
            if(p->state == ZOMBIE)
                wakeup1(initproc);
        }

    }
    // cprintf("proc %s has  no kids\n",p->name);


    //make all threads exit at trap

    for(t=curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if(t->state!=UNUSED){
            t->killed = 1;
            t->chan = 0; // thread might have been sleeping, we want to make sure no one wakes it up
        }
        if(t->state == RUNNABLE || t->state == SLEEPING){
            t->state = ZOMBIE;
        }

    }

    //count number of Zombies in proc;
    curthread->state = ZOMBIE;
    for(t=curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if(t->state == ZOMBIE || t->state == UNUSED)
            numZombies++;
    }
    // cprintf("all kids are Zombies\n");

    if(numZombies == NTHREAD)
        curproc->state = ZOMBIE;
    else{
        curproc->state = RUNNABLE;
    }
    // proc will be zombie when last child has exited

    // Jump into the scheduler, never to return.
    // current thread won't get back since it will exit at trap
    sched();
    panic("zombie exit");
}




// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{


    struct proc *p;
    int havekids, pid;
    struct proc *curproc = myproc();
//  struct thread * currthrad = mythread();
    struct thread *t;

    acquire(&ptable.lock);
    for(;;){
        // Scan through table looking for exited children.
        havekids = 0;
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->parent != curproc)
                continue;
            havekids = 1;
            if(p->state == ZOMBIE){
                //cprintf("got to wait with ZOMBIE child");
                // Found one. clean threads
                for(t=p->threads; t<&p->threads[NTHREAD]; t++){
                    t->tid = 0;
                    t->killed = 0;
                    if(t->state == ZOMBIE){
                        t->state = UNUSED;
                        kfree(t->kstack);
                        t->kstack=0;
                        continue;
                    }
                }
                pid = p->pid;
                for(int index  = 0; index < MAX_MUTEXES; index++){
                    p->mid[index] = 0;
                }
                p->pid = 0;
                p->parent = 0;
                p->name[0] = 0;
                p->killed = 0;
                p->state = UNUSED;
                freevm(p->pgdir);
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
    curthread->state = SLEEPING;
    int allSleeping = 1;

    for(t = p->threads ; t < &p->threads[NTHREAD]; t++){
        allSleeping &= (t->state == SLEEPING || t->state == UNUSED || t->state == ZOMBIE);
    }

    if (allSleeping){
        p->state = SLEEPING;
    }
    else p->state = RUNNABLE;
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

    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
        if( p->state == SLEEPING ||p->state == RUNNABLE || p->state == RUNNING ){
            for(t = p->threads ; t < &p->threads[NTHREAD]; t++)
                if(t->state == SLEEPING && t->chan == chan){
                    t->state = RUNNABLE;
                    p->state = RUNNABLE;
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
            cprintf("\n");
        }
    }
}

void kthread_exit(){
    struct proc *curproc = myproc();
    struct thread * curthread = mythread();
    struct thread * t;
//    struct proc *p;
    int areAllout = 1;
    acquire(curproc->ttlock);


    wakeup1(curthread);


    for(t=curproc->threads; t<&curproc->threads[NTHREAD]; t++){
        if(t!=curthread && (t->state != UNUSED || t->state != ZOMBIE)){
            areAllout =  0;

        }
    }

    curthread->state = ZOMBIE;


    release(curproc->ttlock);

//
//    //unlock any mutex the thread might be holding
//    for(int i=0;i<MAX_MUTEXES;i++){
//        if(kthread_mutex_unlock(i)==0)
//            cprintf("holding mutex lock!");
//    }

    if(areAllout == 1){
        exit();
    }
    acquire(&ptable.lock);

    curproc->state = RUNNABLE;


    sched();
    panic("zombie exit");



}
int kthread_id(){
    return mythread()->tid;
}

int kthread_join(int thread_id){

    struct proc *curproc = myproc();
    struct thread * currthread = mythread();
    struct thread *t;
    //struct thread *

    acquire(curproc->ttlock);
    for(t = curproc->threads; t < &curproc->threads[NTHREAD]; t++) {

        if (t->tid == thread_id)
            goto found;
    }
    release(curproc->ttlock);
    return -1;

    found:
    for(;;){
        // Scan through table looking for exited thread with the tid.

        if (t->state == ZOMBIE) {
            t->state = UNUSED;
            kfree(t->kstack);
            t->kstack = 0;
            t->tid = 0;
            t->killed = 0;
            release(&ptable.lock);
            return 0;

        }

        // if there is no thread with that id its an error.

        if( currthread->killed){
            release(&ptable.lock);
            return -1;
        }
        sleep(t,curproc->ttlock);  //DOC: wait-sleep

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
    t->tf->esp = (uint)(stack+MAX_STACK_SIZE); // start point in stack
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
            curproc->mid[i] = 1;
            //initlock(&m->lk, "mutex lock");
            m->name = curproc->name;
            m->locked = 0;
            m->tid = 0;
            release(&mtable.lock);
            return i;
        }
        i++;
    }
    release(&mtable.lock);
    return -1;
}



int kthread_mutex_dealloc(int mutex_id){

    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES)
        return -1;

    struct kthread_mutex* m = &mtable.mutex[mutex_id];
    struct proc* curproc = myproc();


    acquire(curproc->ttlock);
    //("%d\n", kthread_id());
    if (curproc->mid[mutex_id]==0){
        release(curproc->ttlock);
        return -1;
    }
    //cprintf("%d\n", kthread_id());

    release(curproc->ttlock);
    acquire(&mtable.lock);

    if(m->allocated == 0) {
        release(&mtable.lock);
        return -1;
    }
    if(m->locked == 1) {
        release(&mtable.lock);
        return -1;
    }
    //if not locked there is no other thread waiting for this mutex
    m->allocated = 0;
    acquire(curproc->ttlock);
    curproc->mid[mutex_id] = 0;
    release(curproc->ttlock);
    m->name = "";
    m->locked = 0;
    m->tid = 0;
    release(&mtable.lock);
    cprintf("%d\n", kthread_id());
    return 0;
}
int kthread_mutex_lock(int mutex_id){

    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES) {
        cprintf("bad id\n");
        return -1;
    }

    struct kthread_mutex* m = &mtable.mutex[mutex_id];
    struct proc* curproc = myproc();
    struct thread* curthread = mythread();

    acquire(&ptable.lock);
    if (curproc->mid[mutex_id]==0){
        return -1;
    }
    release(&ptable.lock);
    acquire(&mtable.lock);
    if(m->allocated == 0) {
        cprintf("not allocated\n");
        release(&mtable.lock);
        return -1;
    }
    if (m->tid == curthread->tid) {
        cprintf("the mutex is already lock by this thread\n");
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
        panic("Thread is Blocked");
    }
    release(curproc->ttlock);

    m->locked = 1;
    m->tid = curthread->tid;
    release(&mtable.lock);
    return 0;


}

int kthread_mutex_unlock(int mutex_id){

    if (mutex_id < 0 || mutex_id >= MAX_MUTEXES)
        return -1;

    struct kthread_mutex* m = &mtable.mutex[mutex_id];
    struct proc* curproc = myproc();
    struct thread* curthread = mythread();


    acquire(&ptable.lock);

    if (curproc->mid[mutex_id]==0){
        release(&ptable.lock);
        return -1;
    }
    release(&ptable.lock);
    acquire(&mtable.lock);


    if(m->allocated == 0) {
        release(&mtable.lock);
        return -1;
    }
    //make sure calling thread is holding thread
    if(curthread->tid != mtable.mutex[mutex_id].tid ){
        release(&mtable.lock);
        return -1;
    }
    struct thread* t = curthread;
    t++;
    if(t == &curproc->threads[NTHREAD]){
        t=curproc->threads;
    }
    while(t != curthread) {
        acquire(curproc->ttlock);
        if(t->blocked == 1 && m == t->chan){
            t->chan = 0;
            t->state = RUNNABLE;
            release(curproc->ttlock);
            t->blocked = 0;
            m->tid = t->tid;
            release(&mtable.lock);
            return 0;
        }
        t++;
        if(t == &curproc->threads[NTHREAD]){
            t=curproc->threads;
        }
        release(curproc->ttlock);
    }

    m->locked = 0;
    m->tid = 0;
    release(&mtable.lock);
    return 0;
}

//debug sys call

int mutex_tid(int mid){


    return mtable.mutex[mid].tid;
}