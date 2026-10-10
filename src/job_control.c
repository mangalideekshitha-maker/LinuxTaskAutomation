
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <termios.h>

#include "../include/job_control.h"

#define MAX_JOBS 64

typedef struct {
    int id;
    pid_t pid;
    int used;
    int stopped;
    char command[256];
} Job;

static Job jobs[MAX_JOBS];
static int next_job_id = 1;
static pid_t shell_pgid;
static int terminal_control = 0;

static Job *find_job(int id)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].used && jobs[i].id == id)
            return &jobs[i];
    }

    return NULL;
}

static Job *find_latest_job(void)
{
    Job *latest = NULL;

    for (int i = 0; i < MAX_JOBS; i++) {
        if (jobs[i].used &&
            (latest == NULL || jobs[i].id > latest->id))
            latest = &jobs[i];
    }

    return latest;
}

static Job *add_job(pid_t pid, const char *command, int stopped)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].used) {
            jobs[i].id = next_job_id++;
            jobs[i].pid = pid;
            jobs[i].used = 1;
            jobs[i].stopped = stopped;

            snprintf(jobs[i].command,
                     sizeof(jobs[i].command), "%s", command);

            return &jobs[i];
        }
    }

    fprintf(stderr, "Job table is full.\n");
    return NULL;
}

static void reap_background_jobs(void)
{
    for (int i = 0; i < MAX_JOBS; i++) {
        if (!jobs[i].used)
            continue;

        int status;
        pid_t result = waitpid(jobs[i].pid, &status,
                               WNOHANG | WUNTRACED | WCONTINUED);

        if (result <= 0)
            continue;

        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            printf("[%d] Done: %s\n",
                   jobs[i].id, jobs[i].command);
            jobs[i].used = 0;
        } else if (WIFSTOPPED(status)) {
            jobs[i].stopped = 1;
        } else if (WIFCONTINUED(status)) {
            jobs[i].stopped = 0;
        }
    }
}

void job_control_init(void)
{
    shell_pgid = getpgrp();

    if (isatty(STDIN_FILENO) &&
        tcgetpgrp(STDIN_FILENO) == shell_pgid) {
        terminal_control = 1;
    }

    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

static void wait_for_foreground(Job *job)
{
    int status;
    pid_t result;

    if (terminal_control)
        tcsetpgrp(STDIN_FILENO, job->pid);

    do {
        result = waitpid(job->pid, &status, WUNTRACED);
    } while (result < 0 && errno == EINTR);

    if (terminal_control)
        tcsetpgrp(STDIN_FILENO, shell_pgid);

    if (result < 0) {
        perror("waitpid");
        job->used = 0;
        return;
    }

    if (WIFSTOPPED(status)) {
        int stored = 0;

        for (int i = 0; i < MAX_JOBS; i++) {
            if (job == &jobs[i]) {
                stored = 1;
                break;
            }
        }

        if (!stored) {
            Job *saved = add_job(job->pid, job->command, 1);

            if (saved != NULL) {
                printf("\n[%d] Stopped: %s\n",
                       saved->id, saved->command);
            }
        } else {
            job->stopped = 1;
            printf("\n[%d] Stopped: %s\n",
                   job->id, job->command);
        }
    } else {
        job->used = 0;
    }
}

static int parse_job_id(const char *arg, int *id)
{
    if (arg == NULL)
        return 0;

    if (*arg == '%')
        arg++;

    char *end;
    long value = strtol(arg, &end, 10);

    if (*arg == '\0' || *end != '\0' ||
        value <= 0 || value > 2147483647L)
        return -1;

    *id = (int)value;
    return 1;
}

int job_control_handle(char **args)
{
    if (args == NULL || args[0] == NULL)
        return 0;

    reap_background_jobs();

    if (strcmp(args[0], "jobs") == 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (jobs[i].used) {
                printf("[%d] %s  %s\n",
                       jobs[i].id,
                       jobs[i].stopped ? "Stopped" : "Running",
                       jobs[i].command);
            }
        }
        return 1;
    }

    if (strcmp(args[0], "fg") != 0 &&
        strcmp(args[0], "bg") != 0)
        return 0;

    int id = 0;
    int parsed = parse_job_id(args[1], &id);

    if (parsed < 0) {
        fprintf(stderr, "Usage: %s [%%job_id]\n", args[0]);
        return 1;
    }

    Job *job = parsed == 0
        ? find_latest_job()
        : find_job(id);

    if (job == NULL) {
        fprintf(stderr, "No such job.\n");
        return 1;
    }

    if (strcmp(args[0], "bg") == 0) {
        if (kill(-job->pid, SIGCONT) < 0) {
            perror("bg");
            return 1;
        }

        job->stopped = 0;
        printf("[%d] %s &\n", job->id, job->command);
        return 1;
    }

    if (terminal_control)
        tcsetpgrp(STDIN_FILENO, job->pid);

    if (kill(-job->pid, SIGCONT) < 0) {
        if (terminal_control)
            tcsetpgrp(STDIN_FILENO, shell_pgid);

        perror("fg");
        return 1;
    }

    job->stopped = 0;
    wait_for_foreground(job);
    return 1;
}

int job_control_execute(char **args)
{
    if (args == NULL || args[0] == NULL)
        return 1;

    char *argv[64];
    int count = 0;

    while (args[count] != NULL && count < 63) {
        argv[count] = args[count];
        count++;
    }

    int background = 0;

    if (count > 0 && strcmp(argv[count - 1], "&") == 0) {
        background = 1;
        count--;
    }

    if (count == 0)
        return 1;

    argv[count] = NULL;

    char command[256] = "";

    for (int i = 0; i < count; i++) {
        size_t used = strlen(command);
        size_t remaining = sizeof(command) - used;

        if (remaining <= 1)
            break;

        snprintf(command + used, remaining, "%s%s",
                 i == 0 ? "" : " ", argv[i]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        setpgid(0, 0);

        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);

        execvp(argv[0], argv);
        perror(argv[0]);
        _exit(127);
    }

    setpgid(pid, pid);

    Job temporary = {
        .id = next_job_id,
        .pid = pid,
        .used = 1,
        .stopped = 0
    };

    snprintf(temporary.command,
             sizeof(temporary.command), "%s", command);

    if (background) {
        Job *job = add_job(pid, command, 0);

        if (job != NULL)
            printf("[%d] %d\n", job->id, (int)pid);
        else
            wait_for_foreground(&temporary);

        return 1;
    }

    wait_for_foreground(&temporary);
    return 1;
}
