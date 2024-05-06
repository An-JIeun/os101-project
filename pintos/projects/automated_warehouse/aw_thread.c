#include "projects/automated_warehouse/aw_thread.h"

//
// You need to read carefully thread/synch.h and thread/synch.c
// 
// In the code, a fucntion named "sema_down" implements blocking thread and 
// makes list of blocking thread
// 
// And a function named "sema_up" implements unblocing thread using blocking list
//
// You must implement blocking list using "blocking_threads" in this code.
// Then you can also implement unblocking thread.
//


struct list blocked_threads;

/**
 * A function unblocking all blocked threads in "blocked_threads" 
 * It must be called by robot threads
 */
void block_thread(){
    // You must implement this

    // Code below is example
    enum intr_level old_level;
    old_level = intr_disable ();
    thread_block ();
    intr_set_level (old_level);
}

/**
 * A function unblocking all blocked threads in "blocked_threads" 
 * It must be called by central control thread
 */

void unblock_threads(){
    // Disable interrupts to ensure atomicity
    enum intr_level old_level;
    old_level = intr_disable ();
    
    // Acquire lock to access the blocked threads list
    lock_acquire(&blocked_threads);
    
    // Iterate through the blocked threads list and unblock each thread
    while (!list_empty(&blocked_threads)) {
        struct list_elem *e = list_pop_front(&blocked_threads);
        struct thread *t = list_entry(e, struct thread, elem);
        thread_unblock(t);
    }
    
    // Release lock
    lock_release(&blocked_threads);
    
    // Enable interrupts
    intr_set_level (old_level);
}