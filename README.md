# SIMPING 

## Introduction
SIMPING is a simple ping clone program that I made to understand more about *raw socket*, *icmp packet*, *ip flood attack* 

SIMPING supports
- IPv4 protocol

SIMPING does not support 
- IPv6 protocol
- DNS 
- echo reply listener is not implemented 

OS supports 
- Linux

## Requirements
- gcc
- make
## Installation 
```
git clone https://github.com/TVKain/simping.git
cd simping
make 
./simping -h
```

## Features
- packet with custom size 
- send packet indefinitely until receive interrupt, can specify delay between each packet 
- specify number of packets to send 
- disable/enable logging 
- send as many packets as possible (try to flood with the command `simping -t <ip_address>`) 

```
Usage: ./simping [-s <size>] [-d <delay>] [-c <count>] <-t <ip_address>>
Options:
    -t, --target <ip_address>       Specify the target address to send the packet
    -s, --size <size>               Specify the size (in bytes) of the packet to be sent
                                    If not specify default size is 64 bytes
    -d, --delay <delay>             Specify the delay (in milisecond) between each packet
                                    If not specify send without delay
    -c, --count <packet_count>      Specify the number of packets to be sent
                                    If not specify send until receive interrupt
    -l, --logging                   If not specify does not log anything
    -h, --help                      Display this help message
```

