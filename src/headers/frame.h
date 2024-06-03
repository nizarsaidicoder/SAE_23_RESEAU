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
    uint8_t data[1500];
    uint32_t FCS;
} Frame;

// TODO : REMY
void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, char *data);
void frame_print_data_user_mode(Frame *frame);
void frame_print_data_hex_mode(Frame *frame);
uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[]);
void update_switching_table(Network *network, Device *switch_, Device *device);
void add_entry_to_switching_table(Device *switch_, MACAddress *mac_address, uint8_t port_number);
uint8_t get_port_number(Network *network, Device *switch_, Device *device);

// bool send_frame_from_station(Network *network, Device *source, Device *destination, Frame *frame);
// bool send_frame_from_switch(Network *network, Device *switch_, Device *destination, Frame *frame);
void send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame);
int check_switching_table_entries(Device *switch_, MACAddress *mac_address);
void flood_frame(Network *network, Device *source, Device *previous_source, Frame *frame);
bool receive_frame(Device *device, Frame *frame);
