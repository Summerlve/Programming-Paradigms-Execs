#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

bool traceFlag;
typedef struct ThreadInfo {
    
};
struct ThreadPool{
    int logicalLength;
    int allocatedLength;
    pthread_t **threadPtrs;
} threadPool;
 ;
void InitThreadPackage(bool traceFlag);
void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...);
void ThreadSleep(int microSecs);
const char *ThreadName(void);
void RunAllThreads(void);
struct SemaphoreImplementation {
    sem_t __semaphore__;
    const char *debugName;
};
typedef struct SemaphoreImplementation *Semaphore;
Semaphore SemaphoreNew(const char *debugName, int initialValue);
const char *SemaphoreName(Semaphore s);
void SemaphoreWait(Semaphore s);
void SemaphoreSignal(Semaphore s);
void SemaphoreFree(Semaphore s);
void AcquireLibraryLock(void);
void ReleaseLibraryLock(void);
#define PROTECT(code) {     \\
    AcquireLibraryLock();   \\
    code;                   \\
    ReleaseLibraryLock();   \\
}
void ListAllThreads(void);
void ListAllSemaphores(void);
