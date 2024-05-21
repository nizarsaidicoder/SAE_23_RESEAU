#include "headers/device.h"

void device_init(Device *device, MACAddress mac_address, DeviceType type)
{
    // This function should initialize the device structure
    // Set the MAC address
    // Set the device type
}

void station_init(Device *device, MACAddress mac_address, IPAddress ip_address)
{
    // This function should initialize the station structure
    // CALL device_init
    // Set the IP address
}

void switch_init(Device *device, uint16_t priority, uint8_t num_ports)
{
    // This function should initialize the device structure
    // CALL station_init
    // Set the priority
    // Set the number of ports
    // Allocate memory for the ports
    // Allocate memory for the switching table
}
bool device_is_switch(Device *device)
{
    // This function should return true if the device is a switch

    // return true if the device is a switch
}

bool device_is_station(Device *device)
{
    // This function should return true if the device is a station

    // return true if the device is a station
}
void print_station(Device *device)
{
    // This function should print the station structure to the console
    // EXPECTED OUTPUT :
    //  **************Station 1**************
    //  MAC Address : 00:1A:2B:3C:4D:5E
    //  IP Address : 130.194.30.2
}

void print_switch(Device *device)
{
    // This function should print the switch structure to the console
    // EXPECTED OUTPUT :
    //  **************Switch 1**************
    //  MAC Address : 00:1A:2B:3C:4D:5E
    //  Priority : 100
    //  Number of Ports : 4
}

void print_device(Device *device)
{
    // This function should print the device structure to the console
    // based on the device type, print either the station or the switch
    // If the device is a station, call print_station
    // If the device is a switch, call print_switch
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