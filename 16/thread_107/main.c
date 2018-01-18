#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "thread_107.h"

void *test_func(void *args)
{
    ThreadInfo *this = ((ThreadInfo **)args)[0]; 
    printf("Thread: %s is working now.\n", this->debugName);
    
    Semaphore done = ((Semaphore **)args)[1];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[2];
    int *total_money = ((int **)args)[3];

    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    printf("Thread: %s, total_money is: %d now.\n", this->debugName, *total_money);

    SemaphoreSignal(done);

    return NULL; 
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    int no = 6; // the num of threads. 
    
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
