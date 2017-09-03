#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct {
    char *name;
    char **friends;
    int numfriends;
} person;

person *decompress(void *image)
{
    int recordsSize = *(int *)image;
    int rest = recordsSize;
    image = (char *)image + sizeof(int);
    person *results = malloc(sizeof(person) * recordsSize);

    while (rest-- > 0) {
        person p;
        p.name = malloc(strlen((char *)image) + 1);
        strcpy(p.name, image);
        int nameLength = strlen(p.name) + 4 - (strlen(p.name) % 4);
        p.numfriends = *(int *)((char *)image + nameLength);
        p.friends = malloc(p.numfriends * sizeof(char *));
        image = (char *)image + nameLength + sizeof(int);

        for (int i = 0; i < p.numfriends; i++)
        {
            p.friends[i] = malloc(strlen(*(char **)image) + 1);
            strcpy(p.friends[i], *(char **)image);
            image = (char *)image + sizeof(char *);
        }

        memcpy((char *)results + (recordsSize - rest - 1) * sizeof(person), &p, sizeof(person));
    }

    return results;
}

int main(int argc, char **argv)
{
    void *image = malloc(sizeof(int) * 3 + 12 + sizeof(char *) * 2);
    void *imageOri = image;
    *(int *)image = 2;
    char a[] = "Alice";
    char f[] = "Flo";
    memcpy((char *)image + sizeof(int), a, 6);
    image = (char *)image + sizeof(int) + 8;
    *(int *)image = 1;
    image = (char *)image + sizeof(int);

    *(char **)image = (char *)image + sizeof(char *);
    image = (char *)image + sizeof(char *);
    memcpy(image, f, 4);

    image = (char *)image + 4;
    *(int *)image = 1;
    image = (char *)image + sizeof(int);
    *(char **)image = (char *)image - (sizeof(int) * 2 + 12 + sizeof(char *));

    person *results = decompress(imageOri);

    printf("%s\n", results[0].name);
    printf("%s\n", results[1].name);

    free(imageOri);
    return 0;
}
