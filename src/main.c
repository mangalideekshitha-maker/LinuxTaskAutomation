#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/input.h"
#include "../include/parser.h"
#include "../include/process.h"

int main()
{
    char *input;
    char **tokens;

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

        if (strcmp(tokens[0], "list") == 0)
        {
            printf("\nAvailable Linux Commands:\n");
            printf("1. ls      - List files\n");
            printf("2. pwd     - Show current directory\n");
            printf("3. date    - Show system date\n");
            printf("4. whoami  - Show current user\n");
            printf("5. free -h - Show memory information\n");
            printf("6. df -h   - Show disk usage\n");
            printf("7. exit    - Exit platform\n");
        }
        else
        {
            execute(tokens);
        }

        free_tokens(tokens);
        free(input);
    }

    printf("Goodbye!\n");

    return 0;
}
