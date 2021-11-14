#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

//redirect to file
int redirect_file(char* filePath)
{
    int openf = 0;
    //open file
    openf = open(filePath, O_RDWR | O_CREAT,0644);
    if(openf == -1)
    {
        printf("redirect:file open failed,check again\n");
        return 1;
    }

    //redirect
    dup2(1,99);//change descriptor of stdout to 99

    dup2(openf,1);//change descriptor of file to 1
    close(openf);

    return 0;
}

//redirect to stdout 1
int redirect_stdout()
{
    int k =0;
    fflush(stdout);
    k = dup2(99,1);//change descriptor of stdout back to 1
    close(99);
    if(k == -1)
    {
        printf("redirect:redirect to stdout failed\n");   
        return 1;
    }
    return 0;
}

//redirect for pipe
int redirect_pipe(int fd[2])
{
    //open file
    dup2(1,99);//change descriptor of stdout to 99
    
    dup2(fd[1],1);////change descriptor of pipe_in to 1
    return 0;
}

//more than once redirect
int mto_redirect_pipe(int fd[2])
{
    dup2(fd[1],1);
    return 0;
}