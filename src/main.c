#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char input[1024];

    printf("========================================\n");
    printf("     Linux Task Automation Platform\n");
    printf("========================================\n");

    while (1)
    {
        printf("task> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Linux Task Automation Platform...\n");
            break;
        }

        if (strcmp(input, "list") == 0)
        {
            printf("\nAvailable Tasks:\n");
            printf("1. files  - List files\n");
            printf("2. pwd    - Show current directory\n");
            printf("3. date   - Show system date\n");
            printf("4. memory - Show memory information\n");
            printf("5. disk   - Show disk usage\n");
            printf("6. exit   - Exit platform\n\n");
        }
        else if (strcmp(input, "files") == 0)
        {
            system("ls");
        }
        else if (strcmp(input, "pwd") == 0)
        {
            system("pwd");
        }
        else if (strcmp(input, "date") == 0)
        {
            system("date");
        }
        else if (strcmp(input, "memory") == 0)
        {
            system("free -h");
        }
        else if (strcmp(input, "disk") == 0)
        {
            system("df -h");
        }
        else
        {
            printf("Unknown task: %s\n", input);
            printf("Type 'list' to see available tasks.\n");
        }
    }

    return 0;
}
