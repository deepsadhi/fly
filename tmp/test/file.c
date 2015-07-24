#include <stdio.h>
#define SIZE 5

int main()
{
    FILE *fp;
    char c[5];
    size_t s;
    int cur=1;
    int i ;

    fp = fopen("f.txt", "r");


    //fread(c, 6, 1, fp);
    //printf("%s\n", c);

    fseek(fp, SEEK_SET, 0);
    while(!feof(fp))
    {
        s = fread(c, sizeof(char), 5, fp);
        printf("size = %d\n", s);

        if (s < 5)
        {
            for(i = (s-1); i<SIZE; i++)
                c[i] = '\0';
        }
    
        printf("%s\n", c);
    }

    fclose(fp);

return 0;
}
