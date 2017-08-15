#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct {
    const char *licensePlate;
    int numTickets;
} maxTicketsStruct;

typedef struct {

} multiset;

typedef void (*MultiSetMapFunction)(void *elem, int count, void *auxData);

void MultiSetMap(multiset *ms, MultiSetMapFunction map, void *auxData)
{

}

static void getMax(void *elem, int count, void *auxData)
{
    maxTicketsStruct *ticketData = auxData;
    if (count > ticketData->numTickets)
    {
        ticketData->numTickets = count;
        ticketData->licensePlate = elem;
    }
}

void FindQueenOfParkingInfractions(multiset *ms, char licensePlateOfQueen[])
{
    maxTicketsStruct ticketData = {NULL, 0};
    MultiSetMap(ms, getMax, &ticketData);
    strcpy(licensePlateOfQueen, ticketData.licensePlate);
}

int main(void)
{

    return 0;
}
