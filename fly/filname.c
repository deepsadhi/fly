#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_FILENAME 1024
char* get_filename_from_path(char* fp){

    char filepath[SIZE_FILENAME];

    if (fp[0] != '/')
    {
        strcpy(filepath, "/");
        strcat(filepath, fp);
    }else{
        strcpy(filepath, fp);
    }

    char *filename = (char*)calloc(1, sizeof(SIZE_FILENAME));
    filename = (strrchr(filepath, '/'))+1;
    printf(" found filename: %s \n", filename);

    return filename;

}

int main()
{
    printf("%s\n", get_filename_from_path("/hi/iam/abc"));

    return 0;
}
