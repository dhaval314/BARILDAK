#ifndef NETWORK_H
#define NETWORK_H

#include "vpn_common.h"

BOOL initialize_winsock();
SOCKET create_udp_socket(int port);
void close_socket(SOCKET sock);
int send_udp_packet(SOCKET sock, const char *ip, int port, const char *data);
int receive_udp_packet(SOCKET sock, char *buffer, struct sockaddr_in *sender);
void perform_nat_traversal(SOCKET sock, peer_info *local, peer_info *remote);

#endif // NETWORK_H
