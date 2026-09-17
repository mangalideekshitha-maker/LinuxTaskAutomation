#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/builtin.h"

int execute_builtin(char **args)
{
    char cwd[1024];

    if (args[0] == NULL)
        return 1;

    /* exit */
    if (strcmp(args[0], "exit") == 0)
    {
        printf("Exiting Linux Task Automation Platform...\n");
        exit(EXIT_SUCCESS);
    }

    /* pwd */
    if (strcmp(args[0], "pwd") == 0)
    {
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 1;
    }

    /* cd */
    if (strcmp(args[0], "cd") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Usage : cd directory\n");
        }
        else
        {
            if (chdir(args[1]) != 0)
                perror("cd");
        }
        return 1;
    }

    /* clear */
    if (strcmp(args[0], "clear") == 0)
    {
        system("clear");
        return 1;
    }

    /* help */
    if (strcmp(args[0], "help") == 0)
    {
        printf("\nBuilt-in Commands\n");
        printf("---------------------------\n");
        printf("cd      - Change directory\n");
        printf("pwd     - Show current directory\n");
        printf("clear   - Clear terminal\n");
        printf("help    - Show available commands\n");
        printf("env     - Show environment variables\n");
        printf("list    - Show Linux commands\n");
        printf("exit    - Exit platform\n");
        return 1;
    }

    /* env */
    if (strcmp(args[0], "env") == 0)
    {
        printf("HOME = %s\n", getenv("HOME"));
        printf("USER = %s\n", getenv("USER"));
        printf("PATH = %s\n", getenv("PATH"));
        return 1;
    }

    /* list - existing Linux Task Automation feature */
    if (strcmp(args[0], "list") == 0)
    {
        printf("\nAvailable Linux Commands:\n");
        printf("1. ls      - List files\n");
        printf("2. pwd     - Show current directory\n");
        printf("3. date    - Show system date\n");
        printf("4. whoami  - Show current user\n");
        printf("5. free -h - Show memory information\n");
        printf("6. df -h   - Show disk usage\n");
        printf("7. exit    - Exit platform\n");
        return 1;
    }

    return 0;
}
