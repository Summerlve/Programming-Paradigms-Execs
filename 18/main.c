#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../16/thread_107/thread_107.h"

struct{
    bool passed;
    Semaphore requested;
    Semaphore finished;
    Semaphore lock; // binary lock for the manager
    Semaphore approvedAll; // waiting for all customer purchased done.
} inspection;

struct {
    int number;
    Semaphore requested;
    Semaphore customers[10];
    Semaphore lock; // binary lock for the number
} queue;

int RandomInteger(int min, int max)
{
    int value = rand() % (max + 1 - min) + min;
    return value;
}

void *Clerk(void *args)
{
    char *debugName = *(char **)args;
    bool passed = false;
    Semaphore clerksDone = ((Semaphore *)args)[1];

    while (passed != true)
    {
        // MakeCones() here
        SemaphoreWait(inspection.lock);
        SemaphoreSignal(inspection.requested);
        SemaphoreWait(inspection.finished);
        passed = inspection.passed;
        SemaphoreSignal(inspection.lock);
    }

    printf("%s thread, clerk done\n", debugName);
    SemaphoreSignal(clerksDone);

    return NULL;
}

void *Customer(void *args)
{
    char *debugName = *(char **)args;
    int numCones = *(((int **)args)[1]);
    printf("%s thread, numCones is %d\n", debugName, numCones);
    Semaphore clerksDone = SemaphoreNew("clerk", 0);

    for (int i = 0; i < numCones; i++)
    {
        // itos
        char str[32];
        sprintf(str, "Clerk[%d] - %s", i, debugName);
        ThreadNew(str, Clerk, 1, clerksDone);
    }

    for (int i = 0; i < numCones; i++)
    {
        SemaphoreWait(clerksDone);
    }

    // WalkToCashier() here
    SemaphoreWait(queue.lock);
    int place = queue.number ++;
    printf("%s thread, it's queue.number is %d\n", debugName, place);
    SemaphoreSignal(queue.lock);
    SemaphoreSignal(queue.requested);
    SemaphoreWait(queue.customers[place]);

    // free the numCones
    free(((int **)args)[1]);
    printf("%s thread, done\n", debugName);
    SemaphoreSignal(inspection.approvedAll);

    return NULL;
}

void *Cashier(void *args)
{
    char *debugName = *(char **)args;
    for (int i = 0; i < 10; i++)
    {
        SemaphoreWait(queue.requested);
        // CheckOutMoney() here
        printf("%s thread, cashier checks queue.number[%d]\n", debugName, i);
        SemaphoreSignal(queue.customers[i]);
    }
    return NULL;
}

void *Manager(void *args)
{
    char *debugName = *(char **)args;
    int totalCones = *(((int **)args)[1]);
    int approved = 0;
    int checked = 0;

    while (approved < totalCones)
    {
        SemaphoreWait(inspection.requested);
        checked ++;
        inspection.passed = RandomInteger(0, 1);
        if (inspection.passed) 
        {
            approved ++;
            printf("%s thread, manager have checked %d cones\n", debugName, approved);
        }
        SemaphoreSignal(inspection.finished);
    }

    printf("%s thread, manager checked all cones done\n", debugName);
    return NULL;
}

void SetSomeSemaphore()
{
    printf("Set some semaphores\n");
    // init inspection.
    inspection.passed = false;
    inspection.requested = SemaphoreNew("inspection.requested", 0);
    inspection.finished = SemaphoreNew("inspection.finished", 0);
    inspection.lock = SemaphoreNew("inspection.lock", 1);
    inspection.approvedAll = SemaphoreNew("inspection.approvedAll", 0);

    // init queue.
    queue.number = 0;
    queue.requested = SemaphoreNew("queue.requested", 0);
    for (int i = 0; i < 10; i++)
    {
        queue.customers[i] = SemaphoreNew("c_c", 0);
    }
    queue.lock = SemaphoreNew("queue.lock", 1);
}

int main(int argc, char **argv)
{
    InitThreadPackage(false); // this program is about selling cones.
    srand(time(NULL)); // should only be called once.
    SetSomeSemaphore(); 

    int totalCones = 0;
    for (int i = 0; i < 10; i++) // we have 10 customers.
    {
        int *v = (int *)malloc(sizeof(int));
        int cones = RandomInteger(1, 4);
        *v = cones;
        totalCones += cones;
        // itos
        char str[16];
        sprintf(str, "Customer[%d]", i);
        ThreadNew(str, Customer, 1, v);
    }
    ThreadNew("Cashier", Cashier, 0);
    ThreadNew("Manager", Manager, 1, &totalCones);

    printf("Main thread, totalCones is: %d\n", totalCones);

    RunAllThreads();

    for (int i = 0; i < 10; i++)
    {    
        SemaphoreWait(inspection.approvedAll); // waiting for all customers done
    }

    FreeThreadPackage();

    printf("Main thread done\n");

    return EXIT_SUCCESS;
}
