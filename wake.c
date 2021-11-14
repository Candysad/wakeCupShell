#include "include/WCSstd.h"
#include "include/initial.h"

int main()
{
    STAT_S* situation = malloc(sizeof(STAT_S));
    CMD_S* cmd_0 = malloc(sizeof(STAT_S));
    int sig;
    char informations[1024];

    //init
    _WCSinit(situation,cmd_0);

    for(;;)
    {
        //reset
        //reset signal
        sig = 0;
        
        //reset current woeking directory
        getcwd(situation->cwd,PATH_MAX);
        
        //reset informations for readline
        _reInfo(situation,informations);
        
        //reset cmd_0
        _freeCmd(cmd_0);
        cmd_0 = malloc(sizeof(CMD_S));
        _reInitCmd(cmd_0);
        
        //get a command
        sig = _getCmd(cmd_0,informations);

        //analyze command
        if(sig == 0)
        {
            continue;
        }
        sig = _piReAnalyzeCmd(cmd_0,situation->InCmdList,situation->ExCmdList);
        if(sig == -1)
        {
            _basicAnalyzeCmd(cmd_0,situation->InCmdList,situation->ExCmdList);
        }
        else if(sig == 1)
        {
            continue;
        }

        //test
        //print_cmd(cmd_0);
        
        //execute command
        _execCmd(cmd_0);
    }

    return 0;
}