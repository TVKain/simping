#ifndef SIMPING_CONFIG_H
#define SIMPING_CONFIG_H

#include <stdint.h>

#define DEFAULT_BUFF_SIZE 64

struct simping_config {
    uint16_t buff_size; 
    char *target_ip;
    uint16_t delay;
    int packet_count;
    int logging;
};

void init_simping_config(struct simping_config **, int argc, char *argv[]);
void dbg_simping_config(struct simping_config *config);
void free_simping_config(struct simping_config *config);

#endif