//WARNING:function strtok()  always use a same RAM space 
//last result of this function will be changed after using
//ONLY use once in cmdStr.h to get command form stdin  

//standard C head file for project wakeCupShell
#ifndef WCS_std
#include "WCSstd.h"
#endif

//basic interface for WCS programing
#ifndef wcs
#include "WCS.h"
#endif

//macro
#define _MAX_CMD_BUFF  1024

//structure of situation
typedef struct situation
{
    char* Wpath;//working path
    struct passwd* user;
    CMD_S* InCmdList;
    CMD_S* ExCmdList;

}STAT_S;

//function list
int WCSinit(STAT_S* situation);
int getInCmdList(CMD_S* InCmdList);
int getExCmdList(CMD_S* ExCmdList);

//1#
//This is the function for initiate basic situation settings
//return as:
//0 succueed
//1 failed
int WCSinit(STAT_S* situation)
{
    int k = 0;
    situation->ExCmdList = malloc(sizeof(CMD_S));
    situation->InCmdList = malloc(sizeof(CMD_S));
    //get user data
    printf("getting user date\n");
    situation->user = getpwuid(getuid());
    if(situation->user == NULL)
    {
        k = 1;
        perror("get user date failed");
        return k;
    }
    else printf("get user data succeed\n");

    //get internal commands
    printf("getting InCmdList\n");
    situation->InCmdList->type = 0;
    k = getInCmdList(situation->InCmdList);
    if(k==0)
    {
        k = 1;
        perror("get internal command list failed");
        return k;
    }
    else printf("get internal command list succeed\n");

    //get external commands
    printf("getting external command list\n");
    situation->ExCmdList->type = 1;
    k = getExCmdList(situation->ExCmdList);
     if(k==0)
    {
        k = 1;
        perror("get external command list failed");
        return k;
    }
    else printf("get external command list succeed\n");
    
    //get working path


}

int getInCmdList(CMD_S* InCmdList)//修改文件内容格式,用strtok分割$
{
    int k = 0;
    int i = 0;
    char incmd[_MAX_CMD_BUFF];
    FILE* inFile;
    if((inFile = fopen("./cmdList/inList","r")) == NULL)//open internal commands list file
    {
        perror("open inList failed");
        return k;
    }
    fgets(incmd,_MAX_CMD_BUFF,inFile);
    InCmdList->cmds[0] = strtok(incmd,"$");
    while(InCmdList->cmds[i] != NULL)
    {
        i++;
        InCmdList->cmds[i] = strtok(NULL,"$");
    }
    InCmdList->arguments = i;
    return i;
}

int getExCmdList(CMD_S* ExCmdList)
{
    int k = 0;
    int i = 0;
    char excmd[_MAX_CMD_BUFF];
    FILE* exFile;
    if((exFile = fopen("./cmdList/exList","r"))==NULL)//open external commands list file
    {
        perror("open exList failed");
        return k;
    }
    fgets(excmd,_MAX_CMD_BUFF,exFile);
    ExCmdList->cmds[0] = strtok(excmd,"$");
    while(ExCmdList->cmds[i] != NULL)
    {
        i++;
        ExCmdList->cmds[i] = strtok(NULL,"$");
    }
    ExCmdList->arguments = i;
    return i;
}