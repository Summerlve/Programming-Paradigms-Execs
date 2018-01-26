#include <pthread.h>
#include <semaphore.h>

struct SemaphoreImplementation {
    sem_t *__semaphore__;
    const char *debugName;
};

typedef struct SemaphoreImplementation *Semaphore;

typedef struct {
    const char *debugName;
    void *(*func)(void *);
    void *args;
    int nArg;
    pthread_t tid;
} ThreadInfo;

struct ThreadPool{
    int logicalLength;
    int allocatedLength;
    ThreadInfo *threadInfos;
    Semaphore *semaphores;
    int semLogicalLength;
    int semAllocatedLength;
} threadPool;

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