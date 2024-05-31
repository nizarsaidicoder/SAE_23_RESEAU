#pragma once
#include "headers/types.h"

typedef struct Trame
{
    // [170, 170, 170, 170, 170, 170, 170]
    
    uint8_t preambule[7];
    // 171
    uint8_t SFD;
    MACAddress dest;
    MACAddress src;
    uint16_t type;
    uint8_t data[46];
    uint32_t FCS;
} Trame;

void trame_init(Trame * trame, MACAddress dest, MACAddress src, uint16_t type, uint8_t * data);
void trame_print(Trame * trame);