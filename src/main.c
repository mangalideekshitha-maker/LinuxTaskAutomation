#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"
#include "../include/parser.h"
#include "../include/redirect.h"
#include "../include/process.h"
#include "../include/builtin.h"
#include "../include/signals.h"
#include "../include/pipes.h"

static void tokenize_command(char *str, char **argv)
{
    int i = 0;
    char *token = strtok(str, " \t\r\n");

    while (token != NULL && i < 63)
    {
        argv[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    argv[i] = NULL;
}

int main()
{
    char *input;
    char **tokens;

    initialize_signals();

    printf("=====================================\n");
    printf("Linux Task Automation Platform\n");
    printf("=====================================\n");

    while (1)
    {
        printf("task> ");

        input = read_line();

        /*
         * Check for pipe command
         */
        if (strchr(input, '|') != NULL)
        {
            char *left;
            char *right;
            char *argv1[64];
            char *argv2[64];

            left = strtok(input, "|");
            right = strtok(NULL, "|");

            if (left == NULL || right == NULL)
            {
                printf("Invalid pipe command\n");
                free(input);
                continue;
            }

            tokenize_command(left, argv1);
            tokenize_command(right, argv2);

            if (argv1[0] == NULL || argv2[0] == NULL)
            {
                printf("Invalid pipe command\n");
                free(input);
                continue;
            }

            execute_pipe(argv1, argv2);

            free(input);
            continue;
        }

        /*
         * Normal command processing
         */
        tokens = parse_line(input);

        if (tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }

        /*
         * Built-in commands
         */
        if (execute_builtin(tokens) == 0)
        {
            /*
             * I/O Redirection
             */
            if (execute_redirection(tokens) == 0)
            {
                /*
                 * External Linux commands
                 */
                execute(tokens);
            }
        }

        free_tokens(tokens);
        free(input);
    }

    return 0;
}
