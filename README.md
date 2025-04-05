# Barildak

Barildak is a lightweight peer-to-peer VPN client written in C using Winsock, designed for simulating a virtual LAN connection between two clients over the internet. Its primary use case is enabling LAN-based multiplayer games like Age of Empires II without needing a centralized VPN service like Hamachi or ZeroTier.

This project uses UDP hole punching and public STUN servers to traverse NATs, without requiring a signaling server. Users manually exchange and input each other's public IP and port for connection.

## Project Status

This is a work-in-progress. The current prototype includes:

- NAT traversal using UDP hole punching
- Public IP/port discovery using a STUN server
- Peer-to-peer communication over UDP
- Manual peer input (no signaling server)
- All-in-one executable

## Known Issues
- TAP compaitiblity

## How It Works

1. Each client performs a STUN request to get its public IP and port.
2. Users manually exchange their public IP:port information.
3. The clients begin sending UDP packets to each other to open NAT holes.
4. Once a connection is established, data is forwarded directly between the peers.

## Requirements

- Windows 10 or 11
- C compiler (MinGW or Visual Studio)
- STUN server (default: stun.l.google.com:19302)
- TAP-Windows Adapter for full virtual LAN simulation

