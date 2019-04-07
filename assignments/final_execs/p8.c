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
    stpcpy(result->value, expr->value);

    return result;
}

void *evaluateExpressionAdapter(void *args)
{
    const char *debugName = ((char **)args)[0];
    Expression *expr = ((Expression **)args)[1];
    Semaphore rwLock = ((Semaphore *)args)[2];
    Semaphore done = ((Semaphore *)args)[3];
    Expression *result = ((Expression **)args)[4];

    Expression *temp = evaluateExpression(expr);

    // rwlock in write mode.
    SemaphoreWait(rwLock);
    strcpy(result->value, temp->value);
    free(temp);
    SemaphoreSignal(rwLock);

    SemaphoreSignal(done);
}

Expression *evaluateConcurrentAnd(Expression *exprs[], int n)
{
    Semaphore rwLock = SemaphoreNew("rwLock", 1);
    Semaphore done = SemaphoreNew("done", 0);

    Expression *result = (Expression *)malloc(sizeof(Expression));
    result->type = Boolean;
    strcpy(result->value, "true");

    for (int i = 0; i < n; i++) 
    {
        ThreadNew("evaluateExpression", evaluateExpressionAdapter, 4, exprs[i], rwLock, done, result);
    }

    RunAllThreads();

    for (int i = 0; i < n; i++)
    {
        SemaphoreWait(done);
        // rwlock in read mode. 
        SemaphoreWait(rwLock);
        if (strcmp(result->value, "false") == 0) return result;
        SemaphoreSignal(rwLock);
    }

    return result;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    Expression *exprs[4];
    for (int i = 0; i < 3; i++)
    {
        Expression expr = {
            .type = Boolean,
            .value = "true"
        };
        exprs[i] = &expr;
    }
    Expression expr = {
        .type = Boolean,
        .value = "false"
    };
    exprs[3] = &expr;

    Expression *result = evaluateConcurrentAnd(exprs, 4);
    printf("result is: %s \n", result->value);
    free(result);

    FreeThreadPackage();
    return 0; 
}