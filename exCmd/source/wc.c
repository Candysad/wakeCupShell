//wc:check contents of a file,output count of characters as c,lines as l
//SYNOPSIS: wc a.c
#include <stdio.h>

int main(int argc,char* argv[])
{
    if(argc == 1)
    {
        printf("WCS-Wc:too few arguments\n");
        return 1;
    }
    else if(argc > 2)
    {
        printf("WCS-Wc:too much arguments\n");
    }
    else
    {
        FILE* f;
        f = fopen(argv[1],"r");
        if(f == NULL)
        {
            printf("WCS-wc:can't find this file%s\n",argv[1]);
        }
        int i = 0;
        int line = 0;
        char c;
        c = getc(f);
        if(c == '\n')
        {
            line++;
        }
        while(feof(f) == 0)
        {
            c = getc(f);
            if(c == '\n')
            {
                line++;
            }
            i++;
        }
        printf("WCS-Wc: %s: c:%d l:%d\n",argv[1],i,line);
        return 0;
    }
    return 0;
}