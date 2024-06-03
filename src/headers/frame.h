#pragma once
#include "types.h"

typedef struct Frame
{
    // [170, 170, 170, 170, 170, 170, 170]

    uint8_t preambule[7];
    // 171
    uint8_t SFD;
    MACAddress dest;
    MACAddress src;
    uint16_t type;
    uint8_t *data;
    uint32_t FCS;
} Frame;

// TODO : REMY
void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, uint8_t *data);
void frame_print_data_user_mode(Frame *frame);
void frame_print_data_hex_mode(Frame *frame);
uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[]);
void update_switching_table(Network *network, Device *switch_, Device *device);
bool send_frame_from_station(Network *network, Device *source, Device *destination, Frame *frame);
bool send_frame_from_switch(Network *network, Device *switch_, Device *destination, Frame *frame);
bool send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame);
bool receive_frame(Device *device, Frame *frame);

// TODO : REMY
bool compare_mac_address(MACAddress *mac1, MACAddress *mac2);
