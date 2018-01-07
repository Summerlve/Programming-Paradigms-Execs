#include <stdio.h>
#include <stdbool.h>
#include "thread_107.h"

extern bool traceFlag;

void *testThreadPackage(void *args)
{
    Semaphore sem = malloc(sizeof(Semaphore));
    memcpy(&sem, args, sizeof(Semaphore));
    SemaphoreWait(sem);
    fputs("running testThreadPackage now\n", stdout);
    SemaphoreSignal(sem);
    free(sem);
}

int main(int argc, char **argv)
{
    // test Semaphore
    Semaphore waitForTest = SemaphoreNew("wait for test", 1);
    const char *waitForTestName = SemaphoreName(waitForTest);
    fputs(waitForTestName, stdout);
    fputs("\n", stdout);

    // test thread package
    ThreadNew("testThreadPackage", testThreadPackage, 1, waitForTest);
    RunAllThreads();

    ThreadSleep(2000);

    // while(true)
    // {
    //     SemaphoreWait(waitForTest);
    //     break;
    // }

    return 0;
}
