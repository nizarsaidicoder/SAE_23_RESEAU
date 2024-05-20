#include "headers/device.h"
void station_init(Device *device)
{
    // This function should initialize the station structure
}

void switch_init(Device *device)
{
    // This function should initialize the device structure
}
void device_is_switch(Device *device)
{
    // This function should return true if the device is a switch
}

void device_is_staion(Device *device)
{
    // This function should return true if the device is a station
}

void print_device(Device *device)
{
    // This function should print the device structure to the console
    // based on the device type, print either the station or the switch
}

void device_from_config(Device *device, char *info)
{
    // This function should read the device structure from a line and populate the device structure
}

char *device_to_config(Device *device)
{
    // This function should convert the device structure to a string and return it
}

void switch_print_table(Device *device)
{
    // This function should print the switch table to the console
}