//grep: find a line with a keyword in the output of last command
//SYNOPSIS: ls | grep abc
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc,char* argv[])
{
    int k = 0;
    int buffLength;
    int lineSig[1024]  = {0};
    int lineL = 1;
    char lines[1024][1024];
    if(argc == 1)
    {
        k = -1;
        printf("grep: too few arguments\n");
        return k;
    }
    else if(argc > 3)
    {
        k = -1;
        printf("grep: too much arguments\n");
        return k;
    }
    else
    {
        char buff[1024*1024];
        k = argv[2][0] + '0';//change this to match normal mode
        //read from pipe
        read(k,buff,1024*1024);
        buffLength = strlen(buff);
        for(int i = 0;i < buffLength;i++)//change '\n' to ' ',record index for each line
        {
            if(buff[i] == '\n')
            {
                buff[i] = ' ';
                lineSig[lineL++] = i;
            }
        }
        lineSig[lineL++] = buffLength-1;
        int lineSL = 0;
        int cont = 0;
        for(int i = 0;i < lineL-1;i++)//copy each line to a new array
        {
            for(int j = lineSig[i];j < lineSig[i+1];j++)
            {
                lines[lineSL][cont++] = buff[j];
            }
            lines[lineSL][cont] = '\0';
            cont = 0;
            lineSL++;
        }

        for(int i = 0;i < lineSL;i++)//look for substring
        {
            if(strstr(lines[i],argv[1]) != NULL)
            {
                printf("%s\n",lines[i]);
            }
        }
    }
    return 0;
}