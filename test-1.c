// Test number 1 sending 100 packets sequentially using 1 raw socket
#include "simping.h"
#include "simping_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    char buffer[64];

    int sockfd = simping_create_raw_socket();
    struct sockaddr_in  target = simping_create_target("20.89.255.198");

    size_t ret;
    for (int i = 0; i < 100; ++i) {
        ret = simping_send(buffer, 64, sockfd, target);
        if (ret == -1) {
            printf("Fail\n");
        } else {
            printf("Success\n");
        }
    }
    
    return 0;
}