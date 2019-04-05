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
    Expression e = {
        .type = Boolean,
        .value = "false"
    };

    Expression ee;

    Expression *ep = (Expression *)malloc(sizeof(Expression));
    memcpy(ep, &e, sizeof(Expression));
    
    return ep;
}

void *evaluateExpressionAdapter(void *args)
{
    const char *debugName = NULL;
    Expression *e = NULL;

    Expression *result = evaluateExpression(e);
}

Expression *evaluateConcurrentAnd(Expression *exprs[], int n)
{
    Expression *e = (Expression *)malloc(sizeof(Expression));
    e->type = Boolean;
    strcpy(e->value, "true");
    Semaphore resultLock = SemaphoreNew("resultLock", 1);
    Semaphore done = SemaphoreNew("done", 0);

    for (int i = 0; i < n; i++) 
    {
        ThreadNew("evaluateExpression", evaluateExpressionAdapter, 1, exprs[i]);
    }

    RunAllThreads();

    for (int i = 0; i < n; i++)
    {
        SemaphoreWait(done);
        if (strcmp(e->value, "false")) return e;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);
    return 0; 
}