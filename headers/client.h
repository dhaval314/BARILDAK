#ifndef CLIENT_H
#define CLIENT_H

#include "../headers/network.h" // Networking functions
#include "../headers/vpn_common.h" // Shared structures and definitions

// Function declarations for client operations
SOCKET initialize_client_socket(int local_port);
void perform_nat_traversal(SOCKET sock, peer_info *local_peer, peer_info *remote_peer);
void listen_for_peer(SOCKET sock);
void send_hole_punch_packet(SOCKET sock, peer_info *target);

#endif // CLIENT_H
