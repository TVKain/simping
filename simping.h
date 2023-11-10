/*
 Simple ping program
 Supports IPv4
 Does not support dns, ipv6 
*/

#ifndef SIMPING_H
#define SIMPING_H

#include <netinet/in_systm.h>

#include "simping_config.h"

// create and return sockfd for raw socket
int simping_create_raw_socket();

// close raw socket
void simping_close_raw_socket(int sockfd);

// create sockaddr_in for the dest ip
struct sockaddr_in simping_create_target(char *ip);

// send ping packet using sockfd socket, to target 
ssize_t simping_send(uint8_t *send_buffint, uint16_t send_buff_size, int sockfd, struct sockaddr_in target);

// entry to program 
void simping_start(struct simping_config *config);

// todo: add listener to echo reply 
void simping_read_loop();



#endif
