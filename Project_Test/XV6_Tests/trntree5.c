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

#define THREAD_NUM 4
#define STACK_SIZE 500

trnmnt_tree* tree;
volatile int dontStart;
volatile int conditionOne = 0;
volatile int conditionTwo = 0;
int result;

#define THREAD_STACK(name) \
    void * name = ((char *) malloc(STACK_SIZE * sizeof(char))) + STACK_SIZE;

void threadStart_1(){
    int result;
    while(dontStart){} 

    result = trnmnt_tree_acquire(tree, 0); 
    if(result < 0){  
        printf(1,"trnmnt_tree locked unsuccessfully\n"); 
    } 

    conditionOne = 1;
    sleep(400);
    
    if(conditionOne == 1){
        printf(1,"mutual exclusion satisfied\n"); 
    }
    else{
        printf(1,"mutual exclusion not satisfied with %d condition value\n",conditionOne); 
    }

    result = trnmnt_tree_release(tree, 0); 
    if(result < 0){ 
        printf(1,"trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    kthread_exit(); 
}

void threadStart_2(){
    int result;
    while(dontStart){} 
    sleep(200);

    result = trnmnt_tree_acquire(tree, 1); 
    if(result < 0){  
        printf(1,"trnmnt_tree locked unsuccessfully\n"); 
    } 

    conditionOne = -1;

    result = trnmnt_tree_release(tree, 1); 
    if(result < 0){ 
        printf(1,"trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    kthread_exit(); 
}

void threadStart_3(){
    while(dontStart){} 

    conditionTwo = 1;
    sleep(200);
    
    if(conditionTwo == 1){
        printf(1,"Test not creating true critical section\n"); 
    }
    
    kthread_exit(); 
}

void threadStart_4(){
    while(dontStart){} 
    sleep(50);

    conditionTwo = -1;

    kthread_exit(); 
}

void (*threads_starts[])(void) = 
    {threadStart_1,
     threadStart_2,
     threadStart_3,
     threadStart_4};

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
    THREAD_STACK(threadStack_3)
    THREAD_STACK(threadStack_4)

    void (*threads_stacks[])(void) = 
    {threadStack_1,
     threadStack_2,
     threadStack_3,
     threadStack_4};

    tree = trnmnt_tree_alloc(1); 
    if(tree == 0){ 
        printf(1,"trnmnt_tree allocated unsuccessfully\n"); 
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

    result = trnmnt_tree_dealloc(tree); 
    if(result == -1){ 
        printf(1,"trnmnt_tree deallocated unsuccessfully\n"); 
    } 
}

