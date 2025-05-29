#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/in.h>

#define BUFSIZE 2000

// ------------------------
// Create and configure TUN device
// ------------------------
int create_tun_device(char *devname) {
    struct ifreq ifr;
    int fd = open("/dev/net/tun", O_RDWR);
    if (fd < 0) {
        perror("Opening /dev/net/tun");
        exit(EXIT_FAILURE);
    }

    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    if (*devname) strncpy(ifr.ifr_name, devname, IFNAMSIZ);

    if (ioctl(fd, TUNSETIFF, (void *)&ifr) < 0) {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    strcpy(devname, ifr.ifr_name);
    return fd;
}

// ------------------------
// Create and bind UDP socket
// ------------------------
int create_udp_socket(struct sockaddr_in *local_addr) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(0); // ephemeral port

    if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(sock);
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(*local_addr);
    if (getsockname(sock, (struct sockaddr *)local_addr, &len) < 0) {
        perror("getsockname");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP socket bound to local port: %d\n", ntohs(local_addr->sin_port));
    return sock;
}

// ------------------------
// Ask user for peer IP and port
// ------------------------
void get_peer_info(struct sockaddr_in *peer_addr) {
    char ip[32];
    int port;

    printf("Enter peer public IP: ");
    scanf("%31s", ip);
    printf("Enter peer port: ");
    scanf("%d", &port);

    memset(peer_addr, 0, sizeof(*peer_addr));
    peer_addr->sin_family = AF_INET;
    peer_addr->sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &peer_addr->sin_addr) != 1) {
        fprintf(stderr, "Invalid IP address\n");
        exit(EXIT_FAILURE);
    }
}

// ------------------------
// Send hole-punch packets
// ------------------------
void punch_hole(int sock, struct sockaddr_in *peer_addr) {
    char msg[] = "hi";
    for (int i = 0; i < 5; i++) {
        sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *)peer_addr, sizeof(*peer_addr));
        usleep(100000); // 100 ms
    }
}

// ------------------------
// VPN loop: TUN <-> UDP
// ------------------------
void vpn_loop(int tun_fd, int sock, struct sockaddr_in *peer_addr) {
    char buffer[BUFSIZE];
    fd_set fds;

    while (1) {
        FD_ZERO(&fds);
        FD_SET(tun_fd, &fds);
        FD_SET(sock, &fds);
        int maxfd = (tun_fd > sock ? tun_fd : sock) + 1;

        int ret = select(maxfd, &fds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(tun_fd, &fds)) {
            int n = read(tun_fd, buffer, BUFSIZE);
            if (n < 0) {
                perror("read tun");
                break;
            }
            sendto(sock, buffer, n, 0, (struct sockaddr *)peer_addr, sizeof(*peer_addr));
        }

        if (FD_ISSET(sock, &fds)) {
            struct sockaddr_in from;
            socklen_t fromlen = sizeof(from);
            int n = recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *)&from, &fromlen);
            if (n < 0) {
                perror("recvfrom");
                break;
            }
            write(tun_fd, buffer, n);
        }
    }
}

// ------------------------
// Entry Point
// ------------------------
int main() {
    srand(time(NULL));

    // Setup TUN
    char tun_name[IFNAMSIZ] = "tun0";
    int tun_fd = create_tun_device(tun_name);
    printf("Created TUN device: %s\n", tun_name);

    // Setup UDP
    struct sockaddr_in local_addr;
    int sock = create_udp_socket(&local_addr);

    // Setup peer info
    struct sockaddr_in peer_addr;
    get_peer_info(&peer_addr);

    // Punch hole
    punch_hole(sock, &peer_addr);

    // Start VPN
    vpn_loop(tun_fd, sock, &peer_addr);

    close(sock);
    close(tun_fd);
    return 0;
}
