#include "../../param.h"
#include "../../types.h"
#include "../../stat.h"
#include "../../user.h"
#include "../../fs.h"
#include "../../fcntl.h"
#include "../../syscall.h"
#include "../../traps.h"
#include "../../memlayout.h"
#include "../../tournament_tree.h"
#include "../../kthread.h"

#define THREAD_NUM 15
#define STACK_SIZE 500

int mid = -1;
volatile int dontStart;
int num = 0;

#define THREAD_START(name, id) \
    void name(){ \
        int result; \
        while(dontStart){}; \
        result = kthread_mutex_lock(mid); \
        if(result < 0){  \
            printf(1,"mutex locked unsuccessfully\n"); \
        } \
        for(int i = 0;i < 500;i++){ \
            int temp = num; \
            temp++; \
            sleep(1); \
            num = temp; \
        } \
        result = kthread_mutex_unlock(mid); \
        if(result < 0){ \
            printf(1,"mutex unlocked unsuccessfully\n"); \
        } \
        kthread_exit(); \
    } 
#define THREAD_STACK(name) \
    void * name = ((char *) malloc(STACK_SIZE * sizeof(char))) + STACK_SIZE;

THREAD_START(threadStart_1, 1)
THREAD_START(threadStart_2, 2)
THREAD_START(threadStart_3, 3)
THREAD_START(threadStart_4, 4)
THREAD_START(threadStart_5, 5)
THREAD_START(threadStart_6, 6)
THREAD_START(threadStart_7, 7)
THREAD_START(threadStart_8, 8)
THREAD_START(threadStart_9, 9)
THREAD_START(threadStart_10, 10)
THREAD_START(threadStart_11, 11)
THREAD_START(threadStart_12, 12)
THREAD_START(threadStart_13, 13)
THREAD_START(threadStart_14, 14)
THREAD_START(threadStart_15, 15)

void (*threads_starts[])(void) = 
    {threadStart_1,
     threadStart_2,
     threadStart_3,
     threadStart_4,
     threadStart_5,
     threadStart_6,
     threadStart_7,
     threadStart_8,
     threadStart_9,
     threadStart_10,
     threadStart_11,
     threadStart_12,
     threadStart_13,
     threadStart_14,
     threadStart_15};

void initiateMutexTest();

int main(int argc, char *argv[]){
    initiateMutexTest();
    exit();
}

void initiateMutexTest(){
    dontStart = 1;
    int pids[THREAD_NUM];
    int result;
    
    THREAD_STACK(threadStack_1)
    THREAD_STACK(threadStack_2)
    THREAD_STACK(threadStack_3)
    THREAD_STACK(threadStack_4)
    THREAD_STACK(threadStack_5)
    THREAD_STACK(threadStack_6)
    THREAD_STACK(threadStack_7)
    THREAD_STACK(threadStack_8)
    THREAD_STACK(threadStack_9)
    THREAD_STACK(threadStack_10)
    THREAD_STACK(threadStack_11)
    THREAD_STACK(threadStack_12)
    THREAD_STACK(threadStack_13)
    THREAD_STACK(threadStack_14)
    THREAD_STACK(threadStack_15)

    void (*threads_stacks[])(void) = 
    {threadStack_1,
     threadStack_2,
     threadStack_3,
     threadStack_4,
     threadStack_5,
     threadStack_6,
     threadStack_7,
     threadStack_8,
     threadStack_9,
     threadStack_10,
     threadStack_11,
     threadStack_12,
     threadStack_13,
     threadStack_14,
     threadStack_15};

    mid = kthread_mutex_alloc(); 
    if(mid == -1){ 
        printf(1,"mutex allocated unsuccessfully\n"); 
    } 

    for(int i = 0;i < THREAD_NUM;i++){
        pids[i] = kthread_create(threads_starts[i], threads_stacks[i]);
    }

    dontStart = 0;
    
    for(int i = 0;i < THREAD_NUM;i++){
        printf(1,"Attempting to join thread %d\n",i+1);

        int result = kthread_join(pids[i]);
        if(result == 0){
            printf(1,"Finished joing thread %d\n",i+1);
        }
        else if(result == -1){
            printf(1,"Error in joing thread %d\n",i+1);
        }
        else{
            printf(1,"Unknown result code from join\n");
        }
    }

    result = kthread_mutex_dealloc(mid); 
    if(result == -1){ 
        printf(1,"mutex deallocated unsuccessfully\n"); 
    } 

    printf(1,"Sum is %d\n",num);
}

