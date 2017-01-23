#include<stdio.h>

int main(void)
{
    struct stu {
        char *name;
        int age;
    } s, *ps = &s;

    s.name = "zhanghao";
    s.age = 11;

    printf("%lu\n", sizeof s);
    printf("%lu\n", sizeof (struct stu));
    printf("%s\n", (*ps).name);
    printf("%s\n", ps->name);

    struct stu array[] = {
        {"zhanghao", 110},
        {"wocao", 438}
    };

    printf("%lu\n", sizeof array / sizeof(struct stu));

    typedef char* String;

    String my_name = "summer";
    printf("%s\n", my_name);

    typedef struct stu Stu;
    typedef struct stu* Stuptr;

    Stu s2 = {"wori", 255};
    Stuptr s2p = &s2;

    printf("%s\n", s2p->name);

    typedef struct {
        char *word;
    } Word;

    Word w = {"thx"};
    printf("%s\n", w.word);
}
