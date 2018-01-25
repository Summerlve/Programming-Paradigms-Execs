#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "thread_107.h"

void *test_func(void *args)
{

    char *debugName = *(char **)args;
    printf("Thread: %s is start running\n", debugName);

    Semaphore *done = ((Semaphore **)args)[1];
    Semaphore *canIGiveYouSomeMoney = ((Semaphore **)args)[2];
    int *total_money = ((int **)args)[3];

    PROTECT(
        *total_money += 1;
        printf("Thread: %s, total_money is: %d now.\n", debugName, *total_money);
    )

    SemaphoreSignal(*done);

    return NULL;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    int no = 50; // the num of threads.

    Semaphore done = SemaphoreNew("done", 0);
    Semaphore canIGiveYouSomeMoney = SemaphoreNew("canIGiveYouSomeMoney", 1);

    int totoal_money = 0;

    for (int i = 0; i < no; i++)
    {
        // itos
        char str[12];
        sprintf(str, "test_%d", i);
        ThreadNew(str, test_func, 3, &done, &canIGiveYouSomeMoney, &totoal_money);
    }

    RunAllThreads();

    for (int i = 0; i < no; i++)
    {
        SemaphoreWait(done);
    }

    printf("All tasks finished, total money is: %d\n", totoal_money);

    // test semaphore.
    Semaphore a = SemaphoreNew("a", 0);
    Semaphore b = SemaphoreNew("b", 0);
    Semaphore c = SemaphoreNew("c", 0);
    Semaphore d = SemaphoreNew("d", 0);

    ListAllSemaphores();
    ListAllThreads();
    
    FreeThreadPackage();
    
    return EXIT_SUCCESS;
}
