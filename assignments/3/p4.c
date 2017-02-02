static void InRangePrint(void *keyAddr, void *valueAddr, void *auxData)
{
    char *zipcode;
    char *city;
    char *low;
    char *high;

    zipcode = (char *)keyAddr;
    low = ((char **)auxData)[0];
    high = ((char **)auxData)[1];
    city = *(char **)valueAddr;

    if ...
}
