
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
static int prevent_deadlock = 0;

static void *worker1(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&lock1);
    printf("Thread 1: acquired Lock 1\n");

    sleep(1);

    printf("Thread 1: waiting for Lock 2\n");
    pthread_mutex_lock(&lock2);
    printf("Thread 1: acquired Lock 2\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);

    return NULL;
}

static void *worker2(void *arg)
{
    (void)arg;

    if (prevent_deadlock)
    {
        /* Prevention: acquire locks in the same order. */
        pthread_mutex_lock(&lock1);
        printf("Thread 2: acquired Lock 1\n");

        sleep(1);

        printf("Thread 2: waiting for Lock 2\n");
        pthread_mutex_lock(&lock2);
        printf("Thread 2: acquired Lock 2\n");

        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
    }
    else
    {
        /* Demonstration: acquire locks in the opposite order. */
        pthread_mutex_lock(&lock2);
        printf("Thread 2: acquired Lock 2\n");

        sleep(1);

        printf("Thread 2: waiting for Lock 1\n");
        pthread_mutex_lock(&lock1);
        printf("Thread 2: acquired Lock 1\n");

        pthread_mutex_unlock(&lock1);
        pthread_mutex_unlock(&lock2);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread1;
    pthread_t thread2;

    if (argc > 2 ||
        (argc == 2 && strcmp(argv[1], "prevent") != 0))
    {
        fprintf(stderr, "Usage: %s [prevent]\n", argv[0]);
        return EXIT_FAILURE;
    }

    prevent_deadlock = (argc == 2);

    setvbuf(stdout, NULL, _IONBF, 0);

    if (prevent_deadlock)
        printf("Deadlock Prevention Demonstration\n");
    else
        printf("Deadlock Demonstration\n");

    if (pthread_create(&thread1, NULL, worker1, NULL) != 0)
    {
        fprintf(stderr, "Failed to create Thread 1\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&thread2, NULL, worker2, NULL) != 0)
    {
        fprintf(stderr, "Failed to create Thread 2\n");
        pthread_join(thread1, NULL);
        return EXIT_FAILURE;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    printf("Both threads completed successfully.\n");
    return EXIT_SUCCESS;
}

