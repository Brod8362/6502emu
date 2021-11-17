#ifndef BCD_H
#define BCD_H

#include "types.h"

uint8_t bcd_to_dec(const uint8_t hex);

uint8_t dec_to_bcd(const uint8_t dec);

#endif