#ifndef NETWORK_H
#define NETWORK_H

#include "vpn_common.h"

// Function declarations
BOOL initialize_winsock();
SOCKET create_udp_socket();
void send_hole_punch_packet(SOCKET sock, peer_info *target);
void send_keepalive(SOCKET sock, peer_info *peer);
void handle_incoming_connection(SOCKET sock);
SOCKET connect_to_peer(peer_info *peer);
peer_info parse_connection_details(const char *details_str);

#endif // NETWORK_H
