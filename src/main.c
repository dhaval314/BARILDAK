#include "../headers/vpn_common.h"
#include "../headers/network.h"
#include "../headers/client.h"

// Function to get peer info from user input
void get_peer_info(peer_info *peer) {
    printf("Enter remote peer's public IP: ");
    scanf("%15s", peer->public_ip);

    printf("Enter remote peer's public port: ");
    scanf("%d", &peer->public_port);

    printf("Enter remote peer's private IP: ");
    scanf("%15s", peer->private_ip);

    printf("Enter remote peer's private port: ");
    scanf("%d", &peer->private_port);

    // For simplicity, we'll leave the public key empty
    memset(peer->pubkey, 0, sizeof(peer->pubkey));

    printf("Peer information:\n");
    printf("Public IP: %s, Public Port: %d\n", peer->public_ip, peer->public_port);
    printf("Private IP: %s, Private Port: %d\n", peer->private_ip, peer->private_port);
}

int main() {
    const int local_port = 12345; // Local port for this client

    // Local peer information (can be determined via STUN or similar)
    peer_info local_peer = {
        .public_ip = "198.51.100.2", // Replace with actual public IP if available
        .public_port = local_port,
        .private_ip = "192.168.0.20", // Replace with actual private IP if available
        .private_port = local_port,
        .pubkey = {0} // Public key can be added here
    };

    // Remote peer information (to be entered by the user)
    peer_info remote_peer;
    get_peer_info(&remote_peer);

    // Initialize UDP socket bound to the local port
    SOCKET sock = initialize_client_socket(local_port);

    // Perform NAT traversal using UDP hole punching
    perform_nat_traversal(sock, &local_peer, &remote_peer);

    // Start listening for incoming packets from the remote peer
    listen_for_peer(sock);

    // Clean up resources
    closesocket(sock);
    WSACleanup();

    return 0;
}
