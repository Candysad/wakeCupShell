//WARNING:function strtok()  always use a same RAM space 
//last result of this function will be changed after using
//ONLY use once in cmdStr.h to get command form stdin  

//standard C head file for project wakeCupShell
#ifndef WCS_std
#include "WCSstd.h"
#endif

//Some Macros
#define _MAX_ARGUMENTS 32
#define _MAX_BUFF_LENGTH 1024
#define _MAX_CMD_LENGTH 20

//structure of command
typedef struct cmd_s
{
    int type;//0:internal command; 1:external command; 2:executable file
    int arguments;//amount of arguments
    char *cmds[_MAX_ARGUMENTS];//command with arguments
}CMD_S;

//function list
int _getCmd(CMD_S *cmd);
int _analyzeCmd(CMD_S *cmd,CMD_S* InCmdList,CMD_S* ExCmdList);

//interface for command

//1#
//This function is used to
//get a command from stdin, put it into the structure cmd_S
//if succeed return an integer > 0
//else if falied return 0
int _getCmd(CMD_S *cmd)
{
    char buff[_MAX_BUFF_LENGTH];
    fgets(buff,sizeof(buff)-1,stdin);
    buff[strlen(buff) - 1] = '\0';//the final element of buff from fgets() would be '\n'
                                  //change to '\0' for ending it

    cmd->cmds[0]=strtok(buff," ");
    int i = 0;
    while(cmd->cmds[i]!=NULL)
    {
        i++;
        cmd->cmds[i]=strtok(NULL," ");
    }
    cmd->arguments= i;
    return i;
}   

//2#
//InCmdList and ExCmdList are structures of CMD_S declared during the Initiating process
//return as: 
//0 internal command
//1 external command
//2 file path
//3 failed to find executable file 
int _analyzeCmd(CMD_S *cmd,CMD_S* InCmdList,CMD_S* ExCmdList)
{
    int s = 1;
    int i;
    for(i = 0;i < InCmdList->arguments;i++)//matching internal command
    {
        s=strcmp(cmd->cmds[0],InCmdList->cmds[i]);
        if(s==0)
        {
            cmd->type=0;
            s = 0;
            return s;
        }
    }
    for(i = 0;i < ExCmdList->arguments;i++)//matching external command
    {
        s=strcmp(cmd->cmds[0],ExCmdList->cmds[i]);
        if(s==0)
        {
            cmd->type=1;
            s = 1;
            return s;
        }
    }
    if(s!=0)
    {
        cmd->arguments=access(cmd->cmds[0],X_OK);//looking for executable file
        printf("%d\n",cmd->arguments);
        if(cmd->arguments==0)
        {
            s = 2;
        }
        else
        {
            s = 3;
        }
        cmd->type = s;
        return s;
    }
    return s;
}