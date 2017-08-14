#include<stdlib.h>
#include<string.h>
#include<stdio.h>

void *packetize(const void *image, int size, int packetSize)
{
    int numOfPackets = size / packetSize + 1;
    void *packetAddrs[numOfPackets];

    for (int i = 0; i < numOfPackets; i++)
    {
        int restBytes = size - i * packetSize;
        int last = restBytes / packetSize;

        if (last >= 1)
        {
            void *packet = malloc(packetSize + sizeof(void *));
            memcpy(packet, (char *)image + i * packetSize, packetSize);
            packetAddrs[i] = packet;
        }
        else
        {
            void *packet = malloc(restBytes + sizeof(void *));
            memcpy(packet, (char *)image + i * packetSize, restBytes);
            packetAddrs[i] = packet;
        }
    }

    for (int i = 0; i + 1 < numOfPackets; i++)
    {
        memcpy((char *)packetAddrs[i] + packetSize, &packetAddrs[i + 1], sizeof(void *));
    }

    *(void **)((char *)packetAddrs[numOfPackets - 1] + size % packetSize) = NULL;

    return packetAddrs[0];
}

int main(int argc, char **argv)
{
    const void *image = malloc(1700);
    char ch = 'A';
    char ch2 = 'T';
    memcpy((char *)image + 1200, &ch, 1);
    memcpy((char *)image + 1501, &ch2, 1);
    void *list = packetize(image, 1700, 500);

    void *p1 = list;
    void *p2 = *(char **)((char *)p1 + 500);
    void *p3 = *(char **)((char *)p2 + 500);
    void *p4 = *(char **)((char *)p3 + 500);
    printf("%c\n", *((char *)p3 + 200));
    printf("%c\n", *((char *)p4 + 1));
    return 0;
}
