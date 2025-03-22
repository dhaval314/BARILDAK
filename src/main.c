#include "vpn_common.h"
#include "network.h"
#include "tap_device.h"

int main(int argc, char *argv[]) {
    // Initialize Winsock
    if (!initialize_winsock()) {
        return 1;
    }
    
    // Main program implementation...
    
    return 0;
}
