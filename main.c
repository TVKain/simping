#include "simping.h"
#include "simping_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <netinet/in.h>

struct simping_config *global_config;

int main(int argc, char *argv[]) {
    init_simping_config(&global_config, argc, argv);

    simping_start(global_config);
    free_simping_config(global_config);
    
    return 0;
}