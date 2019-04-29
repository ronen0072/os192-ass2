

typedef struct trnmnt_tree trnmnt_tree;

struct trnmnt_tree {
    int depth;
    int * tree;
    int size;
    int tid;
};

trnmnt_tree* trnmnt_tree_alloc(int depth);
int trnmnt_tree_dealloc(trnmnt_tree* tree);
int trnmnt_tree_acquire(trnmnt_tree* tree,int ID);
int trnmnt_tree_release(trnmnt_tree* tree,int ID);
int trnmnt_tree_tid(trnmnt_tree* tree);
