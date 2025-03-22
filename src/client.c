#include "../headers/client.h"

// Initialize a UDP socket bound to the specified local port
SOCKET initialize_client_socket(int local_port) {
    SOCKET sock = create_udp_socket();

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any local address
    local_addr.sin_port = htons(local_port);

    if (bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)) == SOCKET_ERROR) {
        printf("Failed to bind socket: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Socket bound to local port %d\n", local_port);
    return sock;
}

// Perform NAT traversal using UDP hole punching
void perform_nat_traversal(SOCKET sock, peer_info *local_peer, peer_info *remote_peer) {
    printf("Starting NAT traversal...\n");

    send_hole_punch_packet(sock, remote_peer);

    struct sockaddr_in private_addr;
    private_addr.sin_family = AF_INET;
    private_addr.sin_port = htons(remote_peer->private_port);
    inet_pton(AF_INET, remote_peer->private_ip, &private_addr.sin_addr);

    const char *message = "HOLE_PUNCH_PRIVATE";
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr*)&private_addr, sizeof(private_addr));

    printf("Hole punch packets sent to public and private addresses.\n");
}

// Listen for incoming packets from the peer
void listen_for_peer(SOCKET sock) {
    char buffer[1024];
    struct sockaddr_in sender_addr;
    int sender_len = sizeof(sender_addr);

    printf("Listening for incoming packets...\n");

    while (1) {
        int bytes_received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                      (struct sockaddr*)&sender_addr, &sender_len);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            char sender_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, INET_ADDRSTRLEN);

            printf("Received %d bytes from %s:%d: %s\n",
                   bytes_received,
                   sender_ip,
                   ntohs(sender_addr.sin_port),
                   buffer);
        }
    }
}

// Send a hole-punching packet to the target peer
void send_hole_punch_packet(SOCKET sock, peer_info *target) {
    struct sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target->public_port);
    inet_pton(AF_INET, target->public_ip, &target_addr.sin_addr);

    const char *message = "HOLE_PUNCH";
    sendto(sock, message, strlen(message), 0,
           (struct sockaddr*)&target_addr, sizeof(target_addr));

    printf("Hole punch packet sent to %s:%d\n", target->public_ip, target->public_port);
}
