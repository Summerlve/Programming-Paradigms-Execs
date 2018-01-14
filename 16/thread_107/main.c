#include <stdio.h>
#include <stdbool.h>
#include "thread_107.h"

void *test_1(void *args)
{
    Semaphore done = ((Semaphore **)args)[0];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[1];
    int *total_money = ((int **)args)[2];
    
    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    fputs("test_1\n", stdout);
    SemaphoreSignal(done);

    return NULL;
}

void *test_2(void *args)
{
    Semaphore done = ((Semaphore **)args)[0];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[1];
    int *total_money = ((int **)args)[2];
    
    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    fputs("test_2\n", stdout);
    SemaphoreSignal(done);
    
    return NULL;
}

void *test_3(void *args)
{
    Semaphore done = ((Semaphore **)args)[0];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[1];
    int *total_money = ((int **)args)[2];
    
    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    fputs("test_3\n", stdout);
    SemaphoreSignal(done);
    
    return NULL;
}

void *test_4(void *args)
{
    Semaphore done = ((Semaphore **)args)[0];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[1];
    int *total_money = ((int **)args)[2];
    
    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    fputs("test_4\n", stdout);
    SemaphoreSignal(done);

    return NULL;
}

void *test_5(void *args)
{
    Semaphore done = ((Semaphore **)args)[0];
    Semaphore canIGiveYouSomeMoney = ((Semaphore **)args)[1];
    int *total_money = ((int **)args)[2];
    
    SemaphoreWait(canIGiveYouSomeMoney);
    *total_money += 5;
    SemaphoreSignal(canIGiveYouSomeMoney);

    fputs("test_5\n", stdout);
    SemaphoreSignal(done);
    
    return NULL;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    Semaphore done = SemaphoreNew("done", 0); 
    int doneValue = 0;
    Semaphore canIGiveYouSomeMoney = SemaphoreNew("canIGiveYouSomeMoney", 1);

    int *totoal_money = malloc(sizeof(int));
    *totoal_money = 0;
    
    ThreadNew("test_1", test_1, 3, done, canIGiveYouSomeMoney, totoal_money);
    ThreadNew("test_2", test_2, 3, done, canIGiveYouSomeMoney, totoal_money);
    ThreadNew("test_3", test_3, 3, done, canIGiveYouSomeMoney, totoal_money);
    ThreadNew("test_4", test_4, 3, done, canIGiveYouSomeMoney, totoal_money);
    ThreadNew("test_5", test_5, 3, done, canIGiveYouSomeMoney, totoal_money);
    RunAllThreads();

    for (int i = 0; i < 5; i++)
    {
        SemaphoreWait(done);
    }

    printf("123123123\n");
    printf("total money is: %d\n", *totoal_money);

    return 0;
}
