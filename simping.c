#include "simping.h"
#include "simping_config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int simping_create_raw_socket() {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockfd < 0) {
        fprintf(stderr, "Error in creating raw socket\n");
        exit(1);
    }

    return sockfd;
}

void simping_close_raw_socket(int sockfd) {
    close(sockfd);
}

unsigned short simping_checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = (unsigned short)~sum;

    return result;
}

void simping_start_inner(struct simping_config *config, int sockfd, struct sockaddr_in target, uint8_t *send_buff) {
    
    ssize_t ret;
    do {
        ret = simping_send(send_buff, config->buff_size, sockfd, target);

        if (config->logging) {
            if (ret == -1) {
                fprintf(stderr, "Error: can not send packet to %s\n", config->target_ip);
            } else {
                printf("Sent packet with %d byte to %s\n", config->buff_size, config->target_ip);
            }
        }
    } while (ret == -1);

    if (config->delay != 0) {
        sleep(((float)config->delay / 1000));
    }
}

void simping_start(struct simping_config *config) {
    int sockfd = simping_create_raw_socket();

    struct sockaddr_in target = simping_create_target(config->target_ip);

    uint8_t *send_buff = malloc(sizeof(uint8_t) * config->buff_size);    

    // send until receive interrupt
    if (config->packet_count == 0) {
        for (;;) {
            simping_start_inner(config, sockfd, target, send_buff);
        }
    } else {
        for (int i = 0; i < config->packet_count; ++i) {
            simping_start_inner(config, sockfd, target, send_buff);
        }
    }
}



ssize_t simping_send(uint8_t *send_buff, uint16_t send_buff_size,  int sockfd, struct sockaddr_in target) {
    // Construct icmp header
    // Zero out send buff
    memset(send_buff, 0, sizeof(uint8_t) * send_buff_size);

    struct icmphdr *icmp = (struct icmphdr *) send_buff;;

    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = getpid();
    icmp->un.echo.sequence = 0;
    icmp->checksum = simping_checksum(send_buff, sizeof(uint8_t) * send_buff_size);

    return sendto(sockfd, send_buff, sizeof(uint8_t) * send_buff_size, 0, (struct sockaddr *)&target, sizeof(target));
}

struct sockaddr_in simping_create_target(char *ip) {
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(ip);

    return target;
}

