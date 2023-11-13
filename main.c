#include "simping.h"
#include "simping_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <signal.h>

#include <time.h>

#include <netinet/in.h>

struct simping_config *global_config;

double start, end;

void log_end() {
    end = (double)clock()/CLOCKS_PER_SEC;
    printf("\nTotal sent attempt: %u time(s)\n", global_config->sent_attempt);
    printf("Number of fail attemps: %u\n", global_config->sent_attempt - global_config->packet_sent);
    printf("Successfully sent: %u packages\n", global_config->packet_sent);
    printf("Time elapsed: %lf second(s)\n", end - start);

    double avg_packets = ((double) global_config->packet_sent) / (end - start);

    printf("Average: %u packages/second\n", (uint32_t)avg_packets);
    free_simping_config(global_config);
    exit(EXIT_SUCCESS);
}

void signal_handler(int sig_num) {
    if (sig_num == SIGINT) {
        log_end();
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signal_handler);

    init_simping_config(&global_config, argc, argv);


    start = (double)clock()/CLOCKS_PER_SEC;
    simping_start(global_config);
    end = (double)clock()/CLOCKS_PER_SEC;
    
    log_end();
    
    return 0;
}