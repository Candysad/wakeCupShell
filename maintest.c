#include "include/WCSstd.h"
#include "include/initial.h"

int main()
{
    STAT_S* situation = malloc(sizeof(STAT_S));
    CMD_S* cmd_0 = malloc(sizeof(CMD_S));

    //init
    WCSinit(situation);

    for(int i = 0;i < situation->InCmdList->arguments;i++)
    {
        printf("%s\n",situation->InCmdList->cmds[i]);
    }
    printf("print incmd over %d\n",situation->InCmdList->arguments);
    for(int i = 0;i < situation->ExCmdList->arguments;i++)
    {
        printf("%s\n",situation->ExCmdList->cmds[i]);
    }
    printf("print excmd over %d\n",situation->ExCmdList->arguments);

    _getCmd(cmd_0);
    printf("%d\n",cmd_0->arguments);
    for(int i = 0;i < cmd_0->arguments;i++)
    {
        printf("%s\n",cmd_0->cmds[i]);
    }
    printf("%d\n",_analyzeCmd(cmd_0,situation->InCmdList,situation->ExCmdList));
    
    return 0;
}