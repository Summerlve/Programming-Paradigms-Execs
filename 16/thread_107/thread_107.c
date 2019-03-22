#include "thread_107.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <semaphore.h>
#endif
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

typedef struct {
    const char *debugName;
    void *(*func)(void *);
    void *args;
    int nArg;
    pthread_t tid;
} ThreadInfo;

typedef struct {
    int logicalLength;
    int allocatedLength;
    ThreadInfo *threadInfos;
    Semaphore *semaphores;
    int semLogicalLength;
    int semAllocatedLength;
} ThreadPool;

static bool traceFlag = true; // default value of traceFlag is true.
static pthread_mutex_t mutexLock; // mutex lock for AcquireLibraryLock API
static pthread_mutex_t threadNewLock; // mutex lock to protect shared infomations in threadPool when calling the ThreadNew
static pthread_mutex_t semaphoreNewLock; // mutex lock to protect shared infomations in threadPool when calling the SemaphoreNew

// extern threadPool from thread_107.h.
static ThreadPool threadPool;

// for thread safety, you can call InitThreadPackage function only once in one thread(normally it will be the main thread)
void InitThreadPackage(bool flag)
{
    traceFlag = flag;
    threadPool.logicalLength = 0;
    threadPool.allocatedLength = 4;
    threadPool.threadInfos = malloc(sizeof(ThreadInfo) * threadPool.allocatedLength);
    threadPool.semLogicalLength = 0;
    threadPool.semAllocatedLength = 4;
    threadPool.semaphores = malloc(sizeof(Semaphore) * threadPool.semAllocatedLength);

    // init mutexLock
    int inited = pthread_mutex_init(&mutexLock, NULL);
    if (inited != 0) perror("pthread_mutex_init error");

    // init threadNewLock
    inited = pthread_mutex_init(&threadNewLock, NULL);
    if (inited != 0) perror("pthread_mutex_init error");

    // init semaphoreNewLock
    inited = pthread_mutex_init(&semaphoreNewLock, NULL);
    if (inited != 0) perror("pthread_mutex_init error");
}

// for thread safety, you can call FreeThreadPackage function only once in one thread(normally it will be the main thread)
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

    // free the whole semaphores
    free(threadPool.semaphores);

    // free mutexLock
    int destoryed = pthread_mutex_destroy(&mutexLock);
    if (destoryed != 0) perror("pthread_mutex_destory error");

    // free threadNewLock
    destoryed = pthread_mutex_destroy(&threadNewLock);
    if (destoryed != 0) perror("pthread_mutex_destory error");

    // free semaphoreNewLock
    destoryed = pthread_mutex_destroy(&semaphoreNewLock);
    if (destoryed != 0) perror("pthread_mutex_destory error");
}

void ThreadNew(const char *debugName, void *(*func)(void *), int nArg, ...)
{
    // variable-argument function, only accepts pointer(actually void *) as non-name arguments.

    // confirm that only one thread can call this function every single time
    int locked = pthread_mutex_lock(&threadNewLock);
    if (locked != 0) perror("pthread_mutex_lock error");

    // expand the threadInfos
    if (threadPool.logicalLength == threadPool.allocatedLength)
    {
        threadPool.threadInfos = realloc(threadPool.threadInfos,
                                        sizeof(ThreadInfo) * threadPool.allocatedLength * 2);
        if (threadPool.threadInfos == NULL) {
            printf("errorno is: %d\n", errno);
            perror("realloc error\n");
        }
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

    int unlocked = pthread_mutex_unlock(&threadNewLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");
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
    const char *debugName = NULL;

    // confirm that only one thread can call this function every single time
    int locked = pthread_mutex_lock(&threadNewLock);
    if (locked != 0) perror("pthread_mutex_lock error");

    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        ThreadInfo t_info = threadPool.threadInfos[i];
        if (t_info.tid == tid) debugName = t_info.debugName;
    }

    int unlocked = pthread_mutex_unlock(&threadNewLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");

    return debugName;
}

// for thread safety, you can call RunAllThreads function only once in one thread(normally it will be the main thread)
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
    // confirm that only one thread can call this function every single time
    int locked = pthread_mutex_lock(&semaphoreNewLock);
    if (locked != 0) perror("pthread_mutex_lock error");

    // expand the semphores. 
    if (threadPool.semLogicalLength == threadPool.semAllocatedLength)
    {
        threadPool.semaphores = realloc(threadPool.semaphores,
                                        sizeof(Semaphore) * threadPool.semAllocatedLength * 2);
        if (threadPool.semaphores == NULL) {
            printf("errno is: %d\n", errno);
            perror("realloc error\n");
        }
        threadPool.semAllocatedLength *= 2;
    }

    Semaphore sem = malloc(sizeof(struct SemaphoreImplementation));
    // sem_init is not available in macOS, use sem_open instead it.
    #ifdef __APPLE__
    dispatch_semaphore_t *__semaphore__ = malloc(sizeof(dispatch_semaphore_t));
    *__semaphore__ = dispatch_semaphore_create(initialValue);
    if (*__semaphore__ == NULL) perror("dispatch_semaphore_create error");
    #else
    sem_t *__semaphore__ = sem_open(debugName, O_CREAT, 0600, initialValue);
    if (__semaphore__ == SEM_FAILED) perror("sem_open error");
    #endif
    sem->__semaphore__ = __semaphore__;
    sem->debugName = malloc(strlen(debugName) + 1);
    strcpy(sem->debugName, debugName);

    memcpy(&(threadPool.semaphores[threadPool.semLogicalLength]), &sem, sizeof(Semaphore));
    threadPool.semLogicalLength ++;

    int unlocked = pthread_mutex_unlock(&semaphoreNewLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");

    return sem;
}

const char *SemaphoreName(Semaphore s)
{
    return s->debugName;
}

void SemaphoreWait(Semaphore s)
{
    #ifdef __APPLE__
    int result = dispatch_semaphore_wait(*(s->__semaphore__), DISPATCH_TIME_FOREVER);
    if (result != 0) perror("dispatch_semaphore_wait error");
    #else
    int result = sem_wait(s->__semaphore__);
    if (result != 0) perror("sem_wait error");
    #endif
}

void SemaphoreSignal(Semaphore s)
{
    #ifdef __APPLE__
    dispatch_semaphore_signal(*(s->__semaphore__));
    #else
    int result = sem_post(s->__semaphore__);
    if (result != 0) perror("sem_post error");
    #endif
}

void SemaphoreFree(Semaphore s)
{
    #ifdef __APPLE__
    free(s->__semaphore__);
    #else
    int result = sem_close(s->__semaphore__);
    if (result != 0) perror("sem_close error");
    #endif
    free(s->debugName);
}

void AcquireLibraryLock(void)
{
    int locked = pthread_mutex_lock(&mutexLock);
    if (locked != 0) perror("pthread_mutex_lock error");
}

void ReleaseLibraryLock(void)
{
    int unlocked = pthread_mutex_unlock(&mutexLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");
}

void ListAllThreads(void)
{
    // confirm that only one thread can call this function every single time
    int locked = pthread_mutex_lock(&threadNewLock);
    if (locked != 0) perror("pthread_mutex_lock error");

    for (int i = 0; i < threadPool.logicalLength; i++)
    {
        printf("Thread's debugName is: %s\n", threadPool.threadInfos[i].debugName);
    }

    int unlocked = pthread_mutex_unlock(&threadNewLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");
}

void ListAllSemaphores(void)
{
    // confirm that only one thread can call this function every single time
    int locked = pthread_mutex_lock(&semaphoreNewLock);
    if (locked != 0) perror("pthread_mutex_lock error");

    for (int i = 0; i < threadPool.semLogicalLength; i++)
    {
        printf("Semaphore's debugName is %s\n", threadPool.semaphores[i]->debugName);
    }

    int unlocked = pthread_mutex_unlock(&semaphoreNewLock);
    if (unlocked != 0) perror("pthread_mutex_unlock error");
}