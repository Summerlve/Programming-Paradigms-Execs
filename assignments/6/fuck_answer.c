#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../16/thread_107/thread_107.h"
#define NUM_TAS 10
#define NUM_STUDENTS 115
#define NUM_MACHINES 20

static struct ta
{
    bool available;
    Semaphore availLock;
    Semaphore requested;
    Semaphore finished;
    int numBugs;
} tas[NUM_TAS];
static Semaphore numTAsAvailable;
static Semaphore numMachinesAvailable;
static int numStudentsLeft = NUM_STUDENTS;
static Semaphore studentsLeftLock;
static Semaphore TAFinished; // set to 0, waiting for NUM_TAS

static int RandomInteger(int min, int max)
{
    int value = rand() % (max + 1 - min) + min;
    return value;
}

static int Examine(void)
{
    int min = 0;
    int max = 10;
    return RandomInteger(min, max);
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

static void *TA(void *args)
{
    int id = *(((int **)args)[1]);
    printf("TA[%d] starts running\n", id);
    while (true)
    {
        SemaphoreWait(tas[id].requested);
        printf("TA[%d] got a request\n", id);
        if (numStudentsLeft == 0) break;
        tas[id].numBugs = Examine();
        SemaphoreSignal(tas[id].finished);
        ReadEmail();
    }

    SemaphoreSignal(TAFinished);
    return NULL; 
}

static void *Student(void * args)
{
    int numBugs = 1;
    int ta;
    int stu = *(((int **)args)[1]);
    printf("Student[%d] starts running\n", stu);

    // SemaphoreWait(numMachinesAvailable);
    printf("Student[%d] got a machine\n", stu);

    while ((numBugs > 0) && (numBugs < 10))
    {
        printf("Student[%d]: bugs is %d\n", stu, numBugs);
        Debug();
        SemaphoreWait(numTAsAvailable);
        printf("Student[%d] got a TA\n", stu);

        for (ta = 0; ta < NUM_TAS; ta++)
        {
            SemaphoreWait(tas[ta].availLock);
            if (tas[ta].available) break;
            SemaphoreSignal(tas[ta].availLock);
        }
        tas[ta].available = false;
        SemaphoreSignal(tas[ta].availLock);
        SemaphoreSignal(tas[ta].requested);
        SemaphoreWait(tas[ta].finished);
        numBugs = tas[ta].numBugs;
        printf("Student[%d] found %d bugs\n", stu, numBugs);
        tas[ta].available = true;
        SemaphoreSignal(numTAsAvailable);
    }

    if (numBugs == 0) Rejoice();
    SemaphoreWait(studentsLeftLock);
    numStudentsLeft--;
    bool everyoneDone = (numStudentsLeft == 0);
    SemaphoreSignal(studentsLeftLock);

    if (everyoneDone)
    {
        for (ta = 0; ta < NUM_TAS; ta++)
        {
            SemaphoreSignal(tas[ta].requested);
        }
    }

    SemaphoreSignal(numMachinesAvailable);

    return NULL;
}

void SetSomeSemaphore(void)
{
    numMachinesAvailable = SemaphoreNew("numMachinesAvailable", NUM_MACHINES);
    numTAsAvailable = SemaphoreNew("numTAsAvailable", NUM_TAS);
    TAFinished = SemaphoreNew("TAFinished", 0);
    studentsLeftLock = SemaphoreNew("CounterLock", 1);
    for (int i = 0; i < NUM_TAS; i++)
    {
        tas[i].availLock =  SemaphoreNew("availLock", 1);
        tas[i].requested = SemaphoreNew("requested", 0);
        tas[i].finished = SemaphoreNew("finished", 0);
        tas[i].numBugs = 0;
        tas[i].available = true;
    }
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    srand(time(NULL)); // random seed init, should only be called once.
    SetSomeSemaphore();
    for (int i = 0; i < NUM_TAS; i++)
    {
        int *ta_index = malloc(sizeof(int));
        *ta_index = i;
        ThreadNew("TA", TA, 1, ta_index);
    }
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        int *stu_index = malloc(sizeof(int));
        *stu_index = i;
        ThreadNew("Student", Student, 1, stu_index);
    }
    RunAllThreads();
    for (int i = 0; i < NUM_TAS; i++)
        SemaphoreWait(TAFinished);
    return EXIT_SUCCESS;
}