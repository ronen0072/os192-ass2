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

    result = trnmnt_tree_acquire(tree, 0); 
    if(result < 0){  
        printf(1,"1 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 0); 
    if(result < 0){ 
        printf(1,"1 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 1); 
    if(result < 0){  
        printf(1,"2 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 1); 
    if(result < 0){ 
        printf(1,"2 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"1 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"1 unkown return code from trnmnt_tree_dealloc\n"); 
    } 




    tree = trnmnt_tree_alloc(2); 
    if(tree == 0){ 
        printf(1,"2 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 0); 
    if(result < 0){  
        printf(1,"3 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 0); 
    if(result < 0){ 
        printf(1,"3 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 1); 
    if(result < 0){  
        printf(1,"4 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 1); 
    if(result < 0){ 
        printf(1,"4 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 2); 
    if(result < 0){  
        printf(1,"5 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 2); 
    if(result < 0){ 
        printf(1,"5 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 3); 
    if(result < 0){  
        printf(1,"6 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 3); 
    if(result < 0){ 
        printf(1,"6 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"12 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"2 unkown return code from trnmnt_tree_dealloc\n"); 
    } 









    tree = trnmnt_tree_alloc(3); 
    if(tree == 0){ 
        printf(1,"3 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 0); 
    if(result < 0){  
        printf(1,"7 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 0); 
    if(result < 0){ 
        printf(1,"7 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 1); 
    if(result < 0){  
        printf(1,"8 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 1); 
    if(result < 0){ 
        printf(1,"8 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 2); 
    if(result < 0){  
        printf(1,"9 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 2); 
    if(result < 0){ 
        printf(1,"9 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 3); 
    if(result < 0){  
        printf(1,"10 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 3); 
    if(result < 0){ 
        printf(1,"10 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 4); 
    if(result < 0){  
        printf(1,"11 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 4); 
    if(result < 0){ 
        printf(1,"11 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 5); 
    if(result < 0){  
        printf(1,"12 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 5); 
    if(result < 0){ 
        printf(1,"12 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 6); 
    if(result < 0){  
        printf(1,"13 trnmnt_tree locked unsuccessfully\n"); 
    } 
    
    result = trnmnt_tree_release(tree, 6); 
    if(result < 0){ 
        printf(1,"13 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 7); 
    if(result < 0){  
        printf(1,"14 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 7); 
    if(result < 0){ 
        printf(1,"14 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"3 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"3 unkown return code from trnmnt_tree_dealloc\n"); 
    } 













    tree = trnmnt_tree_alloc(4); 
    if(tree == 0){ 
        printf(1,"4 trnmnt_tree allocated unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 0); 
    if(result < 0){  
        printf(1,"15 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 0); 
    if(result < 0){ 
        printf(1,"15 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 1); 
    if(result < 0){  
        printf(1,"16 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 1); 
    if(result < 0){ 
        printf(1,"16 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 2); 
    if(result < 0){  
        printf(1,"17 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 2); 
    if(result < 0){ 
        printf(1,"17 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 3); 
    if(result < 0){  
        printf(1,"18 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 3); 
    if(result < 0){ 
        printf(1,"18 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 4); 
    if(result < 0){  
        printf(1,"19 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 4); 
    if(result < 0){ 
        printf(1,"19 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 5); 
    if(result < 0){  
        printf(1,"20 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 5); 
    if(result < 0){ 
        printf(1,"20 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 6); 
    if(result < 0){  
        printf(1,"21 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 6); 
    if(result < 0){ 
        printf(1,"21 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 7); 
    if(result < 0){  
        printf(1,"22 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 7); 
    if(result < 0){ 
        printf(1,"22 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 8); 
    if(result < 0){  
        printf(1,"23 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 8); 
    if(result < 0){ 
        printf(1,"23 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 9); 
    if(result < 0){  
        printf(1,"24 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 9); 
    if(result < 0){ 
        printf(1,"24 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 10); 
    if(result < 0){  
        printf(1,"25 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 10); 
    if(result < 0){ 
        printf(1,"25 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 11); 
    if(result < 0){  
        printf(1,"26 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 11); 
    if(result < 0){ 
        printf(1,"26 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 12); 
    if(result < 0){  
        printf(1,"27 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 12); 
    if(result < 0){ 
        printf(1,"27 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 13); 
    if(result < 0){  
        printf(1,"28 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 13); 
    if(result < 0){ 
        printf(1,"28 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 14); 
    if(result < 0){  
        printf(1,"29 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 14); 
    if(result < 0){ 
        printf(1,"29 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_acquire(tree, 15); 
    if(result < 0){  
        printf(1,"30 trnmnt_tree locked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_release(tree, 15); 
    if(result < 0){ 
        printf(1,"30 trnmnt_tree unlocked unsuccessfully\n"); 
    } 

    result = trnmnt_tree_dealloc(tree); 
    if(result == 0){}
    else if(result == -1){ 
        printf(1,"4 trnmnt_tree deallocated unsuccessfully\n"); 
    } 
    else{ 
        printf(1,"4 unkown return code from trnmnt_tree_dealloc\n"); 
    } 
    
}

