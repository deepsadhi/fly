#include <stdio.h>

struct s
{
    int num ;
};

void fa(int a)
{
    printf("a = %d\n", a);
}

void fs(struct s *s1)
{
    s1->num = 10;
}

void fb(int *b)
{
    printf("b = %d\n", *b);
    *b=5;
}

int main()
{
    int a = 1;
    int *b = 2;
    struct s *s1;

    fa(a);
    fb(&b);

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("s = %d\n", s1->num);
    

return 0;
}

    
