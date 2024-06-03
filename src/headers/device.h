#pragma once

#include "types.h"
#include "address.h"

void device_init(Device *device, MACAddress mac_address, DeviceType type);
void device_free(Device *device);
void station_init(Device *device, MACAddress mac_address, IPAddress ip_address);
void switch_init(Device *device, MACAddress mac_address, uint16_t priority, uint8_t num_ports);
void assign_bpdu(Device *device);
bool device_is_switch(Device *device);
bool device_is_station(Device *device);
void print_station(Device *device);
void print_switch(Device *device);
void print_device(Device *device);
void print_link(Link *link);
void device_from_config(Device *device, char *info);
char *device_to_config(Device *device, char *out);
void switch_print_table(Switch switch_);
