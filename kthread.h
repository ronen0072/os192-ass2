#define MAX_STACK_SIZE 4000
#define MAX_MUTEXES 64



enum threadstate { UNUSED, SLEEPING, RUNNABLE, RUNNING };
//pre thread state
struct thread {
  void *chan;                  // If non-zero, sleeping on chan (TODO:move completly from proc)
  int tid;                     // Thread ID
  enum threadstate state;      // Process state
  struct context *context;     // swtch() here to run thread in process (TODO:the contex is now thread based)
  char * kstack;                // Bottom of kernel stack for this process
}
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

trnmnt_tree* trnmnt_tree_alloc(int depth);
int trnmnt_tree_dealloc(trnmnt_tree* tree);
int trnmnt_tree_acquire(trnmnt_tree* tree,int ID);
int trnmnt_tree_release(trnmnt_tree* tree,int ID);
