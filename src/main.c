#include "../include/vpn_common.h"
#include "../include/network.h"
#include "../include/tap_device.h"
#include "../include/client.h"

int main() {
    if(!initialize_winsock()) {
        return 1;
    }

    // Create TAP device
    HANDLE tap_handle = open_tap_device();
    if(tap_handle == INVALID_HANDLE_VALUE) {
        WSACleanup();
        return 1;
    }

    // Configure TAP interface
    if(!configure_tap_interface(tap_handle, "10.0.0.1", "255.255.255.0") ||
       !set_tap_status(tap_handle, TRUE)) {
        close_tap_device(tap_handle);
        WSACleanup();
        return 1;
    }

    // Create UDP socket
    SOCKET sock = create_udp_socket(12345);
    if(sock == INVALID_SOCKET) {
        close_tap_device(tap_handle);
        WSACleanup();
        return 1;
    }

    // Get peer info
    peer_info remote_peer;
    get_peer_info(&remote_peer);

    // Perform NAT traversal
    perform_nat_traversal(sock, NULL, &remote_peer);

    // Start VPN client
    run_vpn_client(sock, tap_handle);

    // Cleanup
    close_socket(sock);
    close_tap_device(tap_handle);
    WSACleanup();

    printf("Press Enter to exit...");
    while(getchar() != '\n');  // Clear input buffer
    getchar();  // Wait for Enter key
    return 0;
}
