#include "../../param.h"
#include "../../types.h"
#include "../../stat.h"
#include "../../user.h"
#include "../../fs.h"
#include "../../fcntl.h"
#include "../../syscall.h"
#include "../../traps.h"
#include "../../memlayout.h"

#define NUM_PROC 61
#define STACK_SIZE 500

int execute(char * command, char** args);
void createProcess();

int main(int argc, char *argv[]){
    
    for(int i = 0;i < 10;i++){
        createProcess();
        sleep(1000);
    }

    exit();
}

void createProcess(){
    char * command;
    char *args[4];

    for(int i = 0;i < NUM_PROC;i++){
        printf(1,"Creating process %d out of %d\n",i+1, NUM_PROC);
        args[0] = "/cThreW16T";
        args[1] = 0;
        command = "/cThreW16T";
        execute(command,args);
    }

    for(int i = 0;i < NUM_PROC;i++){
        printf(1,"Waiting for process %d out of %d\n",i+1, NUM_PROC);
        wait();
    }
}

int execute(char * command, char** args){
    int pid;

    if((pid = fork()) == 0){
        exec(command, args);
        printf(1, "exec %s failed\n", command);
        exit();
    }
    else if(pid > 0){
        return pid;
    }
    else{
        printf(1,"fork failed\n");
        exit();
    }
}