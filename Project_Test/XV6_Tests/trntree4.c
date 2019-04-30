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
#define STACK_SIZE 700
#define DEPTH_MAX 3

int num;
volatile int dontStart;
volatile int depth;

int power(int number,int power);
int powerHelper(int currentValue,int power, int times);

#define THREAD_START(name, id) \
    void name(){ \
        trnmnt_tree* tree; \
        int result; \
        tree = trnmnt_tree_alloc(depth); \
        if(tree == 0){  \
            printf(1,"4 trnmnt_tree allocated unsuccessfully\n"); \
        } \
        result = trnmnt_tree_acquire(tree, id - 1); \
        if(result < 0){  \
            printf(1,"trnmnt_tree locked unsuccessfully\n"); \
        } \
        result = trnmnt_tree_release(tree, id - 1); \
        if(result < 0){ \
            printf(1,"trnmnt_tree unlocked unsuccessfully\n"); \
        } \
        result = trnmnt_tree_dealloc(tree); \
        if(result == -1){ \
            printf(1,"1 trnmnt_tree deallocated unsuccessfully\n"); \
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
    int pid;

    printf(1,"Starting tournament test 4\n");

    for(int i = 0;i < DEPTH_MAX;i++){
        depth = i + 1;
        printf(1,"\n---------------------------------------\nStarted current test for %d depth\n",i+1);

        if((pid = fork()) == 0){
            initiateMutexTest();
            exit();
        }
        else if(pid > 0){
            wait();
        }
        else{
            printf(1,"fork failed\n");
        }

        printf(1,"Finished current test for %d depth\n---------------------------------------\n",i+1);
    }
    
    exit();
}

void initiateMutexTest(){
    dontStart = 1;
    int pids[THREAD_NUM];
    int result;
    int num_threads = power(2, depth);

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


    for(int i = 0;i < num_threads;i++){
        pids[i] = kthread_create(threads_starts[i], threads_stacks[i]);
    }

    dontStart = 0;
    
    for(int i = 0;i < num_threads;i++){
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
}

// Calculating math power
int power(int number,int power){
    if(power == 0){
        return 1;
    }
    else if(power == 1){
        return number;
    }
    else{
        return powerHelper(1, number, power);
    }
}

// Calculating math power helper
int powerHelper(int currentValue,int power, int times){
    if(times == 0){
        return currentValue;
    }
    else{
        return powerHelper(currentValue * power, power, times - 1);
    }
}

