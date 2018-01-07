#include "thread_107.h"

// default value of traceFlag is true.
bool traceFlag = true;

void InitThreadPackage(bool flag)
{
    extern bool traceFlag;
    traceFlag = flag;
    threadPool.logicalLength = 0;
    threadPool.allocatedLength = 4;
    threadPool.threadInfos = malloc(sizeof(ThreadInfo) * threadPool.allocatedLength);
}

void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...)
{
    // variable-argument function, only accepts pointer as non-name argument.
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

    threadPool.threadInfos = realloc(threadPool.threadInfos, sizeof(ThreadInfo) * (threadPool.logicalLength + 1));
    threadPool.logicalLength ++;

    t_info.args = malloc(nArg * sizeof(void *));
    va_list ap;
    va_start(ap, nArg);
    for (int i = 0; i < nArg; i++)
    {
        void *v = va_arg(ap, void *);
        memcpy(&((void **)t_info.args)[i], &v, sizeof(void *));
    }
    va_end(ap);
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
    pthread_t tid = pthread_self();

    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        ThreadInfo t_info = threadPool.threadInfos[i];
        if (t_info.tid = tid) return t_info.debugName;
    }

    return NULL;
}

void RunAllThreads(void)
{
    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        pthread_t tid;
        ThreadInfo t_info = threadPool.threadInfos[i];
        t_info.tid = tid;
        pthread_create(tid, NULL, t_info.func, t_info.args);
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
