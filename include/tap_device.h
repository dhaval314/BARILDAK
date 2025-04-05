#ifndef TAP_DEVICE_H
#define TAP_DEVICE_H

#include "vpn_common.h"

// Explicitly declare Windows types
typedef void *HANDLE;
typedef unsigned long DWORD;

#define TAP_IOCTL_SET_MEDIA_STATUS CTL_CODE(FILE_DEVICE_UNKNOWN, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define TAP_IOCTL_CONFIG_TUN CTL_CODE(FILE_DEVICE_UNKNOWN, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)

HANDLE open_tap_device();
BOOL configure_tap_interface(HANDLE tap_handle, const char *ip, const char *subnet);
BOOL set_tap_status(HANDLE tap_handle, BOOL status);
DWORD read_from_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size);
DWORD write_to_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size);
void close_tap_device(HANDLE tap_handle);

#endif // TAP_DEVICE_H
