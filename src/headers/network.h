#pragma once
#include "types.h"

void network_init(Network *network);
void network_free(Network *network);

uint8_t network_num_devices(Network *network);
uint16_t network_num_links(Network *network);
uint8_t network_num_stations(Network *network);
uint8_t network_num_switches(Network *network);

bool network_link_exists(Network *network, Link *link);
uint8_t network_link_index(Network *network, Link *link);
uint16_t network_link_weight(Network *network, uint8_t device1_index, uint8_t device2_index);

void network_add_device(Network *network, Device *device);
bool network_add_link(Network *network, Link *link);

Device *network_find_device(Network *network, MACAddress *mac_address);
uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[]);

void network_print(Network *network);
bool network_from_config(Network *network, char *filename);
