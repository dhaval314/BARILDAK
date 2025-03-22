#ifndef NETWORK_H
#define NETWORK_H

#include "../headers/vpn_common.h" // Shared structures and definitions

// Function declarations for networking operations
BOOL initialize_winsock();
SOCKET create_udp_socket();
void send_hole_punch_packet(SOCKET sock, peer_info *target);
void send_keepalive(SOCKET sock, peer_info *peer);
peer_info parse_connection_details(const char *details_str);

#endif // NETWORK_H
