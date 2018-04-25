#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define __APPLE__
#include "../../16/thread_107/thread_107.h"

#define NUM_TAS 10
#define NUM_STUDENTS 115
#define NUM_MACHINES 20

struct {
    Semaphore availLock; // set to 1
    Semaphore requested; // set to 0
    Semaphore finished; // set to 0
    int bugs; // set to 0
    bool available; // set to true
} tas[NUM_TAS];

static Semaphore Computers; // set to NUM_MACHINES
static Semaphore TAs; // set to NUM_TAS
static int StudentCounter = 0;
static Semaphore StudentCounterLock; // set to 1
Semaphore TAFinished; // set to 0, waiting for NUM_TAS

static int RandomInteger(int min, int max)
{
    int value = rand() % (max + 1 - min) + min;
    fprintf(stdout, "Random result is: %d\n", value);
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
    fprintf(stdout, "TA is reading email\n");
}

static void Debug()
{
    fprintf(stdout, "Student is debuging\n");
}

static void Rejoice(void)
{
    fprintf(stdout, "Student rejoice\n");
}

static void *TA(void *args)
{
    int ta = *(((int **)args)[1]);
    fprintf(stdout, "TA[%d] starts running\n", ta);

    while (true)
    {
        SemaphoreWait(tas[ta].requested);
        if (StudentCounter == NUM_STUDENTS) break;
        tas[ta].bugs = Examine();
        fprintf(stdout, "TA[%d] found %d bugs\n", tas[ta].bugs, ta);
        SemaphoreSignal(tas[ta].finished);
        ReadEmail();
    }

    fprintf(stdout, "TA[%d]: I can go home now\n", ta);
    SemaphoreSignal(TAFinished);
    free((((int **)args)[1]));
    return NULL;
}

static void *Student(void *args)
{
    int bugs = 1; // assume is just one bug initially
    int ta;
    int stu = *(((int **)args)[1]);

    fprintf(stdout, "Student[%d] starts running\n", stu);

    SemaphoreWait(Computers);

    while ((bugs > 0) && (bugs < 10))
    {
        fprintf(stdout, "Student[%d]: bugs is %d\n", stu, bugs);
        Debug();
        SemaphoreWait(TAs);
        fprintf(stdout, "Student[%d] I got one TA\n", stu);
        for (ta = 0; ta < NUM_TAS + 1; ta++)
        {
            ta = ta % NUM_TAS;
            SemaphoreWait(tas[ta].availLock);
            if(tas[ta].available) break;
            SemaphoreSignal(tas[ta].availLock);
        }
        fprintf(stdout, "Student[%d]: waiting for TA[%d]\n", stu, ta);
        tas[ta].available = false;
        SemaphoreSignal(tas[ta].availLock);
        SemaphoreSignal(tas[ta].requested);
        fprintf(stdout, "Student[%d]: requesting for TA[%d]\n", stu, ta);
        SemaphoreWait(tas[ta].finished);
        bugs = tas[ta].bugs;
        tas[ta].available = true;
        SemaphoreSignal(TAs);
    }

    if (bugs == 0) Rejoice();

    SemaphoreWait(StudentCounterLock);
    StudentCounter ++;
    bool done = (StudentCounter == NUM_STUDENTS);
    SemaphoreSignal(StudentCounterLock);

    if (done)
    {
        for (ta = 0; ta < NUM_TAS; ta++)
        {
            SemaphoreSignal(tas[ta].requested);
        }
    }
    
    SemaphoreSignal(Computers); // realse computer
    free((((int **)args)[1]));
    return NULL;
}

static void SetSomeSemaphores(void)
{
    TAs = SemaphoreNew("TAs", NUM_TAS);
    Computers = SemaphoreNew("Computers", NUM_MACHINES);
    TAFinished = SemaphoreNew("TAFinished", 0);
    StudentCounterLock = SemaphoreNew("CounterLock", 1);
    for (int i = 0; i < NUM_TAS; i++)
    {
        tas[i].availLock = SemaphoreNew("TA_LOCK", 1);
        tas[i].requested = SemaphoreNew("TA_REQUESTED", 0);
        tas[i].finished = SemaphoreNew("TA_FINISHED", 0);
        tas[i].bugs = 0;
        tas[i].available = true;
    }
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    srand(time(NULL)); // random seed init, should only be called once.
    SetSomeSemaphores();
    for (int i = 0; i < NUM_TAS; i++) {
        int *ta_index = malloc(sizeof(int));
        *ta_index = i;
        char ta_str[16];
        sprintf(ta_str, "TA[%d]", i);
        ThreadNew(ta_str, TA, 1, ta_index);
    }
    for (int i = 0; i < NUM_STUDENTS; i++) {
        int *stu_index = malloc(sizeof(int));
        *stu_index = i;
        char stu_str[16];
        sprintf(stu_str, "Student[%d]", i);
        ThreadNew(stu_str, Student, 1, stu_index);
    }
    RunAllThreads();
    for (int i = 0; i < NUM_TAS; i++) SemaphoreWait(TAFinished);
    FreeThreadPackage();
    fprintf(stdout, "All things done\n");
    return EXIT_SUCCESS;
}