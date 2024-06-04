#pragma once
#include "types.h"

void network_init(Network *network);
void network_free(Network *network);
uint8_t network_num_devices(Network *network);
uint16_t network_num_links(Network *network);
uint8_t network_num_stations(Network *network);
uint8_t network_num_switches(Network *network);
void network_add_device(Network *network, Device *device);
bool network_link_exists(Network *network, Link *link);
uint8_t network_link_index(Network *network, Link *link);
bool network_add_link(Network *network, Link *link);
void network_print(Network *network);
Device *network_find_device(Network *network, MACAddress *mac_address);
bool network_from_config(Network *network, char *filename);
void network_to_config(Network *network, char *filename);
void print_switching_tables(Network *network);
uint16_t network_link_weight(Network *network, uint8_t device1_index, uint8_t device2_index);
