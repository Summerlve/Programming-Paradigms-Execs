#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../16/thread_107/thread_107.h"

#define NUM_TAS 10
#define NUM_STUDENTS 115
#define NUM_MACHINES 20



void *TA(void *args)
{
    return NULL;
}

void *Student(void *args)
{

    return NULL;
}

void SetSomeSemaphore(void)
{

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

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    srand(time(NULL)); // random seed init, should only be called once.
    for(int i = 0; i < NUM_TAS; i++) ThreadNew("TA", TA, 1, i);
    for(int i = 0; i < NUM_STUDENTS; i++) ThreadNew("Student", Student, 0);
    RunAllThreads();
    return EXIT_SUCCESS;
}