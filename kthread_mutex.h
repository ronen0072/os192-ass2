

// Long-term locks for threads
struct kthread_mutex {
    uint locked;       // Is the lock held?
    //struct spinlock lk; // spinlock protecting this sleep lock
    int allocated;
    // For debugging:
    char *name;        // Name of lock.
    int tid;           // thread holding lock
};
