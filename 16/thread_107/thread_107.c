#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
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

    t_info.args = malloc((nArg + 1) * sizeof(void *));
 
    va_list ap;
    va_start(ap, nArg);
    
    for (int i = 0; i < nArg; i++)
    {
        void *v = va_arg(ap, void *);
        memcpy(&(((void **)t_info.args)[i+1]), &v, sizeof(void *));
    }
    va_end(ap);

     // copy 'this' to t_info.args[0].
    ThreadInfo *this = &(threadPool.threadInfos[threadPool.logicalLength]);
    memcpy(t_info.args, &this, sizeof(ThreadInfo *));

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
        ThreadInfo *t_info = &(threadPool.threadInfos[i]);
        t_info->tid = tid;
        if (pthread_create(&(t_info->tid), NULL, t_info->func, t_info->args) != 0) perror("pthread_create error");
    }
}

Semaphore SemaphoreNew(const char *debugName, int initialValue)
{
    Semaphore sem = malloc(sizeof(struct SemaphoreImplementation));
    // sem_init is not available in macOS, use sem_open instead it.
    sem_t *__semaphore__ = sem_open(debugName, O_CREAT, 0644, initialValue);
    if (__semaphore__ == SEM_FAILED) perror("sem_open error");
    sem->__semaphore__ = __semaphore__;
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
    if (sem_wait(s->__semaphore__) != 0) perror("sem_wait error");
}

void SemaphoreSignal(Semaphore s)
{
    if (sem_post(s->__semaphore__) != 0) perror("sem_post error");
}

void SemaphoreFree(Semaphore s)
{
    if(sem_close(s->__semaphore__) != 0) perror("sem_close error");
}