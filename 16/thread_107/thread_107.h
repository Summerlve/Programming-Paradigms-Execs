#ifndef THREAD_107_H
#define THREAD_107_H

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#include <stdbool.h>
#include <pthread.h>

struct SemaphoreImplementation {
    #ifdef __APPLE__
    dispatch_semaphore_t *__semaphore__;
    #else
    sem_t *__semaphore__;
    #endif
    const char *debugName;
};

typedef struct SemaphoreImplementation *Semaphore;

void InitThreadPackage(bool traceFlag);
void FreeThreadPackage();
void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...);
void ThreadSleep(int microSecs);
const char *ThreadName(void);
void RunAllThreads(void);
Semaphore SemaphoreNew(const char *debugName, int initialValue);
const char *SemaphoreName(Semaphore s); // get semaphore's debugName
void SemaphoreWait(Semaphore s); // semaphore -1
void SemaphoreSignal(Semaphore s); // semaphore +1
void SemaphoreFree(Semaphore s); // free semaphore

void AcquireLibraryLock(void);
void ReleaseLibraryLock(void);
#define PROTECT(code) {     \
    AcquireLibraryLock();   \
    code                   \
    ReleaseLibraryLock();   \
}
void ListAllThreads(void);
void ListAllSemaphores(void);

#endif