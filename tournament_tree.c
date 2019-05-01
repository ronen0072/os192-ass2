#include "tournament_tree.h"
#include "types.h"
#include "user.h"
#include "fcntl.h"


/*
struct trnmnt_tree {
    int depth;
    int * tree;
    int size;
};*/

trnmnt_tree* trnmnt_tree_alloc(int depth) {
    if(depth < 1)
        return 0;
    struct trnmnt_tree * tree = malloc(sizeof( trnmnt_tree));
    tree->size = ((2 << depth) - 1);
    tree->depth = depth;
    int i=0;
    tree->tree = malloc(tree->size * (sizeof(int)));
    // allocate mutexes for the tree
    for( i=0; i< tree->size ; i++){
        tree->tree[i] = kthread_mutex_alloc();
        if(tree->tree[i] < 0) //if  cant allocate mutexes (might be out of mutexes)
            goto bad;
    }

    return tree;


    bad:
    // deallocate all mutexes
    for(int j = 0 ; j<i ; j++){
        if(kthread_mutex_dealloc(tree->tree[j])< 0 )
            printf(1,"couldn't free mutexes");
    }
    free(tree->tree);
    free(tree);
    return 0;



}
int trnmnt_tree_dealloc(trnmnt_tree* tree){

    if(tree == 0 || tree->tree == 0){
        return -1;
    }
// deallocate all mutexes
    for(int j = 0 ; j<tree->size ; j++){
        if(kthread_mutex_dealloc(tree->tree[j]) < 0)
            return -1;
    }
    free(tree->tree);
    free(tree);
    return 0;
}

int trnmnt_tree_acquire_helper(trnmnt_tree* tree,int index) {

    if(index == 0){
        return kthread_mutex_lock(tree->tree[0]);
    }
    if(kthread_mutex_lock(tree->tree[index]) < 0)
        return -1;
    return trnmnt_tree_acquire_helper(tree, (index - 1)/2);

}

int trnmnt_tree_acquire(trnmnt_tree* tree,int ID){

    if(tree == 0 || tree->tree == 0 || ID < 0 || ID > 16){
        return -1;
    }
    int ans = trnmnt_tree_acquire_helper(tree,(tree->size+ID -1 )/2);
    if(ans >=0){
        tree->tid = ID;
        return ans;
    }
    else return -1;
}

int trnmnt_tree_release_helper(trnmnt_tree* tree,int index) {

    if(index == 0){
        return kthread_mutex_unlock(tree->tree[0]);
    }

    if (trnmnt_tree_release_helper(tree, (index - 1)/2) < 0)
        return  -1;
    return kthread_mutex_unlock(tree->tree[index]);
}

int trnmnt_tree_release(trnmnt_tree* tree,int ID) {
    tree->tid = -1;
    if(tree == 0 || tree->tree == 0 || ID < 0 || ID > 16){
        return -1;
    }

    return trnmnt_tree_release_helper(tree,(tree->size+ID -1 )/2);
}

int trnmnt_tree_tid(trnmnt_tree* tree){
    return  tree->tid;
}

