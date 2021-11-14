//mv: rename a file
//SYNOPSIS: mv a.c b.c
#include <stdio.h>

int main(int argc,char* argv[])
{
    if(argc == 1)
    {
        printf("WCS-rename:enter path of the file to rename\n");
        return 1;
    }
    else if(argc == 2)
    {
        printf("WCS-rename:missing a path ,check again\n");
        return 1;
    }
    else if(argc > 3)
    {
        printf("WCS-rename:too much arguments\n");
        return 1;
    }
    else
    {
        int k = 0;
        k = rename(argv[1],argv[2]);
        if(k != 0)
        {
            printf("WCS-rename failed:check again\n");
        }
        return k;
    }
}