//This file includes functions for all internal commands

//1# cd
int fake_cd(char* path)
{
    int k = 0;
    k = chdir(path);
    if(k != 0)
    {
        printf("WCS-cd:change current directory failed,check the path\n");
    }
    return 0;
}

//2# help
int fake_help()//this function comes from the early-shell(fakeshell)
{
    char name[20];
    printf("help for _mysh\n");//
    
    for(;;)
    {
        printf("\nenter name of command to see usage\n");
        printf("enter\033[32m L \033[0mto see command list\n");
        printf("enter\033[32m Q \033[0mto leave help page\n");
        printf("please enter:");
        fgets(name,sizeof(name),stdin);
        name[strlen(name)-1] = '\0';
        if(strcmp("Q",name) == 0)
        {
            return 0;
        }
        else if(strcmp("L",name) == 0)
        {
            printf("\033[32mcat\ncd\necho\nexit\ngrep\nhelp\nls\nmkdir\nmv\npwd\nrm\nwc\n\"|\" for pipe\n\">\" for redirect\033[0m");
        }
        else if(strcmp("cat",name) == 0)
        {
            printf("\033[32mcat: to see content of a file\nSYNOPSIS:\033[0m cat a.c\n");
        }
        else if(strcmp("cd",name) == 0)
        {
            printf("\033[32mcd: to change current working directory of present process\nSYNOPSIS:\033[0m cd /home\n");
        }
        else if(strcmp("echo",name) == 0)
        {
            printf("\033[32mecho: printf something as you entered,like an echo\nSYNOPSIS:\033[0m echo hello!\n");
        }
         else if(strcmp("exit",name) == 0)
        {
            printf("\033[32mexit: exit this shell\nSYNOPSIS:\033[0m exit\n");
        }
        else if(strcmp("help",name) == 0)
        {
            printf("\033[32mhelp: to see help for this shell,like what you seeing now\nSYNOPSIS:\033[0m help\n");
        }
        else if(strcmp("ls",name) == 0)
        {
            printf("\033[32mls: to see list of content for a directory\nSYNOPSIS:\033[0m ls /home\n");
        }
        else if(strcmp("pwd",name) == 0)
        {
            printf("\033[32mpwd: to see current working directory of present process\nSYNOPSIS:\033[0m pwd\n");
        }
        else if(strcmp("mkdir",name) == 0)
        {
            printf("\033[32mmkdir: to make a new directory \nSYNOPSIS:\033[0m mkdir test | mkdir ../test\n");
        }
        else if(strcmp("mv",name) == 0)
        {
            printf("\033[32mmv: move a file or empty directory to a new path or change the name of it\nSYNOPSIS:\033[0m mv a.c b.c | mv a.c ../\n");
        }
        else if(strcmp("rm",name) == 0)
        {
            printf("\033[32mrm: remove a file or empty directory\nSYNOPSIS:\033[0m rm a\n");
        }
        else if(strcmp("wc",name) == 0)
        {
            printf("\033[32mwc: to count characters of a file\nSYNOPSIS:\033[0m wc a.c\n");
        }
        else if(strcmp("|",name) == 0)
        {
            printf("\033[32m\"|\" pipe: open a pipe for two commands\n\"|\" should be separated by blank\nSYNOPSIS:\033[0m cat a.c | grep main\n");
        }
        else if(strcmp(">",name) == 0)
        {
            printf("\033[32m\">\" redirect:redirect to change output of a command ro a file\n\">\" should be separated by blank\nSYNOPSIS:\033[0m cat a.c > test\n");
        }
        else
        {
            printf("no command:check again\n");
        }

    }
    return 0;
}