#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET initialize_server(const char *ip, int port) {
    SOCKET server_sock;
    struct sockaddr_in server_addr;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        perror("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Bind socket to IP and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("Bind failed");
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    return server_sock;
}

void start_listening(SOCKET server_sock) {
    if (listen(server_sock, 5) == SOCKET_ERROR) {
        perror("Listen failed");
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server is listening...\n");
}

SOCKET create_udp_socket() {
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return sock;
}




