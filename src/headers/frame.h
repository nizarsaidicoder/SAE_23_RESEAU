#pragma once
#include "types.h"
#include "device.h"
#include "network.h"
#include "address.h"
#include "utils.h"
#include "bpdu.h"
#include <stdbool.h>



void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, char *data);
void frame_print_data_user_mode(Frame *frame);
void frame_print_data_hex_mode(Frame *frame);

uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[]);
void add_entry_to_switching_table(Device *switch_, MACAddress *mac_address, uint8_t port_number);
uint8_t get_port_number(Network *network, Device *switch_, Device *device);

void send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame);
int check_switching_table_entries(Device *switch_, MACAddress *mac_address);
void flood_frame(Network *network, Device *source, Device *previous_source, Frame *frame);
bool receive_frame(Network *network, Device *device, Device *previous_device, Frame *frame);
