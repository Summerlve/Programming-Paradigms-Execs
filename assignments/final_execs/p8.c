#include "../../16/thread_107/thread_107.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <string.h>
#define __APPLE__

typedef struct {
    enum {Boolean, Integer, String, Symbol, Empty, List} type;
    char value[8];
} Expression;

Expression *evaluateExpression(Expression *expr)
{
    Expression *result = (Expression *)malloc(sizeof(Expression));
    result->type = Boolean;
    stpcpy(result->value, "true");

    return result;
}

void *evaluateExpressionAdapter(void *args)
{
    const char *debugName = ((char **)args)[0];
    Expression *expr = ((Expression **)args)[1];
    Semaphore writeLock = ((Semaphore *)args)[2];
    Semaphore readLock = ((Semaphore *)args)[3];
    Semaphore done = ((Semaphore *)args)[4];
    Expression *result = ((Expression **)args)[5];

    Expression *temp = evaluateExpression(expr);

    // rwlock in write mode.
    SemaphoreWait(readLock);
    SemaphoreWait(writeLock);
    strcpy(result->value, temp->value);
    SemaphoreSignal(writeLock);
    SemaphoreSignal(readLock);

    SemaphoreSignal(done);
}

Expression *evaluateConcurrentAnd(Expression *exprs[], int n)
{
    Semaphore writeLock = SemaphoreNew("writeLock", 1);
    Semaphore readLock = SemaphoreNew("readLock", 1);
    Semaphore done = SemaphoreNew("done", 0);

    Expression *result = (Expression *)malloc(sizeof(Expression));
    result->type = Boolean;
    strcpy(result->value, "true");

    for (int i = 0; i < n; i++) 
    {
        ThreadNew("evaluateExpression", evaluateExpressionAdapter, 5, exprs[i], writeLock, readLock, done, result);
    }

    RunAllThreads();

    for (int i = 0; i < n; i++)
    {
        SemaphoreWait(done);
        // rwlock in read mode. 
        SemaphoreWait(writeLock);
        SemaphoreWait(readLock);
        if (strcmp(result->value, "false") == 0) return result;
        SemaphoreSignal(readLock);
        SemaphoreSignal(writeLock);
    }

    return result;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    Expression *result = evaluateConcurrentAnd(NULL, 4);
    printf("result is: %s \n", result->value);
    free(result);

    FreeThreadPackage();
    return 0; 
}