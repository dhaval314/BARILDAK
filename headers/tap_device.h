#ifndef TAP_DEVICE_H
#define TAP_DEVICE_H

#include "../headers/vpn_common.h" // Shared structures and definitions
#include <windows.h> // For HANDLE, DWORD, etc.

// TAP-Windows device constants
#define TAP_IOCTL_SET_MEDIA_STATUS CTL_CODE(FILE_DEVICE_UNKNOWN, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_CONFIG_TUN CTL_CODE(FILE_DEVICE_UNKNOWN, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Function declarations
HANDLE open_tap_device(char *guid);
BOOL configure_tap_interface(HANDLE tap_handle, ULONG ip_addr, ULONG subnet_mask);
BOOL set_tap_status(HANDLE tap_handle, BOOL up);
BOOL read_from_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size, DWORD *bytes_read);
BOOL write_to_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size, DWORD *bytes_written);

#endif // TAP_DEVICE_H
