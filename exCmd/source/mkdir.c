//mkdir: make a new directory
//SYNOPSIS: mkdir a
#include <stdio.h>
#include <sys/stat.h>


int main(int argc,char* argv[])
{
    if(argc == 1)
    {
        printf("WCS-mkdir:too few arguments\n");
        return 1;
    }
    else
    {
        int k = 0;
        for(int i = 1;i < argc;i++)
        {
            k = mkdir(argv[i],0755);
            if(k != 0)
            {
                printf("WCS-mkdir failed:%s\n",argv[i]);
            }
        }
    }
    return 0;
}