#pragma once

#include <stdint.h>

typedef enum {
    OS_UNSURE,
    OS_LINUX,
    OS_WINDOWS,
    OS_MACOS,
    OS_IOS,
} OSVariant;

void process_wlength(const uint16_t w_length);
OSVariant guess_host_os(void);
