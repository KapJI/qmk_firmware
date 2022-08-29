#pragma once

#include <stdint.h>

#define STORED_USB_SETUPS 50
#define EEPROM_USER_OFFSET (uint8_t*)35

typedef enum {
    OS_UNSURE,
    OS_LINUX,
    OS_WINDOWS,
    OS_MACOS,
    OS_IOS,
} OSVariant;

void process_wlength(const uint16_t w_length);
OSVariant guess_host_os(void);

void print_all_setups(void);
void store_setups_in_eeprom(void);
