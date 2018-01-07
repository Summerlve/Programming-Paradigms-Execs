#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>

extern bool traceFlag;

typedef struct {
    const char *debugName;
    pthread_t tid;
    void *(*func)(void *);
    int nArg;
    void *args;
} ThreadInfo;

struct ThreadPool{
    int logicalLength;
    int allocatedLength;
    ThreadInfo *threadInfos;
} threadPool;

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
const char *SemaphoreName(Semaphore s); // get semaphore's debugName
void SemaphoreWait(Semaphore s); // semaphore -1
void SemaphoreSignal(Semaphore s); // semaphore +1
void SemaphoreFree(Semaphore s); // free semaphore
void AcquireLibraryLock(void);
void ReleaseLibraryLock(void);
#define PROTECT(code) {     \\
    AcquireLibraryLock();   \\
    code;                   \\
    ReleaseLibraryLock();   \\
}
void ListAllThreads(void);
void ListAllSemaphores(void);
