#include "types.h"
#include "address.h"

void station_init(Device *device);
void switch_init(Device *device);
void device_is_switch(Device *device);
void device_is_staion(Device *device);
void print_device(Device *device);
void device_from_config(Device *device, char * info);
char * device_to_config(Device *device);
void switch_print_table(Device *device);
