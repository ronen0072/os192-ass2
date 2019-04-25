
#include "types.h"
#include "user.h"
#include "fcntl.h"
#define MAX_STACK_SIZE 4000

int testnum = 20;
int success=0, fail=0,ans=-1, fibNum=25;
int pids[20];

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


void make_test(void (*f)(void) , int expected ,char * fail_msg){


    printf(1,"__________starting test_______________________\n");
    f();
    if(ans == expected)
        success++;
    else {
        fail++;
        printf(1,"%s\n",fail_msg);
    }



}

int main(void){

    ans = 0;
    make_test(test_forking,20,"test forking didn't work\n");

    ans = 0;
    make_test(sanity_kthread,1,"faild to create\n");

    ans = 0;
    make_test(test_full_kthread,15,"faild to create\n");

    ans = 0;
    make_test(create_extra_kthread,1,"extra created!\n");

    ans = 0;
    make_test(kthread_wrong_join,-1,"wrong join not error!\n");

    printf(1,"num of success:%d num of failures: %d\n",success,fail );

    if(fail == 0)
        printf(1,"All tests passed!! Yay!\n");
    exit();
}
