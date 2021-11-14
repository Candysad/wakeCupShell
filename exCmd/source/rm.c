//rm: remove  file(s) or  directory(ies)
//SYNOPSIS: rm a.c b.c
#include <stdio.h>

int main(int argc,char* argv[])
{
    if(argc == 1)
    {
        printf("WCS-rm:too few arguments,check again\n");
        return 1;
    }
    else
    {
        int k = 0;
        for(int i = 1;i < argc;i++)
        {
            k = remove(argv[i]);
            if(k != 0)
            {
                printf("WCS-rm:rename failed,check:%s\n",argv[i]);
            }
            k = 0;
        }
    }
    return 0;
}