#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

// Structure to store peer information
typedef struct {
    char public_ip[16];      // Public IP address of the peer
    int public_port;         // Public port of the peer
    char private_ip[16];     // Private IP address of the peer (behind NAT)
    int private_port;        // Private port of the peer (behind NAT)
    unsigned char pubkey[32]; // Public key for encryption
} peer_info;

// Function declarations for networking operations
BOOL initialize_winsock();
SOCKET create_udp_socket();
void send_hole_punch_packet(SOCKET sock, peer_info *target);
void send_keepalive(SOCKET sock, peer_info *peer);
peer_info parse_connection_details(const char *details_str);

#endif // NETWORK_H
