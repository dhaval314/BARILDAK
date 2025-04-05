#ifndef VPN_COMMON_H
#define VPN_COMMON_H

// Winsock2 MUST come before Windows.h
#define WIN32_LEAN_AND_MEAN  // Reduce header conflicts
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>  // Included after Winsock2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_PEERS 10
#define BUFFER_SIZE 1024
#define TAP_DEVICE_GUID "tap0901"

typedef struct {
    char public_ip[16];
    int public_port;
    char private_ip[16];
    int private_port;
    unsigned char pubkey[32];
} peer_info;

#endif // VPN_COMMON_H
