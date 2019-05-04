
#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "tournament_tree.h"

#define MAX_STACK_SIZE 4000


#define STACK_CREATE(name) \
    void * name = ((char *) malloc(MAX_STACK_SIZE));

int testnum = 20;
int success=0, fail=0,ans=-1, fibNum=10,mid=-1;
int pids[20];
volatile int dnum = -1;
int depth = 1;
trnmnt_tree * tree;
int loopnum=1;

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

    STACK_CREATE(stack)
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
        STACK_CREATE(stack)
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
        STACK_CREATE(stack)
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
void kthread_exiting(){
    //printf(1,"%d\n",kthread_id());
    sleep(10*kthread_id());
    kthread_exit();

}
void test_kthread_exit(){
    int pid,rpid;
    pid = fork();
    if(pid == 0){
        for (int j = 0; j < 15; j++) {
            STACK_CREATE(stack);
            memset(stack, 0, sizeof(*stack));
            if(kthread_create(kthread_exiting, stack)<0){
                printf(1,"bad create\n");
                exit();
            }
        }
        kthread_exit();
    }
    printf(1,"Intending to wait for:%d\n", pid);
    rpid =  wait();
    printf(1,"finish to wait for:%d\n", pid);

    if(rpid == pid)
        ans++;
}

void test_kthread_join(){
    int tid[30];
    int rtid;
    for (int i = 0; i < 15; i++) {
        STACK_CREATE(stack)
        memset(stack, 0, sizeof(*stack));
        tid[i] = kthread_create(kthread_exiting, stack);
        if(tid[i]<0){
            printf(1,"Couldn't create\n");
        }
        else
            printf(1,"created %d\n",tid[i]);
    }
    for (int i = 0; i < 15; i++) {
        printf(1, "Intending to join for:%d\n", tid[i]);
        rtid = kthread_join(tid[i]);
        if(rtid == 0)
            ans++;
        printf(1, "finish to join for:%d\n", tid[i]);
    }
    for (int i = 0; i < 15; i++) {
        printf(1, "Intending to join for:%d\n", tid[i]);
        rtid = kthread_join(tid[i]);
        if(rtid == -1)
            ans++;
        printf(1, "finish to join for:%d\n", tid[i]);
    }
    for (int i = 15; i < 30; i++) {
        STACK_CREATE(stack)
        memset(stack, 0, sizeof(*stack));
        tid[i] = kthread_create(kthread_exiting, stack);
        if(tid[i]<0){
            printf(1,"Couldn't create\n");
        }
        else
            printf(1,"created %d\n",tid[i]);
    }

    for (int i = 15; i < 30; i++) {
        printf(1, "Intending to join for:%d\n", tid[i]);
        rtid = kthread_join(tid[i]);
        if(rtid == 0)
            ans++;
        printf(1, "finish to join for:%d\n", tid[i]);
    }
    for (int i = 15; i < 30; i++) {
        printf(1, "Intending to join for:%d\n", tid[i]);
        rtid = kthread_join(tid[i]);
        if(rtid == -1)
            ans++;
        printf(1, "finish to join for:%d\n", tid[i]);
    }
}

void endlessLoop_or_sleep(){
    if( kthread_id() %2 == 0){
        while(1);
    } else{
        sleep(10000);
    }
    printf(1,"Thread %d exiting, should have been killed by exit\n",kthread_id());
    kthread_exit();
}

int  numproc = 10;
void test_exit_process(){
    int rpid;
    int pid[numproc];
    for (int i=0;i<numproc;i++) {
        rpid = 0;
        pid[i] = fork();
        if (pid[i] == 0) {
            sleep(200);
            for (int j = 0; j < 15; j++) {
                STACK_CREATE(stack)
                memset(stack, 0, sizeof(*stack));
                if(kthread_create(endlessLoop_or_sleep, stack) < 0){
                    printf(1,"bad create\n");
                    exit();
                }
                // else printf(1,"created %d\n",j);
            }
            // fib(10);
            sleep(100);
            exit();
        }
        sleep(200);
    }

    for (int i=0;i<numproc;i++) {
        printf(1,"Intending to wait for:%d\n", pid[i]);
        rpid = wait();
        printf(1,"finish to wait for:%d\n", pid[i]);
        if(rpid >= 0)
            ans++;
    }
    printf(1,"test_exit_process finised\n");
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
    printf(1,"%d\n", kthread_id());
    kthread_mutex_lock(mid);
    dnum = 1;

    while(dnum != 0){
        // printf(1,"dnum:%d\n", dnum);
    }

    kthread_mutex_unlock(mid);
    kthread_exit();

}

void mutex_bad_dealloc(){
    STACK_CREATE(stack)
    memset(stack, 0, sizeof(*stack));
    mid = kthread_mutex_alloc();
    int tid1 = kthread_create(cs2, stack);
    //printf(1,"%d\n", kthread_id());
    //printf(1,"dnum:%d\n", dnum);
    while(dnum != 1){}

    ans = kthread_mutex_dealloc(mid);
    dnum = 0;
    //printf(1,"%d\n", kthread_id());
    kthread_join(tid1);
    kthread_mutex_dealloc(mid);
    // printf(1,"ans:%d\n",ans);
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
   // printf(1,"%d\n", kthread_id());
    for(int i=0;i<loopnum;i++) {
        if(kthread_mutex_lock(mid)<0){
            printf(1,"bad mutex lock\n");
            goto exit;
        }
        fib(fibNum);
        if (mutex_tid(mid) == kthread_id())
            ans++;
        if(kthread_mutex_unlock(mid) == -1){
            printf(1,"bad mutex lock\n");
            goto exit;
        }
    }
    exit:
   // printf(1,"%d\n", kthread_id());
   kthread_exit();


}


void mutex_lock(){
    int tid[num_threads];
    mid = kthread_mutex_alloc();
    if(mid<0){
        printf(1,"bad mutex aloc\n");
        return;
    }
    for (int i = 0; i <num_threads ; i++) {
        STACK_CREATE(stack)
        memset(stack, 0, sizeof(*stack));
        tid[i] = kthread_create(cs, stack);
        if(tid[i]<0){
            printf(1,"bad tcreate\n");
        }

    }
    for (int i = 0; i <num_threads ; i++) {
        if(kthread_join(tid[i])<0){
            printf(1,"bad tjoin\n");
        }
    }
    if(kthread_mutex_dealloc(mid)<0){
        printf(1,"bad mutex dealoc\n");
    }
}


// tournament_tree
void sanity_tree_alloc_dealloc(){
    trnmnt_tree * tree = trnmnt_tree_alloc(depth);
    if(tree != 0){
        ans++;
        if(trnmnt_tree_dealloc(tree) == 0)
            ans++;

        else printf(1,"bad dealloc\n");
    }
    else printf(1,"bad alloc\n");

}

void test_tournament_cs(){

    for(int i=0; i<loopnum;i++) {
        if (trnmnt_tree_acquire(tree, kthread_id()) == 0) {
            ans++;
            //printf(1, "acquire success\n");
        }
        else printf(1,"bad acquire\n");
        //printf(1,"thread:%d acquire\n",kthread_id());
        if (trnmnt_tree_release(tree, kthread_id()) == 0) {
            ans++;
            //  printf(1, "release success\n");
        }
        else printf(1,"bad release\n");
        //(1,"thread:%d release\n",kthread_id());
    }
    kthread_exit();
}

void sanity_tree_tournament(){
    tree = trnmnt_tree_alloc(depth);
    int tid[num_threads];
    if(tree != 0){

        for(int i=0; i<num_threads;i++){
            STACK_CREATE(stack)
            memset(stack, 0, sizeof(*stack));
            tid[i] = kthread_create(test_tournament_cs, stack);
        }

        for(int i=0; i<num_threads;i++){
            kthread_join(tid[i]);
        }
        if(trnmnt_tree_dealloc(tree) != 0)
            printf(1,"bad dealloc\n");

    }
    else printf(1,"bad tree alloc\n");
}

void tournament_cs(){

    for(int i=0; i<loopnum;i++) {
        if(trnmnt_tree_acquire(tree, kthread_id()) < 0)
            printf(1,"bad acquire\n");
        fib(fibNum);
        //printf(1,"thread:%d acquire\n",kthread_id());
        int kthread_tid = kthread_id();
        int trnmnt_tid = trnmnt_tree_tid(tree);
        if(kthread_tid == trnmnt_tid)
            ans++;
        else
            printf(1, "kthread_tid:%d , trnmnt_tree_tid:%d\n",kthread_tid, trnmnt_tid);
        if(trnmnt_tree_release(tree, kthread_id()) < 0)
            printf(1,"bad acquire\n");
        //printf(1,"thread:%d release\n",kthread_id());
    }
    kthread_exit();
}

void test_tree_tournament(){
    tree = trnmnt_tree_alloc(depth);
    int tid[num_threads];
    if(tree != 0){

        for(int i=0; i<num_threads;i++){
            STACK_CREATE(stack)
            memset(stack, 0, sizeof(*stack));
            tid[i] = kthread_create(tournament_cs, stack);
        }

        for(int i=0; i<num_threads;i++){
            kthread_join(tid[i]);
        }
        if(trnmnt_tree_dealloc(tree) != 0)
            printf(1,"bad dealloc\n");

    }
    else printf(1,"bad alloc\n");
}



volatile int have1 , have2;
int dontStart;

void threadStart_1(void){
    int result;
    while(dontStart);

    tree = trnmnt_tree_alloc(1);
    if(tree == 0){
        printf(1,"1 trnmnt_tree allocated unsuccessfully\n");
    }
    else ans++;
    have1=1;

    sleep(50);
    result = trnmnt_tree_acquire(tree, 0);
    if(result < 0){
        printf(1,"2 trnmnt_tree locked unsuccessfully\n");
    }else ans++;

    //sleep(500);

    result = trnmnt_tree_release(tree, 0);
    if(result < 0){
        printf(1,"3 trnmnt_tree unlocked unsuccessfully\n");
    }else ans++;
    have1=0;

    while(have2==1);
    result = trnmnt_tree_dealloc(tree);
    if(result == 0){
        printf(1,"4 trnmnt_tree deallocated successfully where it should not have been\n");
    }else ans++;

    kthread_exit();
}

void threadStart_2(void){
    int result;
    while(dontStart);

    while(have1==0);

    result = trnmnt_tree_acquire(tree, 1);
    if(result < 0){
        printf(1,"5 trnmnt_tree locked unsuccessfully\n");
    }else ans++;


    result = trnmnt_tree_release(tree, 1);
    if(result < 0){
        printf(1,"6 trnmnt_tree unlocked unsuccessfully\n");
    }else ans++;
    while(have1==1);

    result = trnmnt_tree_dealloc(tree);
    if(result == -1){
        printf(1,"7 trnmnt_tree deallocated unsuccessfully\n");
    }else ans++;
    have2=0;
    kthread_exit();
}


void vs_tree_Test(void){
    have1 = 0, have2 = 1;
    dontStart = 1;
    int pids[2];

    STACK_CREATE(threadStack_1)
    STACK_CREATE(threadStack_2)

    void (*threads_stacks[])(void) =
            {threadStack_1,
             threadStack_2};

    sleep(100);

        pids[0] = kthread_create(threadStart_1, threads_stacks[0]);
        pids[1] = kthread_create(threadStart_2, threads_stacks[1]);


    dontStart = 0;

    for(int i = 0;i < 2;i++){
        printf(1,"Attempting to join thread %d\n",i);

        int result = kthread_join(pids[i]);
        if(result == 0){
            ans++;
            printf(1,"Finished joing thread %d\n",i);
        }
        else if(result == -1){
            printf(1,"Error in joing thread %d\n",i);
        }
        else{
            printf(1,"Unknown result code from join\n");
        }
    }

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
    make_test(test_forking,20,"test_forking");
    make_test(sanity_kthread,1,"sanity_kthread");
    make_test(test_full_kthread,15,"sanity_kthread");
    make_test(create_extra_kthread,1,"create_extra_kthread");
    make_test(kthread_wrong_join,-1,"kthread_wrong_join");
    make_test(test_kthread_exit,1,"test_kthread_exit");
    make_test(test_kthread_join,60,"test_kthread_join");
    make_test(test_exit_process,numproc,"test_exit_process");


   // __________________SIMPLE MUTEX___________________
    make_test(mutex_alloc,1,"mutex_alloc");
    make_test(mutex_dealloc,1,"mutex_dealloc");
    make_test(mutex_dealloc_twice,1,"mutex_dealloc_twice");
    make_test(mutex_dealloc_non_alocated,1,"mutex_dealloc_non_alocated");
    make_test(mutex_bad_dealloc,-1,"mutex_bad_dealloc");
    make_test(sanity_mutex_lock,1,"sanity_mutex_lock");
    make_test(sanity_mutex_unlock,1,"sanity_mutex_unlock");
    make_test(sanity_mutex_double_lock,-1,"sanity_mutex_double_lock");
    for(int j = 0;j<5;j++) {

//
       for (loopnum = 1; loopnum < 10; loopnum++) {
           printf(1, "Test complexity : %d \n", loopnum);
           num_threads = 2; //two threads
           printf(1, "thread num: %d \n", num_threads);
           make_test(mutex_lock, num_threads*loopnum, "mutex_lock two threads");
           num_threads = 7; // half full threads
           printf(1, "thread num: %d \n", num_threads);
           make_test(mutex_lock, num_threads*loopnum, "mutex_lock half threads");
           num_threads = 15; // all threads
           printf(1, "thread num: %d \n", num_threads);
           make_test(mutex_lock, num_threads*loopnum, "mutex_lock all threads");
       }
    }

//    // __________________tournament_tree ______________________

        make_test(sanity_tree_alloc_dealloc,2,"sanity_tree_alloc_dealloc");
        make_test(vs_tree_Test,9,"vs_tree_Test");

    for(int j = 0;j<5;j++) {
        for (loopnum = 1; loopnum < 10; loopnum++) {
            num_threads = 2;
            depth = 2;
            printf(1, "Test complexity : %d \n", loopnum);
            printf(1, "thread num: %d \n", num_threads);
            make_test(sanity_tree_tournament, num_threads * 2 * loopnum, "sanity_tree_tournament");
            num_threads = 7;
            depth = 3;
            printf(1, "thread num: %d \n", num_threads);
            make_test(sanity_tree_tournament, num_threads * 2 * loopnum, "sanity_tree_tournament");
            num_threads = 15;
            depth = 4;
            printf(1, "thread num: %d \n", num_threads);
            make_test(sanity_tree_tournament, num_threads * 2 * loopnum, "sanity_tree_tournament");
        }
    }
    for(loopnum = 1;loopnum < 10; loopnum++) {
        num_threads = 2;
        depth = 2;
        printf(1,"Test complexity : %d \n", loopnum);
        make_test(test_tree_tournament, num_threads *loopnum, "test_tree_tournament");
        num_threads = 7;
        depth = 3;
        make_test(test_tree_tournament, num_threads *loopnum, "test_tree_tournament");
        num_threads = 15;
        depth = 4;
        make_test(test_tree_tournament, num_threads *loopnum, "test_tree_tournament");
    }
    // ___________________SUMMERY_______________________________
    printf(1,"num of success:%d num of failures: %d\n",success,fail );

    if(fail == 0)
        printf(1,"All tests passed!! Yay!\n");
    exit();
}