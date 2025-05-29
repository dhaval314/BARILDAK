# Barildak

This is a minimal VPN program in C that creates a TUN interface and uses UDP hole punching to establish a direct peer-to-peer VPN connection without relying on a STUN server.

---

## Features

- Creates a TUN virtual network interface (`tun0`)
- Uses UDP socket communication with hole punching technique
- Sends packets between local TUN device and remote peer's UDP socket
- No external dependencies or STUN server required
- Works on Linux systems with TUN/TAP support

---

## How It Works

1. The program creates a TUN interface (`tun0`) which acts as a virtual network device.
2. It creates a UDP socket and binds to an ephemeral port.
3. It prompts the user for the peer's public IP address and UDP port.
4. It sends a few "hole punch" UDP packets to the peer to open up NATs.
5. Then it forwards packets bidirectionally between the TUN device and the UDP socket.

---

## Requirements

- Linux with TUN/TAP support enabled (`/dev/net/tun` device)
- C compiler (e.g., `gcc`)
- Root privileges to create and configure TUN interface

---

## Building

Compile the program using gcc:

```bash
gcc -o barildak barildak.c
```

## Running

Run the program as root or with sudo (required for TUN device access):

```bash
sudo ./barildak
```

Run this on peer A

```bash
sudo ip addr add 10.0.0.1/24 dev tun0
sudo ip link set tun0 up
```

Run this on peer B

```bash
sudo ip addr add 10.0.0.1/24 dev tun0
sudo ip link set tun0 up
```

## Notes

- Both peers must run this program, each providing the other's public IP and port.
- The program currently does not implement signaling or automatic peer discovery.
- The VPN IPs assigned to the TUN interface are static and should be configured separately if used in a real scenario.
