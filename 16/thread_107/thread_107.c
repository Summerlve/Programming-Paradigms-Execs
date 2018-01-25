#include <fcntl.h>
#include <stdlib.h>
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
pthread_mutex_t mutexLock;

void InitThreadPackage(bool flag)
{
    extern bool traceFlag;
    extern struct ThreadPool threadPool;
    
    traceFlag = flag;
    threadPool.logicalLength = 0;
    threadPool.allocatedLength = 4;
    threadPool.threadInfos = malloc(sizeof(ThreadInfo) * threadPool.allocatedLength);
    threadPool.semLogicalLength = 0;
    threadPool.semAllocatedLength = 4;
    threadPool.semaphores = malloc(sizeof(Semaphore) * threadPool.allocatedLength);

    // init mutexLock
    int inited = pthread_mutex_init(&mutexLock, NULL);
    if (inited != 0) perror("pthread_mutex_init error");
}

void FreeThreadPackage()
{
    // free ThreadInfo's debugName and args.
    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        ThreadInfo *t_info = &(threadPool.threadInfos[i]);
        free((void *)t_info->debugName);
        free(t_info->args);
    }
    
    // free the whole threadInfos
    free(threadPool.threadInfos);

    // free all Semaphores.
    for (int i = 0; i < threadPool.semLogicalLength; i++)
    {
        Semaphore semaphore = threadPool.semaphores[i];
        SemaphoreFree(semaphore);
    }

    // free mutexLock
    int destoryed = pthread_mutex_destroy(&mutexLock);
    if (destoryed != 0) perror("pthread_mutex_destory error");
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
    t_info.args = malloc(sizeof(char *));

    if (nArg != 0)
    {
        t_info.args = malloc((nArg + 1) * sizeof(void *));

        va_list ap;
        va_start(ap, nArg);

        for (int i = 0; i < nArg; i++)
        {
            void *v = va_arg(ap, void *);
            memcpy(&(((void **)t_info.args)[i + 1]), &v, sizeof(void *));
        }
        va_end(ap);
    }

    // copy debugName to t_info.args[0].
    memcpy(t_info.args, &t_info.debugName, sizeof(char *));

    // copy mem from loacl var to heap mem for data persistence.
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
        if (t_info.tid == tid) return t_info.debugName;
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
     // expand the semphores. 
    if (threadPool.semLogicalLength == threadPool.semAllocatedLength)
    {
        threadPool.semaphores = realloc(threadPool.semaphores,
                                        sizeof(Semaphore) * threadPool.semAllocatedLength * 2);
        threadPool.semAllocatedLength *= 2;
    }

    Semaphore sem = malloc(sizeof(struct SemaphoreImplementation));
    // sem_init is not available in macOS, use sem_open instead it.
    sem_t *__semaphore__ = sem_open(debugName, O_CREAT, 0644, initialValue);
    if (__semaphore__ == SEM_FAILED) perror("sem_open error");
    sem->__semaphore__ = __semaphore__;
    sem->debugName = malloc(strlen(debugName) + 1);
    strcpy(sem->debugName, debugName);

    memcpy(&(threadPool.semaphores[threadPool.semLogicalLength]), &sem, sizeof(Semaphore));
    threadPool.semLogicalLength ++;

    return sem;
}

const char *SemaphoreName(Semaphore s)
{
    return s->debugName;
}

void SemaphoreWait(Semaphore s)
{
    int result = sem_wait(s->__semaphore__);
    if (result != 0) perror("sem_wait error");
}

void SemaphoreSignal(Semaphore s)
{
    int result = sem_post(s->__semaphore__);
    if (result != 0) perror("sem_post error");
}

void SemaphoreFree(Semaphore s)
{
    int result = sem_close(s->__semaphore__);
    if (result != 0) perror("sem_close error");
    free(s->debugName);
}

void AcquireLibraryLock(void)
{
    extern pthread_mutex_t mutexLock;
    int locked = pthread_mutex_lock(&mutexLock);
    if (locked != 0) perror("pthread_mutex_lock error");
}

void ReleaseLibraryLock(void)
{
    extern pthread_mutex_t mutexLock;
    int unlocked = pthread_mutex_unlock(&mutexLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");
}

void ListAllThreads(void)
{
    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        printf("Thread's debugName is: %s\n", threadPool.threadInfos[i].debugName);
    }
}

void ListAllSemaphores(void)
{
    for (int i = 0; i < threadPool.semLogicalLength; i++)
    {
        printf("Semaphore's debugName is %s\n", threadPool.semaphores[i]->debugName);
    }
}