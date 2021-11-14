//cat: see content of a file
//SYNOPSIS: cat a.c b.c
#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
    char* check;
    if(argc == 1)
    {
        printf("WCS-cat:too few arguments\n");
        return 0;
    }
    for(int i = 1;i < argc;i++)
    {
        char c;
        FILE* f;
        f = fopen(argv[i],"r");
        if(f == NULL)
        {
            printf("WCS-cat:can't find this file:%s\n",argv[i]);
            continue;
        }
        c = getc(f);
        while(feof(f) == 0)
        {
            printf("%c",c);
            c = getc(f);
        }
        fclose(f);
    }
    return 0;
}
