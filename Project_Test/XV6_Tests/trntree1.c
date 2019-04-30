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

#define STACK_SIZE 500

void initiateMutexTest();

int main(int argc, char *argv[]){
    initiateMutexTest();
    exit();
}

void initiateMutexTest(){
    int result;

     trnmnt_tree* tree;
    
    tree = trnmnt_tree_alloc(1); 
    if(tree == 0){ 
        printf(1,"1 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"1 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"unkown return code from trnmnt_tree_dealloc\n"); 
    } 

    tree = trnmnt_tree_alloc(2); 
    if(tree == 0){ 
        printf(1,"2 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"2 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"unkown return code from trnmnt_tree_dealloc\n"); 
    } 

    tree = trnmnt_tree_alloc(3); 
    if(tree == 0){ 
        printf(1,"3 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"3 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"unkown return code from trnmnt_tree_dealloc\n"); 
    } 

    tree = trnmnt_tree_alloc(4); 
    if(tree == 0){ 
        printf(1,"4 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"4 trnmnt_tree deallocated unsuccessfully\n"); 
    }
    else{ 
        printf(1,"unkown return code from trnmnt_tree_dealloc\n"); 
    }  

    tree = trnmnt_tree_alloc(5); 
    if(tree == 0){ 
        printf(1,"5 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"5 trnmnt_tree deallocated unsuccessfully\n"); 
    }
    else{ 
        printf(1,"unkown return code from trnmnt_tree_dealloc\n"); 
    }  
}

