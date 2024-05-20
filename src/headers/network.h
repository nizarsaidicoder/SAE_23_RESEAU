#pragma once
#include "types.h"

void network_init(Network *network);
uint16_t network_num_devices(Network *network);
uint16_t network_num_links(Network *network);
void network_add_device(Network *network, Device *device);
void network_add_link(Network *network, uint8_t device1_index, uint8_t device2_index);
Device *network_find_device(Network *network, uint8_t device_index);
void network_from_config(Network *network, char *filename);
void network_to_config(Network *network, char *filename);