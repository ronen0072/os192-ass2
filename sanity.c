
#include "types.h"
#include "user.h"
#include "fcntl.h"

int testnum = 20;
int success=0, fail=0,ans=-1, fibNum=40;
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

void sanity_kthread(){
    int tid;
    kthread_create(fib, );

}


void make_test(void (*f)(void) , int expected ,char * fail_msg){

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
    printf(1,"num of success:%d num of failures: %d\n",success,fail );
    if(fail == 0)
        printf("All tests passed!! Yay!");
    exit();
  }
