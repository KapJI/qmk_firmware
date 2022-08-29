#include "guess_os.h"

struct setups_data_t {
    uint8_t count;
    uint8_t cnt_02;
    uint8_t cnt_04;
    uint8_t cnt_ff;
    uint16_t last_wlength;
};

struct setups_data_t setups_data = {
    .count = 0,
    .cnt_02 = 0,
    .cnt_04 = 0,
    .cnt_ff = 0,
};

void process_wlength(const uint16_t w_length) {
    setups_data.count++;
    setups_data.last_wlength = w_length;
    if (w_length == 0x2) {
        setups_data.cnt_02++;
    } else if (w_length == 0x4) {
        setups_data.cnt_04++;
    } else if (w_length == 0xFF) {
        setups_data.cnt_ff++;
    }
}

// Some data I collected:
// Windows 10: [FF, FF, 4, 24, 4, 24, 4, FF, 24, FF, 4, FF, 24, 4, 24, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A, 20A]
// Another host with Windows 10: [FF, FF, 4, 24, 4, 24, 4, 24, 4, 24, 4, 24]
// macOS 12.5: [2, 24, 2, 28, FF]
// iOS/iPadOS 15.6: [2, 24, 2, 28]
// Linux (including Android, Raspberry Pi and WebOS TV): [FF, FF, FF]
// PS5: [2, 4, 2, 28, 2, 24]
// Nintendo Switch: [82, FF, 40, 40, FF, 40, 40, FF, 40, 40, FF, 40, 40, FF, 40, 40]
// Quest 2: [FF, FF, FF, FE, FF, FE, FF, FE, FF, FE, FF]
OSVariant guess_host_os(void) {
    if (setups_data.count < 3) {
        return OS_UNSURE;
    }
    if (setups_data.cnt_04 > 1) {
        // Windows usually has five packets with 0x4.
        if (setups_data.count > 10 && setups_data.cnt_ff > 1 && setups_data.cnt_04 > 3) {
            return OS_WINDOWS;
        } else {
            return OS_UNSURE;
        }
    }
    if (setups_data.count == setups_data.cnt_ff) {
        // Linux has 3 packets with 0xFF.
        return OS_LINUX;
    }
    if (setups_data.count < 10 && setups_data.last_wlength == 0xFF && setups_data.cnt_ff == 1 && setups_data.cnt_02 == 2) {
        // MacOS has 5 packets.
        return OS_MACOS;
    }
    if (setups_data.count < 10 && setups_data.last_wlength == 0x28 && setups_data.cnt_ff == 0 && setups_data.cnt_02 == 2) {
        // iOS and iPadOS don't have the last 0xFF packet.
        return OS_IOS;
    }
    if (setups_data.cnt_ff == 0 && setups_data.cnt_02 == 3) {
        // This is actually PS5.
        return OS_LINUX;
    }
    if (setups_data.cnt_ff > 3 && setups_data.cnt_02 == 0 && setups_data.cnt_04 == 0) {
        // This is actually Quest 2 or Nintendo Switch.
        return OS_LINUX;
    }
    return OS_UNSURE;
}
