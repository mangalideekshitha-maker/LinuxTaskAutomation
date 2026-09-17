#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/input.h"
#include "../include/parser.h"
#include "../include/process.h"
#include "../include/builtin.h"

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

        tokens = parse_line(input);

        if (tokens[0] == NULL)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }

        /*
         * Check whether the command is a built-in.
         * Built-in commands execute in the parent process.
         */
        if (execute_builtin(tokens) == 0)
        {
            /*
             * If it is not a built-in,
             * execute it as an external Linux command.
             */
            execute(tokens);
        }

        free_tokens(tokens);
        free(input);
    }

    return 0;
}
