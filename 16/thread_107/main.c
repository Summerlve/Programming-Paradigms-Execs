#include <stdio.h>
#include <stdbool.h>
#include "thread_107.h"

void *testThreadPackage(void *args)
{
    fputs("running testThreadPackage now\n", stdout);
}

int main(int argc, char **argv)
{

    InitThreadPackage(false);
    fputs("111111111\n", stdout);
    ThreadNew("testThreadPackage", testThreadPackage, 0);
    RunAllThreads();
    fputs("222222222\n", stdout);

    ThreadSleep(2000);

    return 0;
}
