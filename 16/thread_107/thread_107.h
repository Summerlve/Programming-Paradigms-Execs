#include <stdbool.h>
#include <stdio.h>

bool traceFlag;
void InitThreadPackage(bool traceFlag);
void ThreadNew(const char *debugName, void (*func)(), int nArg, ...);
void ThreadSleep(int microSecs);
const char *ThreadName(void);
void RunAllThreads(void);
struct SemaphoreImplementation {

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
