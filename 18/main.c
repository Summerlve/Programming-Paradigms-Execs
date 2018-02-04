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

    printf("clerk done\n");
    SemaphoreSignal(clerksDone);

    return NULL;
}

void *Customer(void *args)
{
    int numCones = *(((int **)args)[1]);
    printf("customer's numCones is %d\n", numCones);
    Semaphore clerksDone = SemaphoreNew("clerk", 0);

    for (int i = 0; i < numCones; i++)
    {
        ThreadNew("clerk", Clerk, 1, clerksDone);
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

    printf("customer done\n");

    return NULL;
}

void *Cashier(void *args)
{
    for (int i = 0; i < 10; i++)
    {
        SemaphoreWait(queue.requested);
        // CheckOutMoney() here
        SemaphoreSignal(queue.customers[i]);

        printf("cashier checked out customer[%d]\n", i);
    }

    return NULL;
}

void *Manager(void *args)
{
    int totalCones = *(((int **)args)[1]);
    int approved = 0;
    int checked = 0;

    while (approved < totalCones)
    {
        SemaphoreWait(inspection.requested);
        checked ++;
        inspection.passed = RandomInteger(0, 1);
        if (inspection.passed) approved ++;
        printf("manager checked %d cones up", approved);
        SemaphoreSignal(inspection.finished);
    }

    SemaphoreSignal(inspection.approvedAll);

    return NULL;
}

void SetSomeSemaphore()
{
    printf("set some semaphore\n");
    // init inspection.
    inspection.passed = false;
    inspection.requested = SemaphoreNew("m_r", 0);
    inspection.finished = SemaphoreNew("m_f", 0);
    inspection.lock = SemaphoreNew("m_l", 1);
    inspection.approvedAll = SemaphoreNew("m_a", 0);

    // init queue.
    queue.number = 0;
    queue.requested = SemaphoreNew("c_r", 0);
    for (int i = 0; i < 10; i++)
    {
        queue.customers[i] = SemaphoreNew("c_c", 0);
    }
    queue.lock = SemaphoreNew("c_l", 1);
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
        ThreadNew("customer", Customer, 1, v);
    }
    ThreadNew("cashier", Cashier, 0);
    ThreadNew("manager", Manager, 1, &totalCones);

    printf("totalCones is: %d\n", totalCones);

    RunAllThreads();

    SemaphoreWait(inspection.approvedAll); // waiting for all cones done
    printf("manager checked all cones done\n");

    FreeThreadPackage();

    return EXIT_SUCCESS;
}
