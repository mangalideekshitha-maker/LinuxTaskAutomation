#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/input.h"
#include "../include/parser.h"

int main()
{
    char *input;
    char **tokens;
    int i;

    printf("=====================================\n");
    printf("Linux Task Automation Platform\n");
    printf("=====================================\n");

    while (1)
    {
        printf("task> ");

        input = read_line();

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting Linux Task Automation Platform...\n");
            free(input);
            break;
        }

        tokens = parse_line(input);

        if (tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }

        printf("\nParsed Tokens\n");

        for (i = 0; tokens[i] != NULL; i++)
        {
            printf("argv[%d] = %s\n", i, tokens[i]);
        }

        if (strcmp(tokens[0], "list") == 0)
        {
            printf("\nAvailable Tasks:\n");
            printf("1. files   - List files\n");
            printf("2. pwd     - Show current directory\n");
            printf("3. date    - Show system date\n");
            printf("4. memory  - Show memory information\n");
            printf("5. disk    - Show disk usage\n");
            printf("6. exit    - Exit platform\n");
        }
        else if (strcmp(tokens[0], "files") == 0)
        {
            system("ls");
        }
        else if (strcmp(tokens[0], "pwd") == 0)
        {
            system("pwd");
        }
        else if (strcmp(tokens[0], "date") == 0)
        {
            system("date");
        }
        else if (strcmp(tokens[0], "memory") == 0)
        {
            system("free -h");
        }
        else if (strcmp(tokens[0], "disk") == 0)
        {
            system("df -h");
        }
        else
        {
            printf("\nUnknown task: %s\n", tokens[0]);
            printf("Type 'list' to see available tasks.\n");
        }

        free_tokens(tokens);
        free(input);
    }

    printf("Goodbye!\n");

    return 0;
}
