#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/signals.h"

static void sigint_handler(int sig)
{
    (void)sig;

    printf("\nLinuxTaskAutomation: Press 'exit' to quit.\n");
    fflush(stdout);
}

static void sigchld_handler(int sig)
{
    (void)sig;

    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }
}

void initialize_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, sigchld_handler);
}
