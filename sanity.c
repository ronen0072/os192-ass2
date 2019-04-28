
#include "types.h"
#include "user.h"
#include "fcntl.h"
#define MAX_STACK_SIZE 4000

int testnum = 20;
int success=0, fail=0,ans=-1, fibNum=10,mid=-1;
int pids[20];
int num_threads=1;

int fib(uint num){
    if (num == 0)
        return 0;
    if(num == 1)
        return 1;
    return fib(num-1) + fib(num-2);
}

// test that processes are working properly
void test_forking(){
    int pid,rpid;
    for (int i=0;i<20;i++){
        pid = fork();
        if(pid == 0){
            fib(20);
            exit();
        }
        rpid =  wait();
        //printf(1,"rpid:%d  pid:%d\n",rpid,pid);
        if(rpid == pid)
            ans++;

    }

}
void printfibexit(){
    fib(fibNum);
    printf(1,"tid::%d\n", kthread_id());
    kthread_exit();

}

void sanity_kthread(){
    int tid;
    uint *stack;


    stack = malloc(MAX_STACK_SIZE);
    memset(stack, 0, sizeof(*stack));
    if ((tid = (kthread_create(printfibexit, stack))) < 0) {
        printf(2, "thread_create error\n");
    }

    else {ans++;
        kthread_join(tid);
    }


}

void test_full_kthread(){
   int tids[15];
    for(int i=0;i<15;i++){

        uint *stack = malloc(MAX_STACK_SIZE);
        memset(stack, 0, sizeof(*stack));
        if ((tids[i] = (kthread_create(printfibexit, stack))) < 0) {
            printf(2, "thread_create error\n");
        }
    }
    for(int i=0;i<15;i++){
        printf(1,"waiting:%d\n",tids[i]);
        kthread_join(tids[i]);
        ans++;
        printf(1,"done:%d\n",tids[i]);

    }

}


void create_extra_kthread(){
    int tids[16];
    for(int i=0;i<16;i++){

        uint *stack = malloc(MAX_STACK_SIZE);
        memset(stack, 0, sizeof(*stack));
        if ((tids[i] = (kthread_create(printfibexit, stack))) < 0) {
            ans++;
            printf(2, "thread_create error\n");
        }
    }
    for(int i=0;i<15;i++){
        kthread_join(tids[i]);
    }

}
void kthread_wrong_join(){
    ans = kthread_join(-5);

}

void mutex_alloc(){

    mid = kthread_mutex_alloc();
    if(mid >= 0)
        ans++;


}
void mutex_dealloc(){
    int ret = kthread_mutex_dealloc(mid);
    if(ret >= 0)
        ans++;

}

void mutex_dealloc_non_alocated(){
    int ret = kthread_mutex_dealloc(mid+1);
    if(ret < 0)
        ans++;

}


void mutex_dealloc_twice(){
    mid = kthread_mutex_alloc();
    kthread_mutex_dealloc(mid);
    int ret2 = kthread_mutex_dealloc(mid);
    if(ret2 < 0)
        ans=1;

}

void cs2(){

    kthread_mutex_lock(mid);
    fib(fibNum);
    kthread_mutex_unlock(mid);
    kthread_exit();

}

void mutex_bad_dealloc(){
    uint *stack1 = malloc(MAX_STACK_SIZE);
    mid = kthread_mutex_alloc();
    int tid1 = kthread_create(cs2, stack1);
    printf(1,"%d\n", kthread_id());
    fib(fibNum+1);
    ans = kthread_mutex_dealloc(mid);
    printf(1,"%d\n", kthread_id());
    kthread_join(tid1);
    kthread_mutex_dealloc(mid);

}

void sanity_mutex_lock(){

    mid = kthread_mutex_alloc();
    int ret = kthread_mutex_lock(mid);
    if(ret >= 0)
        ans++;


}
void sanity_mutex_unlock(){
    int ret = kthread_mutex_unlock(mid);
    if(ret >= 0)
        ans++;
    kthread_mutex_dealloc(mid);

}
void sanity_mutex_double_lock(){

    mid = kthread_mutex_alloc();
    kthread_mutex_lock(mid);
    ans = kthread_mutex_lock(mid);
    kthread_mutex_unlock(mid);
    kthread_mutex_dealloc(mid);

}
void cs(){
    printf(1,"%d\n", kthread_id());
    kthread_mutex_lock(mid);
    fib(fibNum);
    if(mutex_tid(mid) == kthread_id())
        ans++;
    kthread_mutex_unlock(mid);
    printf(1,"%d\n", kthread_id());
    kthread_exit();

}
//
//void mutex_lock(){
//    uint *stack1 = malloc(MAX_STACK_SIZE);
//    uint *stack2 = malloc(MAX_STACK_SIZE);
//
//    mid = kthread_mutex_alloc();
//    int tid1 = kthread_create(cs, stack1);
//    int tid2 = kthread_create(cs, stack2);
//    kthread_join(tid1);
//    kthread_join(tid2);
//    kthread_mutex_dealloc(mid);
//
//}

void mutex_lock(){
    int tid[num_threads];
    mid = kthread_mutex_alloc();
    for (int i = 0; i <num_threads ; i++) {
        uint *stack1 = malloc(MAX_STACK_SIZE);
         tid[i] = kthread_create(cs, stack1);

    }
    for (int i = 0; i <num_threads ; i++) {
        kthread_join(tid[i]);
    }
    kthread_mutex_dealloc(mid);


}




void make_test(void (*f)(void) , int expected ,char * test_name){

    printf(1,"__________starting test %s_______________________\n",test_name);
    ans = 0;
    f();
    if(ans == expected)
        success++;
    else {
        fail++;
        printf(1,"%s failed!!\n",test_name);
    }



}



int main(void){

    // __________________KTHREAD___________________
//    make_test(test_forking,20,"test_forking");
//    make_test(sanity_kthread,1,"sanity_kthread");
//    make_test(test_full_kthread,15,"sanity_kthread");
//    make_test(create_extra_kthread,1,"create_extra_kthread");
//    make_test(kthread_wrong_join,-1,"kthread_wrong_join");
//
//

   // __________________SIMPLE MUTEX___________________
//    make_test(mutex_alloc,1,"mutex_alloc");
//    make_test(mutex_dealloc,1,"mutex_dealloc");
//    make_test(mutex_dealloc_twice,1,"mutex_dealloc_twice");
//    make_test(mutex_dealloc_non_alocated,1,"mutex_dealloc_non_alocated");
//    make_test(mutex_bad_dealloc,-1,"mutex_bad_dealloc");
//
//    make_test(sanity_mutex_lock,1,"sanity_mutex_lock");
//    make_test(sanity_mutex_unlock,1,"sanity_mutex_unlock");
    make_test(sanity_mutex_double_lock,-1,"sanity_mutex_double_lock");
//    kthread_mutex_dealloc(mid);
//    num_threads = 2; //two threads
//    make_test(mutex_lock,num_threads,"mutex_lock two threads");
//    num_threads = 7; // half full threads
//    make_test(mutex_lock,num_threads,"mutex_lock half threads");
//    num_threads = 15; // all threads
//    make_test(mutex_lock,num_threads,"mutex_lock all threads");


    // ___________________SUMMERY_______________________________
    printf(1,"num of success:%d num of failures: %d\n",success,fail );

    if(fail == 0)
        printf(1,"All tests passed!! Yay!\n");
    exit();
}
