#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif

#include "lock.h"
#include <sched.h>
#include <time.h>

// init the lock
// called by the thread running the benchmark before any child threads are spawned
void rw_lock_init(ReaderWriterLock *rwlock)
{
    __atomic_store_n(&rwlock->reader, 0, __ATOMIC_RELAXED);
    __atomic_store_n(&rwlock->writer, 0, __ATOMIC_RELAXED);
}

/**
 * Try to acquire a read lock and spin until the lock is available.
 * To acquire the read lock, atomically increment the read counter
 * Then check if a writer is waiting,
 * If not then done
 * If a writer is waiting, decrement the read counter and wait for the writer to finish
 * then retry
 */
void read_lock(ReaderWriterLock *rwlock){

    while (true){
        // Check if any writer is present before getting a read lock.
        if (__atomic_load_n(&rwlock->writer, __ATOMIC_ACQUIRE) == 0){

            // Increment reader
            __atomic_fetch_add(&rwlock->reader, 1, __ATOMIC_RELAXED);

            // Check again if writer lock is present
            if (__atomic_load_n(&rwlock->writer, __ATOMIC_RELAXED) == 0)
            {
                return;
            }

            // writer present
            __atomic_fetch_sub(&rwlock->reader, 1, __ATOMIC_RELAXED);
        }

        // sched_yield();

        // for (volatile int i = 1; i < 100; i *= 10){
        //     struct timespec ts = {0, i}; // sec, ns
        //     nanosleep(&ts, NULL);
        // }
    }
}
/**
 * Release an acquired read lock
 * To release the read lock, atomically decrement the read counter
 */
void read_unlock(ReaderWriterLock *rwlock){
    __atomic_fetch_sub(&rwlock->reader, 1, __ATOMIC_RELEASE);
}

/**
 * Try to acquire a write lock and spin until the lock is available
 * Spin on the writer lock
 * Once writer lock is acquired, wait for the read counter to drop to 0
 */
void write_lock(ReaderWriterLock *rwlock){

    while (true){
      
        if (!__atomic_test_and_set(&rwlock->writer, __ATOMIC_ACQUIRE)){

            while (__atomic_load_n(&rwlock->reader, __ATOMIC_RELAXED) != 0){

                for (volatile int i = 1; i < 100; i *= 10){
                    struct timespec ts = {0, i}; // sec, ns
                    nanosleep(&ts, NULL);
                }
            }
            // Got writer lock
            return;
        }
    }
}
/**
 * Release an acquired write lock.
 * To release the write lock, atomically set the writer to 0
 */
void write_unlock(ReaderWriterLock *rwlock){
    __atomic_store_n(&rwlock->writer, 0, __ATOMIC_RELEASE);
}
