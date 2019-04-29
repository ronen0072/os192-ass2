#include "kthread_mutex.h"
#define MAX_STACK_SIZE 4000
#define MAX_MUTEXES 64


enum states { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

//pre thread state
struct thread {
  void *chan;                  // If non-zero, sleeping on chan (TODO:move completly from proc)
  int tid;                     // Thread ID
  int killed;                   // TODO: for exec and...
  enum states state;      // Process state
  struct context *context;     // swtch() here to run thread in process (TODO:the contex is now thread based)
  char * kstack;               // Bottom of kernel stack for this thread
  struct trapframe *tf;        // Trap frame for current syscall
  struct thread * parent;       // TODO: not sure if needed
  struct proc * myproc;
  int blocked;


};
/********************************
        The API of the KLT package
 ********************************/

int kthread_create(void (*start_func)(), void* stack);
int kthread_id();
void kthread_exit();
int kthread_join(int thread_id);

int kthread_mutex_alloc();
int kthread_mutex_dealloc(int mutex_id);
int kthread_mutex_lock(int mutex_id);
int kthread_mutex_unlock(int mutex_id);


int mutex_tid(int mid);



