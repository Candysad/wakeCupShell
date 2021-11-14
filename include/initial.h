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
    char cwd[PATH_MAX];//working path
    char hostname[128];
    struct passwd* user;//user data
    CMD_S* InCmdList;
    CMD_S* ExCmdList;
}STAT_S;


//function list
int _WCSinit(STAT_S* situation,CMD_S* cmd_0);
int _getInCmdList(CMD_S* InCmdList);
int _getExCmdList(CMD_S* ExCmdList);
int _reInitCmd(CMD_S* cmd_0);
int _reInfo(STAT_S* situation,char* informations);

//1#
//This is the function for initiate basic situation settings
//return as:
//0 succueed
//1 failed
int _WCSinit(STAT_S* situation,CMD_S* cmd_0)
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
    k = _getInCmdList(situation->InCmdList);
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
    k = _getExCmdList(situation->ExCmdList);
     if(k==0)
    {
        k = 1;
        perror("get external command list failed");
        return k;
    }
    else printf("get external command list succeed\n");
    
    //get current working directory
    getcwd(situation->cwd,PATH_MAX);

    //get hostname
    gethostname(situation->hostname,sizeof(situation->hostname));

    //set cmd_0
    _reInitCmd(cmd_0);

    //get commands history
    read_history("/home/candy/wakeCupShell/.wake_history");

    //print basic information
    system("clear");//
    printf("welcome :\033[32m %s\033[0m\n",situation->user->pw_name);
    printf("host :\033[32m %s\033[0m\n",situation->hostname);
    printf("This is \033[31mwakeCupShell\033[0m:a fake shell project\n");
    printf("current working directory:\033[32m%s\033[0m\n",situation->cwd);

}

int _getInCmdList(CMD_S* InCmdList)
{
    int k = 0;
    int i = 0;
    char incmd[_MAX_CMD_BUFF];
    FILE* inFile;
    if((inFile = fopen("./cmdList/inList","r")) == NULL)//open internal commands list file
    {
        perror("open inList failed");
        return 1;
    }
    fgets(incmd,_MAX_CMD_BUFF,inFile);
    while(strcmp(incmd,"$") != 0)
    {
        incmd[strlen(incmd)-1] = '\0';//change '\n' to '\0'
        InCmdList->cmds[i] = strdup(incmd);
        i++;
        fgets(incmd,_MAX_CMD_BUFF,inFile);
    }
    InCmdList->arguments = i;
    return i;
}

int _getExCmdList(CMD_S* ExCmdList)
{
    int k = 0;
    int i = 0;
    char excmd[_MAX_CMD_BUFF];
    FILE* exFile;
    if((exFile = fopen("./cmdList/exList","r"))==NULL)//open external commands list file
    {
        perror("open exList failed");
        return 1;
    }
    fgets(excmd,_MAX_CMD_BUFF,exFile);
    while(strcmp(excmd,"$") != 0)
    {
        excmd[strlen(excmd)-1] = '\0';//change '\n' to '\0'
        ExCmdList->cmds[i] = strdup(excmd);
        i++;
        fgets(excmd,_MAX_CMD_BUFF,exFile);
    }
    ExCmdList->arguments = i;
    return i;
}

int _reInitCmd(CMD_S* cmd_0)
{
    cmd_0->arguments = 0;
    cmd_0->subs = 0;
    cmd_0->type = 0;
    cmd_0->cmds[0] = NULL;
    cmd_0->subCmds[0] = NULL;
    return 0;
}

int _reInfo(STAT_S* situation,char* informations)
{
    informations[0] = '\0';
    strcat(informations,"\001\033[31m\002");
    strcat(informations,situation->user->pw_name);
    strcat(informations,"@");
    strcat(informations,situation->hostname);
    strcat(informations,"\001\033[0m:\033[032m\002");
    strcat(informations,situation->cwd);
    strcat(informations,"\001\033[0m$\002");
}