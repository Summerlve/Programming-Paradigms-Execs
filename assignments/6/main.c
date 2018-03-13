#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../16/thread_107/thread_107.h"

#define NUM_TAS 10
#define NUM_STUDENTS 115
#define NUM_MACHINES 20

typedef struct {
    Semaphore lock;
    Semaphore requested;
    Semaphore finished;
    int bugs;
} SingleTA;

SingleTA tas[NUM_TAS];
Semaphore Computer;

void SetSomeSemaphore(void)
{
    Computer =  SemaphoreNew("Computer", NUM_MACHINES);
    for (int i = 0; i < NUM_TAS; i++)
    {
        tas[i].lock = SemaphoreNew("TA_LOCK", 1);
        tas[i].requested = SemaphoreNew("TA_REQUESTED", 0);
        tas[i].finished = SemaphoreNew("TA_FINISHED", 0);
        tas[i].bugs = 0;
    }
}

void WaitComputer(void)
{

}

void SearchForAvailableTA(void)
{
    int index;
}

static int Examine(void)
{
    int min = 0;
    int max = 10;
    int value = rand() % (max + 1 - min) + min;
    return value;
}

static void ReadEmail(void)
{
    printf("TA is reading email\n");
}

static void Debug(void)
{
    printf("Student is debuging\n");
}

static void Rejoice(void)
{
    printf("Student rejoice\n");
}

void *TA(void *args)
{
    int ta;
    SemaphoreWait(tas[ta].requested);
    tas[ta].bugs = Examine();
    SemaphoreSignal(tas[ta].finished);
    ReadEmail();
    return NULL;
}

void *Student(void *args)
{
    int bugs = 1; // assume is just one bug initially
    int ta;

    while (bugs != 0 && bugs < 10)
    {
        WaitComputer();
        Debug();
        SearchForAvailableTA();
        SemaphoreSignal(tas[ta].requested);
        SemaphoreWait(tas[ta].finished);
        bugs = tas[ta].bugs;
    }

    SemaphoreSignal(NULL); // realse computer
    SemaphoreSignal(tas[ta].lock); // realse ta

    return NULL;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    srand(time(NULL)); // random seed init, should only be called once.
    SetSomeSemaphore();
    for(int i = 0; i < NUM_TAS; i++) ThreadNew("TA", TA, 1, i);
    for(int i = 0; i < NUM_STUDENTS; i++) ThreadNew("Student", Student, 0);
    RunAllThreads();
    return EXIT_SUCCESS;
}