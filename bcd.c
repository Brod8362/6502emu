#include "bcd.h"
#include <stdint.h>

uint8_t bcd_to_dec(const uint8_t hex) {
    uint8_t lo = hex & 0b00001111;
    uint8_t hi = (hex & 0b11110000) >> 4;
    return (hi*10)+(lo);
}

uint8_t dec_to_bcd(const uint8_t dec) {
    uint8_t lo = dec % 10;
    uint8_t hi = dec / 10;
    return lo | (hi << 4);
}