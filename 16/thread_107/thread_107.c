#include "thread_107.h"

traceFlag = true;

void InitThreadPackage(bool traceFlag)
{
    traceFlag = traceFlag;    
    threadPool.logicalLength = 0;
    threadPool.allocatedLength = 4; 
    threadPool.threadPtrs = malloc(sizeof(pthread_t *) * threadPool.allocatedLength);
}

void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...)
{
    pthread_t p;
    pthread_create(&p, NULL, func, )
}

void ThreadSleep(int microSecs)
{

}

const char *ThreadName(void)
{

}

void RunAllThreads(void)
{

}

Semaphore SemaphoreNew(const char *debugName, int initialValue)
{
    struct SemaphoreImplementation sem;
    sem_init(&(sem.__semaphore__), 0, initialValue);
    sem.debugName = debugName;
    return &sem;
}

const char *SemaphoreName(Semaphore s)
{
    return s->debugName;
}

void SemaphoreWait(Semaphore s)
{
    sem_wait(&(s->__semaphore__));
}

void SemaphoreSignal(Semaphore s)
{
    sem_post(&(s->__semaphore__));
}

void SemaphoreFree(Semaphore s)
{
    sem_destroy(&(s->__semaphore__));
}

