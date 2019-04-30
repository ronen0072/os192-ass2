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
#define DEPTH_MAX 4

trnmnt_tree* tree;
volatile int dontStart;
int result;
volatile int user;
volatile int depth;

int power(int number,int power);
int powerHelper(int currentValue,int power, int times);

#define THREAD_STACK(name) \
    void * name = ((char *) malloc(STACK_SIZE * sizeof(char))) + STACK_SIZE;

void threadStart_1(){
    int result;
    while(dontStart){} 

    result = trnmnt_tree_acquire(tree, user); 
    if(result < 0){  
        printf(1,"trnmnt_tree locked unsuccessfully\n"); 
    } 

    sleep(400);
    
    result = trnmnt_tree_release(tree, user); 
    if(result < 0){ 
        printf(1,"trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    kthread_exit(); 
}

void threadStart_2(){
    int result;
    while(dontStart){} 
    sleep(200);

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){ 
        printf(1,"trnmnt_tree deallocated successfully where it should not have been\n"); 
    } 

    sleep(600);

    result = trnmnt_tree_dealloc(tree); 
    if(result == -1){ 
        printf(1,"trnmnt_tree deallocated unsuccessfully where it should have been\n"); 
    } 

    kthread_exit(); 
}

void (*threads_starts[])(void) = 
    {threadStart_1,
     threadStart_2};

void initiateMutexTest();

int main(int argc, char *argv[]){
    int pid;

    printf(1,"Starting tournament test 9\n");

    for(int i = 0;i < DEPTH_MAX;i++){
        depth = i + 1;
        printf(1,"\n---------------------------------------\nStarted current test for %d depth\n",i+1);

        if((pid = fork()) == 0){
            int num_threads = power(2, depth);
            for(int j = 0;j < num_threads;j++){
                user = j;

                printf(1, "$$$$$$$$$$$$$$$$$$$$$$$$$$\nStarting test for %d depth with %d user\n",depth, user);
                initiateMutexTest();
                printf(1, "Finishing test for %d depth with %d user\n$$$$$$$$$$$$$$$$$$$$$$$$$$\n",depth, user);
            }
            
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
    
    THREAD_STACK(threadStack_1)
    THREAD_STACK(threadStack_2)

    void (*threads_stacks[])(void) = 
    {threadStack_1,
     threadStack_2};

    tree = trnmnt_tree_alloc(depth); 
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
