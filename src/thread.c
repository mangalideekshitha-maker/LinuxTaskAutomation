#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *monitor(void *arg)
{
    (void)arg;

    while (1)
    {
        sleep(10);

        printf("\n[Monitor] Linux Task Automation Platform Running...\n");
        fflush(stdout);
    }

    return NULL;
}

void start_monitor_thread(void)
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, monitor, NULL) != 0)
    {
        perror("pthread_create");
        return;
    }

    pthread_detach(tid);
}
