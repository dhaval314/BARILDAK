#ifndef CLIENT_H
#define CLIENT_H

#include "vpn_common.h"

void get_peer_info(peer_info *peer);
void run_vpn_client(SOCKET sock, HANDLE tap_handle);

#endif // CLIENT_H
