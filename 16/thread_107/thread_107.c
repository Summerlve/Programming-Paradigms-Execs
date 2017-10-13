#include "thread_107.h"

traceFlag = true;

void InitThreadPackage(bool traceFlag)
{
    traceFlag = traceFlag;
    threadPool.logicalLength = 0;
    threadPool.allocatedLength = 4;
    threadPool.threadInfos = malloc(sizeof(pthread_t *) * threadPool.allocatedLength);
}

void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...)
{
    // expand the threadInfos
    if (threadPool.logicalLength == threadPool.allocatedLength)
    {
        threadPool.threadInfos = realloc(threadPool.threadInfos,
                                        sizeof(ThreadInfo) * threadPool.allocatedLength * 2);
        threadPool.allocatedLength *= 2;
    }

    ThreadInfo t_info;
    t_info.debugName = debugName;
    t_info.func = func;
    t_info.nArg = nArg;

    threadPool.threadInfos
    threadPool.logicalLength ++;
}

void ThreadSleep(int microSecs)
{
    struct timespec sleeper;
    sleeper.tv_sec = 0;
    sleeper.tv_nsec = microSecs * 10e6L; // micro secs 2 nano secs
    nanosleep(&sleeper, NULL);
}

// macOS can not get name of single thread.
const char *ThreadName(void)
{
    return "";
}

void RunAllThreads(void)
{
    for(int i = 0; i < threadPool.logicalLength; i++)
    {
        pthread_t cur = *(pthread_t **)(&threadPool.threadInfos[i]);
        pthread_create(cur, )

    }
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
