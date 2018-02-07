#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../16/thread_107/thread_107.h"

struct{
    bool passed;
    Semaphore requested;
    Semaphore finished;
    Semaphore lock; // binary lock for the manager
    Semaphore approvedAll; // waiting for all cones done.
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

    while (!passed)
    {
        // MakeCones() here
        SemaphoreWait(inspection.lock);
        SemaphoreSignal(inspection.requested);
        SemaphoreWait(inspection.finished);
        passed = inspection.passed;
        SemaphoreSignal(inspection.lock);
    }

    printf("%s, clerk done\n", debugName);
    SemaphoreSignal(clerksDone);

    return NULL;
}

void *Customer(void *args)
{
    char *debugName = *(char **)args;
    int numCones = *(((int **)args)[1]);
    printf("%s, numCones is %d\n", debugName, numCones);
    Semaphore clerksDone = SemaphoreNew("clerk", 0);

    for (int i = 0; i < numCones; i++)
    {
        // itos
        // char str[32];
        // sprintf(str, "Clerk[%d] of %s thread", i, debugName);
        ThreadNew("xxx", Clerk, 1, clerksDone);
    }

    for (int i = 0; i < numCones; i++)
    {
        SemaphoreWait(clerksDone);
    }

    // WalkToCashier() here
    SemaphoreWait(queue.lock);
    int place = queue.number ++;
    SemaphoreSignal(queue.lock);
    SemaphoreSignal(queue.requested);
    SemaphoreWait(queue.customers[place]);

    // free the numCones
    free(((int **)args)[1]);
    printf("%s, done\n", debugName);
    return NULL;
}

void *Cashier(void *args)
{
    char *debugName = *(char **)args;
    for (int i = 0; i < 10; i++)
    {
        SemaphoreWait(queue.requested);
        // CheckOutMoney() here
        SemaphoreSignal(queue.customers[i]);
        printf("%s, cashier checked customer[%d] out\n", debugName, i);
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
        if (inspection.passed) approved ++;
        printf("%s, manager have checked %d cones up in total\n", debugName, approved);
        SemaphoreSignal(inspection.finished);
    }

    SemaphoreSignal(inspection.approvedAll);

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
    // this program is about selling cones
    InitThreadPackage(false);
    srand(time(NULL)); // should only be called once.
    // we have 10 customers.
    SetSomeSemaphore();
    int totalCones = 0;
    for (int i = 0; i < 10; i++)
    {
        void *v = malloc(sizeof(int));
        int cones = RandomInteger(1, 4);
        memcpy(v, &cones, sizeof(int));
        totalCones += cones;
        // itos
        char str[24];
        sprintf(str, "Customer[%d] thread", i);
        ThreadNew(str, Customer, 1, v);
    }
    ThreadNew("Cashier thread", Cashier, 0);
    ThreadNew("Manager thread", Manager, 1, &totalCones);

    printf("Main thread, totalCones is: %d\n", totalCones);

    RunAllThreads();

    SemaphoreWait(inspection.approvedAll); // waiting for all cones done
    printf("Main thread, manager checked all cones done\n");

    FreeThreadPackage();

    return EXIT_SUCCESS;
}
