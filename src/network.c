#include "../include/network.h"

BOOL initialize_winsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", WSAGetLastError());
        return FALSE;
    }
    return TRUE;
}

SOCKET create_udp_socket(int port) {
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "Socket creation failed: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        fprintf(stderr, "Bind failed: %d\n", WSAGetLastError());
        closesocket(sock);
        return INVALID_SOCKET;
    }
    return sock;
}

void close_socket(SOCKET sock) {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
}

int send_udp_packet(SOCKET sock, const char *ip, int port, const char *data) {
    struct sockaddr_in dest_addr = {0};
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &dest_addr.sin_addr);

    return sendto(sock, data, strlen(data), 0,
                  (struct sockaddr*)&dest_addr, sizeof(dest_addr));
}

int receive_udp_packet(SOCKET sock, char *buffer, struct sockaddr_in *sender) {
    int sender_len = sizeof(struct sockaddr_in);
    return recvfrom(sock, buffer, BUFFER_SIZE - 1, 0,
                    (struct sockaddr*)sender, &sender_len);
}

void perform_nat_traversal(SOCKET sock, peer_info *local, peer_info *remote) {
    printf("Starting NAT traversal...\n");
    
    // Send to public endpoint
    send_udp_packet(sock, remote->public_ip, remote->public_port, "HOLE_PUNCH_PUBLIC");
    
    // Send to private endpoint (LAN)
    send_udp_packet(sock, remote->private_ip, remote->private_port, "HOLE_PUNCH_PRIVATE");
    
    printf("Sent hole punch packets to both endpoints\n");
}
