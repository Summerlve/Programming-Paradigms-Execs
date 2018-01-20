#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "thread_107.h"

void *test_func(void *args)
{
    char *debugName = *(char **)args; 
    
    Semaphore done = ((Semaphore *)args)[1];
    Semaphore canIGiveYouSomeMoney = ((Semaphore *)args)[2];
    int *total_money = ((int **)args)[3];

    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    printf("Thread: %s, total_money is: %d now.\n", debugName, *total_money);
    SemaphoreSignal(canIGiveYouSomeMoney);

    SemaphoreSignal(done);

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
        ThreadNew(str, test_func, 3, done, canIGiveYouSomeMoney, &totoal_money);
    }
    RunAllThreads();

    for (int i = 0; i < no; i++)
    {
        SemaphoreWait(done);
    }

    printf("total money is: %d\n", totoal_money);

    return EXIT_SUCCESS;
}
