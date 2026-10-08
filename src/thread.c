#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
 * Background monitoring thread
 */
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

/*
 * Start the background monitoring thread
 */
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

/*
 * Shared counter for synchronization demonstration
 */
static int shared_counter = 0;
static pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Worker thread demonstrating mutex synchronization
 */
void *counter_worker(void *arg)
{
    (void)arg;

    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&counter_mutex);

        shared_counter++;

        pthread_mutex_unlock(&counter_mutex);
    }

    return NULL;
}

/*
 * Demonstrates pthread_create(), pthread_join(),
 * mutex synchronization and protection from race conditions.
 */
void run_thread_demo(void)
{
    pthread_t thread1;
    pthread_t thread2;

    shared_counter = 0;

    printf("\n[Thread Demo] Starting two worker threads...\n");

    if (pthread_create(&thread1, NULL, counter_worker, NULL) != 0)
    {
        perror("pthread_create");
        return;
    }

    if (pthread_create(&thread2, NULL, counter_worker, NULL) != 0)
    {
        perror("pthread_create");
        pthread_join(thread1, NULL);
        return;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("[Thread Demo] Final shared counter: %d\n", shared_counter);
    printf("[Thread Demo] pthread_join() completed successfully.\n");
    printf("[Thread Demo] Mutex synchronization protected the critical section.\n");
}
