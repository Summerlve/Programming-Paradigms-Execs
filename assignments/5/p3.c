#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void *packPackets(short *list)
{
    int packetSize = 0;
    int resultSize = 0;
    short *node = list;
    short *packet = list;
    void *result = NULL;

    while(node != NULL)
    {
        while(*packet != 0)
        {
            packetSize = (int)(*packet);
            packet = (short *)((char *)packet + sizeof(short));
            result = realloc(result, resultSize + packetSize);
            memcpy((char *)result + resultSize, packet, packetSize);
            resultSize += packetSize;
            packet = (short *)((char *)packet + packetSize);
        }

        node = *(short **)((char *)packet + sizeof(short));
        packet = node;
    }

    return result;
}

int main(int argc, char **argv)
{
    short *list = malloc(24);
    short *node = list;

    *node = 4;
    memcpy((char *)node + 2, "abcd", 4);
    node = (short *)((char *)node + 6);

    *node = 6;
    memcpy((char *)node + 2, "efghij", 6);
    node = (short *)((char *)node + 8);

    *node = 2;
    memcpy((char *)node + 2, "kl", 2);
    node = (short *)((char *)node + 4);

    *node = 0;
    node = (short *)((char *)node + 2);

    short *node2 = malloc(20);
    memcpy(node, &node2, sizeof(short *));

    *node2 = 2;
    memcpy((char *)node2 + 2, "mn", 2);
    node2 = (short *)((char *)node2 + 4);

    *node2 = 8;
    memcpy((char *)node2 + 2, "opqrstu\0", 8);
    node2 = (short *)((char *)node2 + 10);

    *node2 = 0;
    node2 = (short *)((char *)node2 + 2);
    *(short **)node2 = NULL;

    void *result = packPackets(list);
    printf("result: %s\n", (char *)result);
}
