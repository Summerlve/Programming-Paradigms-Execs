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
    strcpy(result->value, expr->value);

    return result;
}

void *evaluateExpressionAdapter(void *args)
{
    const char *debugName = ((char **)args)[0];
    Expression *expr = ((Expression **)args)[1];
    Semaphore lock = ((Semaphore *)args)[2];
    Semaphore done = ((Semaphore *)args)[3];
    Semaphore resultReady = ((Semaphore *)args)[4];
    Semaphore resultChecked = ((Semaphore *)args)[5];
    Expression *result = ((Expression **)args)[6];

    Expression *temp = evaluateExpression(expr);
    SemaphoreSignal(done);

    SemaphoreWait(lock);
    result->type = temp->type;
    strcpy(result->value, temp->value);
    free(temp);
    SemaphoreSignal(resultReady);
    SemaphoreWait(resultChecked);
    SemaphoreSignal(lock);

}

Expression *evaluateConcurrentAnd(Expression *exprs[], int n)
{
    Semaphore lock = SemaphoreNew("rwLock", 1);
    Semaphore done = SemaphoreNew("done", 0);
    Semaphore resultReady = SemaphoreNew("resultReady", 1);
    Semaphore resultChecked = SemaphoreNew("resultChecked", 1);

    Expression *result = (Expression *)malloc(sizeof(Expression));
    result->type = Boolean;
    strcpy(result->value, "true");

    for (int i = 0; i < n; i++) 
    {
        ThreadNew("evaluateExpression", evaluateExpressionAdapter, 6, exprs[i], lock, done, resultReady, resultChecked, result);
    }

    RunAllThreads();

    for (int i = 0; i < n; i++)
    {
        SemaphoreWait(done);
        SemaphoreWait(resultReady);
        if (result->type != Boolean) return result;
        if (strcmp(result->value, "false") == 0) return result;
        SemaphoreSignal(resultChecked);
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