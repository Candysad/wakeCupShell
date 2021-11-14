//this is the C file for WCS-ls
//ls: list the content of a directory
//SYNOPSIS: ls /home
//as the first function of WCS project,there are a lot comments of this programe in this file
//formats of head file list for external commands are not stictly required
//thus no need to include "WCSstd.h"
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>

//this is the function to print list of a directory
//return as:
//0 succeed
//1 failed
int printdir(char *dir)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    if ((dp = opendir(dir)) == NULL)//open DIR to dp
    {
        perror("WSC-ls - open diretory structure DIR failed");
        return 1;
    }
    while ((entry = readdir(dp)) != NULL)
    {
        if (strncmp(entry->d_name, ".", 1) == 0)//sikp hiding directory
        {
            continue;
        }
        else if(entry->d_type == 4)
        {
            printf("%s : dir \n",entry->d_name);
        }
        else if(entry->d_type == 8)
        {
            printf("%s : file \n",entry->d_name);
        }
        else
        {
            printf("%s\n",entry->d_name);
        }
    }
    closedir(dp);
    return 0;
}

//the main function for WCS-ls
//the return dosen't mean the result(succeed/failed) of this function 
int main(int argc,char *argv[])
{
    printf("This is the progarm for external command \"ls\" of WCS\n\n");
    int k = 0;
    char topdir[128] = "./";//default directory : present directory
    if (argc >= 2)//argv[0] == "WCSls" or "./ls" 
    {
        for(int i = 1;i < argc;i++)
        {
            printf("***WCS-ls:%s***\n",argv[i]);
            k = printdir(argv[i]);
            if(k)
            {
                printf("error\n");
            }
            printf("***PRINT OVER: directory %d***\n\n",i);//relative path acceptable
            /*useless part
            if(strncmp("/",argv[i],1) == 0)//directory start as "/"
            {
                printf("***WCS-ls:%s***\n",argv[i]);
                k = printdir(argv[i]);
                if(k)
                {
                    printf("error\n");
                }
                printf("***PRINT OVER: directory %d***\n\n",i);
            } 
            else if(strncmp(".",argv[i],1) == 0)//directory start as "."  . .. ~
            {
                printf("***WCS-ls:%s***\n",argv[i]);
                k = printdir(argv[i]);
                if(k)
                {
                    printf("error\n");
                }
                printf("***PRINT OVER: directory %d***\n\n",i);
            }
            else//is a present directory for short
            {
                strcat(topdir,argv[i]);
                printf("***WCS-ls:%s***\n",argv[i]);
                k = printdir(topdir);
                if(k)
                {
                    printf("error\n");
                }
                printf("***PRINT OVER: directory %d***\n\n",i);
                strcpy(topdir,"./");
            }
            */
        }
    }
    else//only 1 argument as commanding "WCS-ls"
    {
        printf("WCS-ls: ./\n");
        k = printdir(topdir);
        if(k)
        {
        printf("error\n");
        }
        char cwd[PATH_MAX];
        getcwd(cwd,PATH_MAX);
        printf("***PRINT OVER: directory: %s***\n\n",cwd);
    }
    return k;
}