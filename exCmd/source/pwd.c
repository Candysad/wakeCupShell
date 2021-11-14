//pwd: show  current working directory
// /SYNOPSIS: pwd
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

int main()
{
    char currentPath[PATH_MAX];
    getcwd(currentPath,PATH_MAX);
    printf("WCS-pwd:%s\n",currentPath);
    return 0;
}