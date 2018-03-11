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

static int Examine(void)
{
    return 1;
}

static void ReadEmail(void)
{

}

static void Debug(void)
{

}

static void Rejoice(void)
{

}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    for(int i = 0; i < NUM_TAS; i++) ThreadNew("TA", TA, 1, i);
    for(int i = 0; i < NUM_STUDENTS; i++) ThreadNew("Student", Student, 0);
    RunAllThreads();
    return EXIT_SUCCESS;
}