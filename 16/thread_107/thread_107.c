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
    // variable-argument function, only accepts pointer(actually void *) as non-name arguments.

    // expand the threadInfos
    if (threadPool.logicalLength == threadPool.allocatedLength)
    {
        threadPool.threadInfos = realloc(threadPool.threadInfos,
                                        sizeof(ThreadInfo) * threadPool.allocatedLength * 2);
        threadPool.allocatedLength *= 2;
    }

    ThreadInfo t_info;
    t_info.debugName = malloc(strlen(debugName) + 1);
    strcpy(t_info.debugName, debugName);
    t_info.func = func;
    t_info.nArg = nArg;
 
    if (nArg == 0)
    {
        t_info.args = NULL;
        return ;
    }

    t_info.args = malloc(nArg * sizeof(void *));
    va_list ap;
    va_start(ap, nArg);
    for (int i = 0; i < nArg; i++)
    {
        void *v = va_arg(ap, void *);
        memcpy(&(((void **)t_info.args)[i]), &v, sizeof(void *));
    }
    va_end(ap);

    memcpy(&(threadPool.threadInfos[threadPool.logicalLength]), &t_info, sizeof(ThreadInfo));
    threadPool.logicalLength ++;
}

void ThreadSleep(int microSecs)
{
    struct timespec sleeper =
    { 
        .tv_sec = microSecs / 1000,
        .tv_nsec = (microSecs % 1000) * 10e6L
    };
    if (nanosleep(&sleeper, NULL) != 0) perror("sleep error");
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
        pthread_create(&tid, NULL, t_info.func, t_info.args);
        // pthread_join will block thread who called him, restart the thread when joined thread finished.
        // pthread_join(tid, NULL);
    }
}

Semaphore SemaphoreNew(const char *debugName, int initialValue)
{
    Semaphore sem = malloc(sizeof(struct SemaphoreImplementation));
    sem_init(&(sem->__semaphore__), 0, initialValue);
    sem->debugName = malloc(strlen(debugName) + 1);
    strcpy(sem->debugName, debugName);
    return sem;
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