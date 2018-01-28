#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../16/thread_107/thread_107.h"

struct{
    bool passed;
    Semaphore requested;
    Semaphore finished;
    Semaphore lock; // binary lock for the manager
} inspection;

struct {
    int number;
    Semaphore requested;
    Semaphore customers[10];
    Semaphore lock; // binary lock for the number    
} queue;

void *Customer(void *args)
{
    return NULL;
}

void *Clerk(void *args)
{
    return NULL;
}

void *Cashier(void *args)
{
    return NULL;
}

void *Manager(void *args)
{
    return NULL;
}

void SetSomeSemaphore()
{
    // init inspection.
    inspection.passed = false;
    inspection.requested = SemaphoreNew("m_r", 0);
    inspection.finished = SemaphoreNew("m_f", 0); 
    inspection.lock = SemaphoreNew("m_l", 1);

    // init queue. 
    queue.number = 0;
    queue.requested = SemaphoreNew("c_r", 0);
    for (int i = 0; i < 10; i++)
    {
        queue.customers[i] = SemaphoreNew("c_c", 0);
    }
    queue.lock = SemaphoreNew("c_l", 1);
}

int RandomInteger(int min, int max)
{
    srand((unsigned int)time(0));
    int value = rand() % (max + 1 - min) + min; 
    return value;
}

int main(int argc, char **argv)
{
    // sell cones

    InitThreadPackage(false);
    
    SetSomeSemaphore();
    int totalCones = 0;
    for (int i = 0; i < 10; i++)
    {
        int cones = RandomInteger(1, 4);
        totalCones += cones;
        ThreadNew("customer", Customer, 1, &cones);
    }
    ThreadNew("cashier", Cashier, 0);
    ThreadNew("manager", Manager, 1, totalCones);

    RunAllThreads();
    FreeThreadPackage();

    return EXIT_SUCCESS;
}