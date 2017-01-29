#include<string.h>
#include<stdlib.h>
#include<assert.h>

typedef int (*MultiTableHashFunction)(const void *keyAddr, int numBuckets);
typedef int (*MultiTableCompareFunction)(const void *keyAddr1, const void *keyAddr2);
typedef int (*MultiTableMapFunction)(void *keyAddr, void *valueAddr, void *auxData);
typedef void (*HashSetMapFunction)(void *curElem);

typedef struct {
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
} vector;

typedef struct {
    int numBuckets;
    MultiTableHashFunction hash;
    MultiTableCompareFunction compare;
    void *elems;
    int elemSize;
    int logiclLength;
    int allocatedLength;
} hashset;

typedef struct {
    hashset mappings;
    int keySize;
    int valueSize;
} multitable;


