//clear: clear the stdout
//SYNOPSIS: clear
#include <stdio.h>
#include <unistd.h>

int main()
{
    fflush(stdout);
    printf("\033[H\033[2J");
    return 0;
}