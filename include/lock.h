#ifndef _LOCK_H_
#define _LOCK_H_

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef __cplusplus
#define __restrict__
extern "C" {
#endif


  typedef struct ReaderWriterLock {
      // read lock
      volatile int reader; 
      // write lock
      volatile int writer; 
      // is writer waiting default no(0): 
      volatile int isWriterWaiting;
  } ReaderWriterLock;


// WARNING: DO NOT CHANGE THE READ/WRITE API. 
// You should not need to change anything below.
// Use the API as is and fill in the implementation in src/lock.c

  // You should not attempt to release a lock before acquiring it. 
  // It can cause undefined behavior. The benchmark doesn't test this.

  // init the lock and any associated resources
  void rw_lock_init(ReaderWriterLock *rwlock);

  // Blocking call to acquire a read lock. 
  void read_lock(ReaderWriterLock *rwlock);

  // unlock a reader thread that has acquired the lock
  void read_unlock(ReaderWriterLock *rwlock);

  // Blocking call to acquire a write lock
  void write_lock(ReaderWriterLock *rwlock);

  // free an acquired write lock
  void write_unlock(ReaderWriterLock *rwlock);

#ifdef __cplusplus
}
#endif



#endif
