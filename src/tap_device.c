#include "../include/tap_device.h"
#include <winioctl.h>

HANDLE open_tap_device() {
    char device_path[MAX_PATH];
    snprintf(device_path, MAX_PATH, "\\\\.\\Global\\%s.tap", TAP_DEVICE_GUID);

    HANDLE handle = CreateFile(
        device_path,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
        0
    );

    if (handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open TAP device: %lu\n", GetLastError());
    }
    return handle;
}

BOOL configure_tap_interface(HANDLE tap_handle, const char *ip, const char *subnet) {
    DWORD len;
    ULONG ip_addr = inet_addr(ip);
    ULONG subnet_mask = inet_addr(subnet);

    struct {
        ULONG ip_addr;
        ULONG subnet_mask;
        ULONG gateway;
    } tap_config = {ip_addr, subnet_mask, ip_addr & subnet_mask};

    return DeviceIoControl(
        tap_handle,
        TAP_IOCTL_CONFIG_TUN,
        &tap_config,
        sizeof(tap_config),
        &tap_config,
        sizeof(tap_config),
        &len,
        NULL
    );
}

BOOL set_tap_status(HANDLE tap_handle, BOOL status) {
    DWORD len;
    ULONG media_status = status ? 1 : 0;
    return DeviceIoControl(
        tap_handle,
        TAP_IOCTL_SET_MEDIA_STATUS,
        &media_status,
        sizeof(media_status),
        &media_status,
        sizeof(media_status),
        &len,
        NULL
    );
}

DWORD read_from_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size) {
    DWORD bytes_read;
    ReadFile(tap_handle, buffer, buffer_size, &bytes_read, NULL);
    return bytes_read;
}

DWORD write_to_tap(HANDLE tap_handle, BYTE *buffer, DWORD buffer_size) {
    DWORD bytes_written;
    WriteFile(tap_handle, buffer, buffer_size, &bytes_written, NULL);
    return bytes_written;
}

void close_tap_device(HANDLE tap_handle) {
    if (tap_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(tap_handle);
    }
}
