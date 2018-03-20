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
    bool available;
} SingleTA;

SingleTA tas[NUM_TAS];
Semaphore Computers;
int StudentCounter = 0;
bool isFinished = false;

void SetSomeSemaphore(void)
{
    Computers =  SemaphoreNew("Computer", NUM_MACHINES);
    for (int i = 0; i < NUM_TAS; i++)
    {
        tas[i].lock = SemaphoreNew("TA_LOCK", 1);
        tas[i].requested = SemaphoreNew("TA_REQUESTED", 0);
        tas[i].finished = SemaphoreNew("TA_FINISHED", 0);
        tas[i].bugs = 0;
        tas[i].available = true;
    }
}

static int RandomInteger(int min, int max)
{
    int value = rand() % (max + 1 - min) + min;
    return value;
}

int SearchForAvailableTA(void)
{
    int ta;

    for (int i = 0; i < NUM_TAS; i++)
    {
        if (tas[i].available == true) 
        {
            ta = i;
            break;
        }
    }

    ta = RandomInteger(0, NUM_TAS - 1);
    
    return ta;
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

void *TA(void *args)
{
    int ta = *(((int **)args)[1]);

    while (isFinished == false)
    {
        SemaphoreWait(tas[ta].requested);
        tas[ta].bugs = Examine();
        SemaphoreSignal(tas[ta].finished);
        ReadEmail();
    }

    printf("TA: I can go home now\n");
    return NULL;
}

void *Student(void *args)
{
    int bugs = 1; // assume is just one bug initially
    int ta;
    SemaphoreWait(Computers);

    while (bugs != 0 && bugs < 10)
    {
        Debug();
        ta = SearchForAvailableTA();
        SemaphoreWait(tas[ta].lock);
        tas[ta].available = false;
        SemaphoreSignal(tas[ta].requested);
        SemaphoreWait(tas[ta].finished);
        bugs = tas[ta].bugs;
    }

    SemaphoreSignal(Computers); // realse computer
    SemaphoreSignal(tas[ta].lock); // realse ta

    Rejoice();

    PROTECT(
        StudentCounter ++;
        if (StudentCounter == NUM_STUDENTS)
        {
            isFinished = true;
            printf("The last student: wake up all the TAs to tell them that they can all go home");
            for (int i = 0; i < NUM_TAS; i++)
            {
                SemaphoreSignal(tas[i].requested);
            }
        }
    )

    return NULL;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    srand(time(NULL)); // random seed init, should only be called once.
    SetSomeSemaphore();
    for(int i = 0; i < NUM_TAS; i++) {
        int *ta_index = malloc(sizeof(int));
        *ta_index = i;
        ThreadNew("TA", TA, 1, ta_index);
    }
    for(int i = 0; i < NUM_STUDENTS; i++) ThreadNew("Student", Student, 0);
    RunAllThreads();
    return EXIT_SUCCESS;
}
