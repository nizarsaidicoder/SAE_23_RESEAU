#include "types.h"
#include "address.h"

void device_init(Device *device, MACAddress mac_address, DeviceType type);
void station_init(Device *device, MACAddress mac_address, IPAddress ip_address);
void switch_init(Device *device, uint16_t priority, uint8_t num_ports);
bool device_is_switch(Device *device);
bool device_is_station(Device *device);
void print_device(Device *device);
void device_from_config(Device *device, char *info);
char *device_to_config(Device *device);
void switch_print_table(Device *device);
