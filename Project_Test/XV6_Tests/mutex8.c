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

#define THREAD_NUM 2
#define STACK_SIZE 500

int mid;
volatile int dontStart;
int result;

#define THREAD_STACK(name) \
    void * name = ((char *) malloc(STACK_SIZE * sizeof(char))) + STACK_SIZE;

void threadStart_1(){
    int result;
    while(dontStart){} 
    sleep(40);
    
    printf(1,"Thread 1 starting...\n");
    
    printf(1,"Thread 1 unlocking unlocked lock...\n");
    result = kthread_mutex_unlock(mid); 
    if(result >= 0){ 
        printf(1,"mutex locked successfully where it should not have been\n"); 
    } 

    printf(1,"Thread 1 locking unlocked lock...\n");
    result = kthread_mutex_lock(mid); 
    if(result < 0){  
        printf(1,"mutex locked unsuccessfully\n"); 
    } 

    sleep(400);
    
    printf(1,"Thread 1 unlocking locked lock...\n");
    result = kthread_mutex_unlock(mid); 
    if(result < 0){ 
        printf(1,"mutex unlocked unsuccessfully\n"); 
    } 

    printf(1,"Thread 1 exiting...\n");

    kthread_exit(); 
}

void threadStart_2(){
    int result;
    while(dontStart){} 
    sleep(200);
    printf(1,"Thread 2 starting...\n");

    printf(1,"Thread 2 locking no true lock...\n");
    result = kthread_mutex_lock(mid + 5); 
    if(result >= 0){  
        printf(1,"mutex locked successfully where it should not have been\n"); 
    } 
    
    printf(1,"Thread 2 unlocking locked lock when not owner...\n");
    result = kthread_mutex_unlock(mid); 
    if(result >= 0){ 
        printf(1,"mutex unlocked successfully where it should not have been\n"); 
    } 

    printf(1,"Thread 2 unlocking no true lock...\n");
    result = kthread_mutex_unlock(mid + 10); 
    if(result >= 0){ 
        printf(1,"mutex unlocked successfully where it should not have been\n"); 
    } 

    printf(1,"Thread 2 exiting...\n");

    kthread_exit(); 
}

void (*threads_starts[])(void) = 
    {threadStart_1,
     threadStart_2};

void initiateMutexTest();

int main(int argc, char *argv[]){
    initiateMutexTest();
    exit();
}

void initiateMutexTest(){
    dontStart = 1;
    int pids[THREAD_NUM];

    THREAD_STACK(threadStack_1)
    THREAD_STACK(threadStack_2)

    void (*threads_stacks[])(void) = 
    {threadStack_1,
     threadStack_2};

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
}

