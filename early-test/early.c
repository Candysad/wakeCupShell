#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

#define NUM 32 
 
int main() 
{ 
    for(;;)
    {
        char buff[1024] = {0}; 
        char tips[_SC_CHAR_MAX] = "[test@test]# ";//仿造一个命令行解释器提示符 
        printf("%s",tips); 
        fgets(buff, sizeof(buff)-1, stdin);//从键盘中获取用户输入的信息，不能用cin，因为会有空格 
        buff[strlen(buff) - 1] = 0; 
    
        char *argv[NUM];//为使用替换函数execvp，创建该数组以传递命令 
        argv[0] = strtok(buff," ");//字符串分隔函数，将每个命令分隔 
        int i = 0; 
        while(argv[i] != NULL){ 
            i++; 
            argv[i] = strtok(NULL," "); 
        } 
        pid_t id = fork(); 
        if(id == 0)//child
        {
            int k;
            k=execvp(argv[0],argv);
            printf("execvp returnning%d\n",k);
            exit(123);
        }
        else
        {//father
            int status = 0;
            waitpid(id, &status, 0);//创建子进程去执行相关命令，父进程只要等就行了
            if(WEXITSTATUS(status) == 123)
            {
                exit(123);
            }
        }
    }
    return 0;
}
