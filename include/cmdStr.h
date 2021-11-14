//WARNING:function strtok()  always use a same RAM space 
//the last result of this function will be changed after using
//ONLY use once in cmdStr.h to get command form stdin  

//standard C head file for project wakeCupShell
#ifndef WCS_std
#include "WCSstd.h"
#endif

//interface
#include "redirect.h"
#include "../inCmd/inCmd.c"

//Some Macros
#define _MAX_ARGUMENTS 32
#define _MAX_BUFF_LENGTH 128
#define _MAX_CMD_LENGTH 20

//structure of command
typedef struct cmd_s
{
    int type;//0:internal command; 1:external command; 
             //2:directory;
             //3:pipe command; 4:redirect command 
    int arguments;//amount of arguments
    int subs;//amount of subcommands
    char* cmds[_MAX_ARGUMENTS];//command with arguments
    struct cmd_s* subCmds[_MAX_ARGUMENTS];//subcommands for pipe command
    //in recursive definition : can't recursivly use alias of structure itself
}CMD_S;

//function list
int _getCmd(CMD_S* cmd,char* informations);
int _piReAnalyzeCmd(CMD_S* cmd,CMD_S* InCmdList,CMD_S* ExCmdList);
int _basicAnalyzeCmd(CMD_S* cmd,CMD_S* InCmdList,CMD_S* ExCmdList);
int _execCmd(CMD_S* cmd);
int _inExec(CMD_S* cmd);
int _exExec(CMD_S* cmd);
int _fileExec(CMD_S* cmd);
int _reExec(CMD_S* cmd);
int _pipeExec(CMD_S* cmd);
void _freeCmd(CMD_S* cmd);

//functions for command

//1#
//This function is used to
//get a command from stdin, put it into the structure cmd_S
//if succeed return an integer > 0
//else if blank command return 0
int _getCmd(CMD_S* cmd,char* informations)
{
    char* tmp;
    char* buff;
    char* read_line = NULL;
    read_line = readline(informations);//the returning of readline is end as '\0' not '\n'
    if(strcmp(read_line,"\0") == 0)//blank
    {
        //free readline buff
        free(read_line);
        return 0;//
    }
    buff = strdup(read_line);
    //free readline buff
    free(read_line);

    //buff[strlen(buff) - 1] = '\0';//the final element of buff from fgets() would be '\n'
                                  //change to '\0' for ending it
    
    //add to history
    add_history(buff);
    //save all history
    write_history("/home/candy/wakeCupShell/.wake_history");
    
    cmd->cmds[0]=strdup(strtok(buff," "));
    int i = 1;
    while((tmp = strtok(NULL," ")) != NULL)
    {
        cmd->cmds[i] = strdup(tmp);
        i++;
        if(i == _MAX_ARGUMENTS)
        {
            printf("command too long\n");
            return 0;
        }
    }
    cmd->arguments= i;
    cmd->cmds[cmd->arguments] = NULL;
    return i;
}   

//2#
//This function is used to
//analyze pipe and redirect command
//return as: 
//0 succeed 
//1 failed unknown command
//-1 not a pipe or redirect command
int _piReAnalyzeCmd(CMD_S* cmd,CMD_S* InCmdList,CMD_S* ExCmdList)
{
    int s = 1;
    int sigP = 0;
    int sigR = 0;
    //illegal command check
    for(int i = 0;i < cmd->arguments;i++)
    {
        if(strcmp(">",cmd->cmds[i]) == 0)
        {
            sigR = 1;
        }
        else if(strcmp("|",cmd->cmds[i]) == 0)
        {
            sigP = 1;
        }
        if(sigP&&sigR)
        {
            printf("analyze:illegal command,check again\n");
            return 1;
        }
    }
    //redirect check
    if(sigR)
    {
        for(int i = 0;i < cmd->arguments;i++)
        {
            
            if(strcmp(">",cmd->cmds[i]) == 0)
            {
                if(i == cmd->arguments - 1)
                {
                    printf("analyze:wrong command\n");
                    cmd->subs = 0;
                    cmd->type = 4;// 4 : redirect
                    return 1;
                }
                else
                {
                    cmd->subCmds[0] = malloc(sizeof(CMD_S));   
                    cmd->subCmds[1] = malloc(sizeof(CMD_S));
                    cmd->subs = 2;
                    cmd->type = 4;// 4 : redirect

                    for(int j = 0;j < i;j++)
                    {
                        cmd->subCmds[0]->cmds[j] = strdup(cmd->cmds[j]);
                        if(j == i - 1)
                        {
                            cmd->subCmds[0]->arguments = j + 1;
                            cmd->subCmds[0]->cmds[j + 1] = NULL;
                        }
                    }
                    for(int j = i + 1;j < cmd->arguments;j++)
                    {
                        cmd->subCmds[1]->cmds[j-i-1] = strdup(cmd->cmds[j]);
                        if(j == cmd->arguments - 1)
                        {
                            cmd->subCmds[1]->arguments = j - i;
                            cmd->subCmds[1]->cmds[j + 1] = NULL;
                        }
                    }
                    _basicAnalyzeCmd(cmd->subCmds[0],InCmdList,ExCmdList);
                    cmd->subCmds[1]->type = 2;//directory
                    return 0;
                }
            }
        }
        
    }
    //pipe check
    else if(sigP)
    {
        cmd->type = 3;// 3 : pipe
        for(int i = 0;i < cmd->arguments;i++)
        {
            if(strcmp("|",cmd->cmds[i]) == 0)
            {
                if(i == cmd->arguments - 1)
                {
                    printf("analyze:wrong command\n");
                    cmd->subs = 0;
                    return 1;
                }
                else
                {
                    int count = 0;
                    cmd->subCmds[count] = malloc(sizeof(CMD_S));
                    for(int j = 0;j < i;j++)
                    {
                        cmd->subCmds[count]->cmds[j] = strdup(cmd->cmds[j]);
                        if(j == i - 1)
                        {
                            cmd->subCmds[0]->arguments = j + 1;
                            cmd->subCmds[0]->cmds[j + 1] = NULL;
                            cmd->subCmds[0]->subs = 0;
                        }
                    }
                    count++;
                    int front = i + 1;
                    for(int j = i + 1;j < cmd->arguments;j++)
                    {
                        if(strcmp("|", cmd->cmds[j]) == 0)
                        {
                            if(j == cmd->arguments - 1)
                            {
                                printf("analyze:wrong command\n");
                                cmd->subs = count;
                                return 1;
                            }
                            cmd->subCmds[count] = malloc(sizeof(CMD_S));
                            for(int k = front; k < j;k++)
                            {
                                cmd->subCmds[count]->cmds[k - front] = strdup(cmd->cmds[k]);
                                if(k == j - 1)
                                {
                                    cmd->subCmds[count]->arguments = k - front + 1;
                                    cmd->subCmds[count]->cmds[k - front + 1] = NULL;
                                    cmd->subCmds[count]->subs = 0;
                                }
                            }
                            count++;
                            front = j + 1;
                        }
                        if(j == cmd->arguments - 1)
                        {
                            cmd->subCmds[count] = malloc(sizeof(CMD_S));
                            for(int k = front; k <= j;k++)
                            {
                                cmd->subCmds[count]->cmds[k-front] = strdup(cmd->cmds[k]);
                                if(k == j)
                                {
                                    cmd->subCmds[count]->arguments = k - front + 1;
                                    cmd->subCmds[count]->cmds[k - front + 1] = NULL;
                                    cmd->subCmds[count]->subs = 0;
                                }
                            }
                            count++;
                            cmd->subs = count;
                            for(int j = 0;j < cmd->subs;j++)
                            {
                                _basicAnalyzeCmd(cmd->subCmds[j],InCmdList,ExCmdList);
                            }
                            return 0;
                        }
                    }
                }
            }
        }
    }
    else//not a pipe or redirect command
    {
        return -1;
    }
}

//3#
//This function is used to
//analyze basic command
//return as: 
//0 succeed 
//1 failed unknown command
int _basicAnalyzeCmd(CMD_S* cmd,CMD_S* InCmdList,CMD_S* ExCmdList)
{
    int check = -1;
    //internal
    for(int i = 0;i < InCmdList->arguments;i++)
    {
        check = strcmp(cmd->cmds[0],InCmdList->cmds[i]);
        if(check == 0)
        {
            cmd->type = 0;
            return 0;
        }
    }
    //external
    for(int i = 0;i < ExCmdList->arguments;i++)
    {
        check = strcmp(cmd->cmds[0],ExCmdList->cmds[i]);
        if(check == 0)
        {
            cmd->type = 1;
            return 0;
        }
    }
    //directory
    cmd->type = 2;
    return 0;
}

//4#
//This part are functions for executting commands
//return as:
//0 succeed 
//1 failed 
//execute command
int _execCmd(CMD_S *cmd)
{
    int k;
    switch (cmd->type)
    {
        case 0://internal
            k = _inExec(cmd);
            break;
        case 1://external
            k =  _exExec(cmd);
            break;
        case 2://excutable file
            k = _fileExec(cmd);
            break;
        case 3://pipe
            k = _pipeExec(cmd);
            break;
        case 4://redirect
            k = _reExec(cmd);
            break;
        default://failed
            k = 1;
            printf("exec:unknown type of command,check again\n");
            break;
    }
    return k;
}

//internal command
int _inExec(CMD_S* cmd)
{
    if(strcmp(cmd->cmds[0],"cd") == 0)
    {
        fake_cd(cmd->cmds[1]);
        return 0;
    }
    else if(strcmp(cmd->cmds[0],"help") == 0)
    {
        fake_help();
        return 0;
    }
    else if(strcmp(cmd->cmds[0],"exit") == 0)
    {
        printf("WCS-exit:calling for command \"exit\",end of program\n");
        exit(0);
    }
    return 0;
}

//external
int _exExec(CMD_S* cmd)
{
    int k = 0;
    pid_t p1;
    char path[1024] = "/home/candy/wakeCupShell/exCmd/";
    strcat(path,cmd->cmds[0]);
    while((p1 = fork()) == -1);
    if(p1 == 0)
    {
        k = execv(path,cmd->cmds);
        if(k == -1)//wrong input
        {
            printf("%s\n",path);
            printf("%s:command do not exist:check your command or enter \"help\" to get help\n",cmd->cmds[0]);
            exit(0);
        }
    }
    else
    {
        wait(0);
    }
    return 0;
}

//directory
int _fileExec(CMD_S* cmd)
{
    int k;
    pid_t p1;
    while((p1 = fork()) == -1);
    if(p1 == 0)
    {
        k = execv(cmd->cmds[0],cmd->cmds);
        if(k == -1)//wrong input
        {
            printf("command do not exist:check your command or enter \"help\"to get help\n");
            exit(0);
        }
    }
    else
    {
        wait(0);
    }
    return 0;
}

//redirect
int _reExec(CMD_S* cmd)
{
    int k;
    for(int i = 0;i < cmd->subCmds[1]->arguments;i++)
    {
        redirect_file(cmd->subCmds[1]->cmds[i]);
        k = (_execCmd(cmd->subCmds[0])&& k);
        redirect_stdout();
    }
    if(k == -1)
    {
        printf("redirect:execute failed,check again\n");
        return 1;
    }
    return 0;
}

//pipe
int _pipeExec(CMD_S* cmd)
{
    char tmp[2];
    int k = 0;
    int fd[_MAX_ARGUMENTS][2];
    for(int i = 1;i <= cmd->subs - 1;i++)
    {
        pipe(fd[i - 1]);//open pipe
        tmp[0] = fd[i - 1][0] - '0';
        tmp[1] = '\0';
        cmd->subCmds[i]->cmds[2] =strdup(tmp);//adapt for "grep"
    }

    //redirect to first pipe
    redirect_pipe(fd[0]);
    pid_t p1;
    while((p1 = fork()) == -1);
    if(p1 == 0)//child
    {
        k = _execCmd(cmd->subCmds[0]);//first command
        if(k == 1)
        {
            redirect_stdout();
            printf("execute:failed to execute a command,check command again\n");   
        }
        exit(0);
    }
    else//parent
    {
        wait(0);
        pid_t p2;
        while((p2 = fork()) == -1);
        if(p2 == 0)//child
        {
            if(cmd->subs > 2)
            {
                //execute commands
                for(int i = 1;i < cmd->subs - 1;i++)
                {
                    mto_redirect_pipe(fd[i]);
                    k = _execCmd(cmd->subCmds[i]);
                    if(k == 1)
                    {
                        redirect_stdout();
                        printf("execute:failed to execute a command,check command again\n"); 
                    }
                }
            }
            exit(0);
        }
        else//parent
        {
            wait(0);
            redirect_stdout();
            k = _execCmd(cmd->subCmds[cmd->subs - 1]);
            for(int i = 0;i < cmd->subs - 1;i++)
            {
                close(fd[i][0]);
                close(fd[i][1]);
            }
        }
    }
    return 0;
}

//to free a command
void _freeCmd(CMD_S* cmd)
{
    //free subcmds
    if(cmd->type > 2)
    {
        for(int i = 0; i < cmd->subs;i++)
        {
            for(int j = 0;j < cmd->subCmds[i]->arguments;j++)
            {
                cmd->subCmds[i]->cmds[j] = NULL;
            }
            cmd->subCmds[i]->arguments = 0;
            cmd->subCmds[i]->subs = 0;
            cmd->subCmds[i]->type = 0;
            free(cmd->subCmds[i]);
        }
    }
    //free itself
    for(int i = 0;i < cmd->arguments;i++)
    {
        cmd->cmds[i] = NULL;
    }
    cmd->arguments = 0;
    cmd->subs = 0;
    cmd->type = 0;
    free(cmd);
}

//for test
//print content of a command
void print_cmd(CMD_S* cmd)
{
    printf("| type:%d |\n",cmd->type);
    printf("| arguments:%d |\n",cmd->arguments);
    printf("| sbus:%d |\n",cmd->subs);
    printf("| cmd:");
    for(int i = 0;i < cmd->arguments;i++)
    {
        printf("%s ",cmd->cmds[i]);
    }
    printf("|\n");
    if(cmd->subs > 0)
    {
        
        for(int i = 0;i < cmd->subs;i++)
        {
            printf("| %d :subcmd%d:",cmd->subCmds[i]->arguments,i);
            for(int j = 0;j < cmd->subCmds[i]->arguments;j++)
            {
                printf("%s ",cmd->subCmds[i]->cmds[j]);
            }
            printf("|\n");
        }
    }
}