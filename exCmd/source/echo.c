//echo: printf something as you entered,like an echo
//SYNOPSIS: echo aaa bbb
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char* argv[])
{
    char buff[50];
    for(int i = 1;i < argc;i++)
    {
        strcat(buff,argv[i]);
        strcat(buff," ");
    }
    printf("%s\n",buff);
    return 0;
}