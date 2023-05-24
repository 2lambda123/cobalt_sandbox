// RUN: %clangxx_tsan -O1 %s -o %t && %deflake %run %t | FileCheck %s
#include "test.h"

int Global;
pthread_mutex_t mtx1;
pthread_mutex_t mtx2;
pthread_rwlock_t mtx3;

void *Thread1(void *x) {
  barrier_wait(&barrier);
  pthread_mutex_lock(&mtx1);
  Global++;
  pthread_mutex_unlock(&mtx1);
  return NULL;
}

void *Thread2(void *x) {
  pthread_mutex_lock(&mtx1);
  pthread_mutex_unlock(&mtx1);
  pthread_mutex_lock(&mtx2);
  pthread_rwlock_rdlock(&mtx3);
  Global--;
  pthread_mutex_unlock(&mtx2);
  pthread_rwlock_unlock(&mtx3);
  barrier_wait(&barrier);
  return NULL;
}

int main() {
  barrier_init(&barrier, 2);
  // CHECK: WARNING: ThreadSanitizer: data race
  // CHECK:   Write of size 4 at {{.*}} by thread T1
  // CHECK:                          (mutexes: write [[M1:M[0-9]+]]):
  // CHECK:   Previous write of size 4 at {{.*}} by thread T2
  // CHECK:               (mutexes: write [[M2:M[0-9]+]], read [[M3:M[0-9]+]]):
  // CHECK:   Mutex [[M1]] (0x{{.*}}) created at:
  // CHECK:     #1 main {{.*}}mutexset6.cpp:[[@LINE+5]]
  // CHECK:   Mutex [[M2]] (0x{{.*}}) created at:
  // CHECK:     #1 main {{.*}}mutexset6.cpp:[[@LINE+4]]
  // CHECK:   Mutex [[M3]] (0x{{.*}}) created at:
  // CHECK:     #1 main {{.*}}mutexset6.cpp:[[@LINE+3]]
  pthread_mutex_init(&mtx1, 0);
  pthread_mutex_init(&mtx2, 0);
  pthread_rwlock_init(&mtx3, 0);
  pthread_t t[2];
  pthread_create(&t[0], NULL, Thread1, NULL);
  pthread_create(&t[1], NULL, Thread2, NULL);
  pthread_join(t[0], NULL);
  pthread_join(t[1], NULL);
  pthread_mutex_destroy(&mtx1);
  pthread_mutex_destroy(&mtx2);
  pthread_rwlock_destroy(&mtx3);
}
