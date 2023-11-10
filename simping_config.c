#include "simping_config.h"

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int is_valid_ipv4(const char *ip) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip, &(sa.sin_addr));
}

void print_help(char *prog_name) {
    printf("Usage: %s [-s <size>] [-d <delay>] [-c <count>] <-t <ip_address>>\n", prog_name);
    printf("Options:\n");
    printf("    -t, --target <ip_address>       Specify the target address to send the packet\n");
    printf("    -s, --size <size>               Specify the size (in bytes) of the packet to be sent\n");
    printf("                                    If not specify default size is 64 bytes\n");
    printf("    -d, --delay <delay>             Specify the delay (in milisecond) between each packet\n");
    printf("                                    If not specify send without delay\n");
    printf("    -c, --count <packet_count>      Specify the number of packets to be sent\n");
    printf("                                    If not specify send until receive interrupt\n");
    printf("    -l, --logging                   If not specify does not log anything\n");
    printf("    -h, --help                      Display this help message\n");
}

void init_simping_config(struct simping_config **current_config, int argc, char *argv[]) {
    int opt;

    char *target_ip = NULL;     
    uint16_t buff_size = DEFAULT_BUFF_SIZE;
    uint16_t delay = 0;
    int packet_count = 0;
    int logging = 0;

    struct option long_options[] = {
        {"size", optional_argument, NULL, 's'},
        {"target", required_argument, NULL, 't'},
        {"delay", optional_argument, NULL, 'd'}, 
        {"count", optional_argument, NULL, 'c'},
        {"logging", no_argument, NULL, 'l'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "s:t:d:c:lh", long_options, NULL)) != -1) {
        switch (opt) {
            case 's': 
                buff_size = strtoul(optarg, NULL, 10);
                if (buff_size < 8) {
                    fprintf(stderr, "Error: Invalid packet size\n");
                    fprintf(stderr, "       Size must be greater than 8 (bytes)\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 't':
                if (!is_valid_ipv4(optarg)) {
                    fprintf(stderr, "Error: Invalid IPv4 address\n");
                    fprintf(stderr, "       %s is not a valid IPv4 address\n", optarg);
                    exit(EXIT_FAILURE);
                }
                target_ip = strdup(optarg);
                break;
            case 'd': 
                delay = strtoul(optarg, NULL, 10);
                if (delay <= 0) {
                    fprintf(stderr, "Error: Invalid delay size\n");
                    fprintf(stderr, "       Delay must be greater than 0 (ms)\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'c': 
                packet_count = strtoul(optarg, NULL, 10);
                if (packet_count <= 0) {
                    fprintf(stderr, "Error: Invalid packet count\n");
                    fprintf(stderr, "       Packet count must be greater than 0\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':
                logging = 1;
                break;
            case 'h': 
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            default: 
                fprintf(stderr, "Use '%s -h' to see usage.\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (target_ip == NULL) {
        fprintf(stderr, "Missing required argument: '--target'\n");
        fprintf(stderr, "Use '%s -h' to see usage.\n", argv[0]);
        exit(EXIT_SUCCESS);
    }

    *current_config = malloc(sizeof (struct simping_config));
    (*current_config)->target_ip = target_ip;
    (*current_config)->buff_size = buff_size;
    (*current_config)->delay = delay;
    (*current_config)->packet_count = packet_count;
    (*current_config)->logging = logging;
}

void dbg_simping_config(struct simping_config *config) {
    printf("simping config\n");
    printf("buff_size: %hu\n", config->buff_size);
    printf("target_ip: %s\n", config->target_ip);
    printf("delay: %hu\n", config->delay);
    printf("packet_count: %d\n", config->packet_count);
    printf("longging: %d\n", config->logging);
}

void free_simping_config(struct simping_config *config) {
    free(config->target_ip);
    free(config);
}

