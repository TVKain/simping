#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function that each thread will execute
void *count_to_ten(void *arg) {
    int core_id = *((int *)arg);

    // Set affinity to the specified core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
        exit(EXIT_FAILURE);
    }

    // Count from 1 to 10
    for (int i = 1; i <= 10; ++i) {
        printf("Thread on Core %d: %d\n", core_id, i);
        sleep(1);  // Sleep for better output visibility
    }

    return NULL;
}

int main() {
    pthread_t threads[20];
    int core_ids[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};  // Assuming you have at least 4 cores

    for (int i = 0; i < 20; ++i) {
        // Create threads, passing the core ID as an argument
        if (pthread_create(&threads[i], NULL, count_to_ten, &core_ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < 20; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
