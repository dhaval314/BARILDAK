#ifndef VPN_COMMON_H
#define VPN_COMMON_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib") // Link Winsock library

// Structure to store peer information
typedef struct {
    char public_ip[16];      // Public IP address of the peer
    int public_port;         // Public port of the peer
    char private_ip[16];     // Private IP address of the peer (behind NAT)
    int private_port;        // Private port of the peer (behind NAT)
    unsigned char pubkey[32]; // Public key for encryption
} peer_info;

#endif // VPN_COMMON_H
