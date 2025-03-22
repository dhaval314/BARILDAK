#ifndef VPN_COMMON_H
#define VPN_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")

// Structure to hold peer information
typedef struct {
    char public_ip[16];
    int public_port;
    char private_ip[16];
    int private_port;
    unsigned char pubkey[32]; // Public key for encryption
} peer_info;

// Other common definitions and macros

#endif // VPN_COMMON_H
