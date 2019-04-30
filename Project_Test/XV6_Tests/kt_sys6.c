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

#define THREAD_NUM 16
#define STACK_SIZE 500

#define THREAD_START(name, id) \
    void name(){ \
        sleep( id * 200); \
        printf(1,"thread %d entering\n", id ); \
        printf(1,"thread %d exiting\n", id ); \
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
THREAD_START(threadStart_16, 16)
THREAD_START(threadStart_17, 17)
THREAD_START(threadStart_18, 18)
THREAD_START(threadStart_19, 19)
THREAD_START(threadStart_20, 20)
THREAD_START(threadStart_21, 21)
THREAD_START(threadStart_22, 22)
THREAD_START(threadStart_23, 23)
THREAD_START(threadStart_24, 24)
THREAD_START(threadStart_25, 25)
THREAD_START(threadStart_26, 26)
THREAD_START(threadStart_27, 27)
THREAD_START(threadStart_28, 28)
THREAD_START(threadStart_29, 29)
THREAD_START(threadStart_30, 30)
THREAD_START(threadStart_31, 31)
THREAD_START(threadStart_32, 32)

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
     threadStart_15,
     threadStart_16,
     threadStart_17,
     threadStart_18,
     threadStart_19,
     threadStart_20,
     threadStart_21,
     threadStart_22,
     threadStart_23,
     threadStart_24,
     threadStart_25,
     threadStart_26,
     threadStart_27,
     threadStart_28,
     threadStart_29,
     threadStart_30,
     threadStart_31,
     threadStart_32};

int main(int argc, char *argv[]){
    int pids[THREAD_NUM*2];

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
    THREAD_STACK(threadStack_16)
    THREAD_STACK(threadStack_17)
    THREAD_STACK(threadStack_18)
    THREAD_STACK(threadStack_19)
    THREAD_STACK(threadStack_20)
    THREAD_STACK(threadStack_21)
    THREAD_STACK(threadStack_22)
    THREAD_STACK(threadStack_23)
    THREAD_STACK(threadStack_24)
    THREAD_STACK(threadStack_25)
    THREAD_STACK(threadStack_26)
    THREAD_STACK(threadStack_27)
    THREAD_STACK(threadStack_28)
    THREAD_STACK(threadStack_29)
    THREAD_STACK(threadStack_30)
    THREAD_STACK(threadStack_31)
    THREAD_STACK(threadStack_32)

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
     threadStack_15,
     threadStack_16,
     threadStack_17,
     threadStack_18,
     threadStack_19,
     threadStack_20,
     threadStack_21,
     threadStack_22,
     threadStack_23,
     threadStack_24,
     threadStack_25,
     threadStack_26,
     threadStack_27,
     threadStack_28,
     threadStack_29,
     threadStack_30,
     threadStack_31,
     threadStack_32};

    for(int i = 0;i < THREAD_NUM;i++){
        pids[i] = kthread_create(threads_starts[i], threads_stacks[i]);

        if(pids[i] > 0){
            printf(1,"Created thread %d successfully\n",i+1);
        }
        else{
            printf(1,"Created thread %d unsuccessfully\n",i+1);
        }
    }

    printf(1,"Starting joining threads, should indicate when each thread was sucessfully joined soon\n");

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

    
    for(int i = 0;i < THREAD_NUM;i++){
        printf(1,"Attempting to join thread %d\n",i+1);

        int result = kthread_join(pids[i]);
        if(result == 0){
            printf(1,"Thread %d shouldn't be my thread anymore\n",i+1);
        }
        else if(result == -1){
            printf(1,"Thread %d isn't my thread anymore, as it should be\n",i+1);
        }
        else{
            printf(1,"Unknown result code from join\n");
        }
    }    

    for(int i = 16;i < THREAD_NUM*2;i++){
        pids[i] = kthread_create(threads_starts[i], threads_stacks[i]);

        if(pids[i] > 0){
            printf(1,"Created thread %d successfully\n",i+1);
        }
        else{
            printf(1,"Created thread %d unsuccessfully\n",i+1);
        }
    }

    for(int i = 16;i < THREAD_NUM*2;i++){
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

    
    for(int i = 16;i < THREAD_NUM*2;i++){
        printf(1,"Attempting to join thread %d\n",i+1);

        int result = kthread_join(pids[i]);
        if(result == 0){
            printf(1,"Thread %d shouldn't be my thread anymore\n",i+1);
        }
        else if(result == -1){
            printf(1,"Thread %d isn't my thread anymore, as it should be\n",i+1);
        }
        else{
            printf(1,"Unknown result code from join\n");
        }
    } 

    exit();
}

