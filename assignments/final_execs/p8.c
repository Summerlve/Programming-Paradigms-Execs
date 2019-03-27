#include <stdlib.h>
#include <stdio.h>

typedef struct {
    enum {Boolean, Integer, String, Symbol, Empty, List} type;
    char value[8];
} Expression;

int main(int argc, char **argv)
{
    return 0; 
}