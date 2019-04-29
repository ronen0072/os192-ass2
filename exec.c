#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "kthread.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"


void kill_threads(struct proc * p){
  struct thread *curthread = mythread();
  struct thread * t ;
  int k=0;
  acquire(p->ttlock);
  for(t=p->threads;t< &p->threads[NTHREAD];t++){
    if(t!=curthread && t->state!=UNUSED){
        t->killed = 1;
    }


      //wake the thread if he is sleeping so he will be killed;
    if(t->state == SLEEPING)
        t->state = RUNNABLE;


  }
  release(p->ttlock);

  // wait until all other threads are killed
  while (k < NTHREAD-1){
    k=0;
      acquire(p->ttlock);
    for (t = p->threads; t< &p->threads[NTHREAD]; t++){
      if (t != curthread && (t->state == ZOMBIE || t->state == UNUSED) ){
            k++;
          // once a thread is zombie make it available again
          if(t->state == ZOMBIE){

              kfree(t->kstack);
              t->kstack = 0;
              t->blocked = 0;
              t->state = UNUSED;
              continue;
          }
      }

    }
      release(p->ttlock);

    //let another cpu catch it;
  }
  //clear all mutexes
    acquire(p->ttlock);
  for(int i=0;i<MAX_MUTEXES;i++){

      if(p->mid[i] == 1){
      //   kthread_mutex_dealloc(i);
          p->mid[i] =0;
      }
  }
    release(p->ttlock);




}
int
exec(char *path, char **argv)
{
  char *s, *last;
  int i, off;
  uint argc, sz, sp, ustack[3+MAXARG+1];
  struct elfhdr elf;
  struct inode *ip;
  struct proghdr ph;
  pde_t *pgdir, *oldpgdir;
  struct proc *curproc = myproc();
  struct thread *curthread = mythread();
 // struct thread * t ;

  // ----------------------
  //cprintf("proc %s is starting exec\n",curproc->name);

  // --------------
  kill_threads(curproc);
  begin_op();

  if((ip = namei(path)) == 0){
    end_op();
    cprintf("exec: fail\n");
    return -1;
  }
  ilock(ip);
  pgdir = 0;

  // Check ELF header
  if(readi(ip, (char*)&elf, 0, sizeof(elf)) != sizeof(elf))
    goto bad;
  if(elf.magic != ELF_MAGIC)
    goto bad;

  if((pgdir = setupkvm()) == 0)
    goto bad;

  // Load program into memory.
  sz = 0;
  for(i=0, off=elf.phoff; i<elf.phnum; i++, off+=sizeof(ph)){
    if(readi(ip, (char*)&ph, off, sizeof(ph)) != sizeof(ph))
      goto bad;
    if(ph.type != ELF_PROG_LOAD)
      continue;
    if(ph.memsz < ph.filesz)
      goto bad;
    if(ph.vaddr + ph.memsz < ph.vaddr)
      goto bad;
    if((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0)
      goto bad;
    if(ph.vaddr % PGSIZE != 0)
      goto bad;
    if(loaduvm(pgdir, (char*)ph.vaddr, ip, ph.off, ph.filesz) < 0)
      goto bad;
  }
  iunlockput(ip);
  end_op();
  ip = 0;

  // Allocate two pages at the next page boundary.
  // Make the first inaccessible.  Use the second as the user stack.
  sz = PGROUNDUP(sz);
  if((sz = allocuvm(pgdir, sz, sz + 2*PGSIZE)) == 0)
    goto bad;
  clearpteu(pgdir, (char*)(sz - 2*PGSIZE));
  sp = sz;

  // Push argument strings, prepare rest of stack in ustack.
  for(argc = 0; argv[argc]; argc++) {
    if(argc >= MAXARG)
      goto bad;
    sp = (sp - (strlen(argv[argc]) + 1)) & ~3;
    if(copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0)
      goto bad;
    ustack[3+argc] = sp;
  }
  ustack[3+argc] = 0;

  ustack[0] = 0xffffffff;  // fake return PC
  ustack[1] = argc;
  ustack[2] = sp - (argc+1)*4;  // argv pointer

  sp -= (3+argc+1) * 4;
  if(copyout(pgdir, sp, ustack, (3+argc+1)*4) < 0)
    goto bad;

  // Save program name for debugging.
  for(last=s=path; *s; s++)
    if(*s == '/')
      last = s+1;
  safestrcpy(curproc->name, last, sizeof(curproc->name));





  // Commit to the user image.
  acquire(curproc->ttlock);
  // check if other proc didnt allready tried to kill the thread/proc
  if(curproc->state == ZOMBIE || curthread->killed){
    release(curproc->ttlock);
    goto bad;
  }


  oldpgdir = curproc->pgdir;
  curproc->pgdir = pgdir;
  curproc->sz = sz;
  curthread->tf->eip = elf.entry;  // main
  curthread->tf->esp = sp;
  release(curproc->ttlock);

  switchuvm(curproc);
  freevm(oldpgdir);
  return 0;

 bad:

  if(pgdir)
    freevm(pgdir);
  if(ip){
    iunlockput(ip);
    end_op();
  }
  return -1;
}
