#include "../include/client.h"
#include "../include/network.h"
#include "../include/tap_device.h"
#include <io.h>      // Add this
#include <fcntl.h>   // Add this

void get_peer_info(peer_info *peer) {
    printf("Enter remote peer's public IP: ");
    scanf("%15s", peer->public_ip);
    printf("Enter remote peer's public port: ");
    scanf("%d", &peer->public_port);
    printf("Enter remote peer's private IP: ");
    scanf("%15s", peer->private_ip);
    printf("Enter remote peer's private port: ");
    scanf("%d", &peer->private_port);
    printf("Peer info captured successfully.\n");
}

void run_vpn_client(SOCKET sock, HANDLE tap_handle) {
    fd_set read_fds;
    struct timeval timeout = {1, 0};
    
    // Convert Windows HANDLE to file descriptor
    int tap_fd = _open_osfhandle((intptr_t)tap_handle, _O_RDWR);

    while(1) {
        FD_ZERO(&read_fds);
        FD_SET(sock, &read_fds);
        FD_SET(tap_fd, &read_fds);  // Use converted file descriptor

        int max_fd = (sock > tap_fd) ? sock : tap_fd;
        if(select(max_fd + 1, &read_fds, NULL, NULL, &timeout) == SOCKET_ERROR) {
            fprintf(stderr, "Select error: %d\n", WSAGetLastError());
            break;
        }

        if(FD_ISSET(sock, &read_fds)) {
            struct sockaddr_in sender;
            char buffer[BUFFER_SIZE];
            int bytes_received = receive_udp_packet(sock, buffer, &sender);
            
            if(bytes_received > 0) {
                write_to_tap(tap_handle, (BYTE*)buffer, bytes_received);
            }
        }

        if(FD_ISSET(tap_fd, &read_fds)) {
            BYTE buffer[BUFFER_SIZE];
            DWORD bytes_read = read_from_tap(tap_handle, buffer, BUFFER_SIZE);
            
            if(bytes_read > 0) {
                send_udp_packet(sock, "203.0.113.1", 54321, (const char*)buffer);
            }
        }
    }
}
