#include <stdio.h>
#include <signal.h>

#include "../include/signals.h"

static void sigint_handler(int sig)
{
    (void)sig;
    printf("\nLinuxTaskAutomation: Press 'exit' to quit.\n");
    fflush(stdout);
}

void initialize_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}
