#include "headers/device.h"
#include "headers/utils.h"

void device_init(Device *device, MACAddress mac_address, DeviceType type)
{
    // This function should initialize the device structure
    // Set the MAC address
    device->mac_address = mac_address;
    // Set the device type
    device->type = type;
}

void station_init(Device *device, MACAddress mac_address, IPAddress ip_address)
{
    // This function should initialize the station structure
    // CALL device_init
    device_init(device, mac_address, STATION);
    // Set the IP address
    device->station_info.ip_address = ip_address;
}

void switch_init(Device *device, MACAddress mac_address, uint16_t priority, uint8_t num_ports)
{
    // This function should initialize the device structure
    // CALL device_init
    device_init(device, mac_address, SWITCH);
    // Set the priority
    device->switch_info.priority = priority;
    // Set the number of ports
    device->switch_info.num_ports = num_ports;
    // Allocate memory for the ports
    device->switch_info.ports = (Port *)malloc(15 * sizeof(Port *));
    // Allocate memory for the switching table
    device->switch_info.switching_table = (MACAddress *)malloc(10 * sizeof(MACAddress *));
}
void device_from_config(Device *device, char *info)
{
    // This function should read the device structure from a line and populate the device structure

    // EXPECTED INPUT :
    // 2;01:45:23:a6:f7:01;8;1024" <-- Switch
    // 1;54:d6:a6:82:c5:01;130.79.80.1 <-- Station

    /**
     * SOME SPECIFICATIONS
     * 1. The device type is the first element in the line : 1 for station, 2 for switch
     * 2. The MAC address is the second element in the line
     * If the device is a station, the IP address is the third element in the line
     * If the device is a switch, the priority and the number of ports are the third and fourth elements in the line
     */

    char **output = (char **)malloc(sizeof(char *) * 256);
    char *info_copy = strdup(info);
    char *delimiter = ";";
    split(info_copy, *delimiter, output);

    if (output[0][0] == '1')
    {
        station_init(device, mac_address_from_string(output[1]), ip_address_from_string(output[2]));
    }

    if (output[0][0] == '2')
    {
        switch_init(device, mac_address_from_string(output[1]), atoi(output[3]), atoi(output[2]));
    }
}

char *device_to_config(Device *device)
{
    // This function should convert the device structure to a string and return it

    // EXPECTED OUTPUT :
    // 2;01:45:23:a6:f7:01;8;1024" <-- Switch
    // 1;54:d6:a6:82:c5:01;130.79.80.1 <-- Station

    char *out = (char *)malloc(sizeof(char) * 256);

    if (device->type == STATION)
    {
        char *mac_address = mac_address_to_string(&device->mac_address);
        char *ip_address = ip_address_to_string(&device->station_info.ip_address);
        sprintf(out, "1;%s;%s", mac_address, ip_address);
    }

    if (device->type == SWITCH)
    {
        char *priority = (char *)malloc(sizeof(char) * 256);
        sprintf(priority, "%d", device->switch_info.priority);
        char *num_ports = (char *)malloc(sizeof(char) * 256);
        sprintf(num_ports, "%d", device->switch_info.num_ports);
        char *mac_address = mac_address_to_string(&device->mac_address);
        sprintf(out, "2;%s;%s;%s", mac_address, num_ports, priority);
    }

    return out;
}

bool device_is_switch(Device *device)
{
    // This function should return true if the device is a switch
    // return true if the device is a switch
    return device->type == SWITCH;
}

bool device_is_station(Device *device)
{
    // This function should return true if the device is a station
    // return true if the device is a station
    return device->type == STATION;
}

void print_station(Device *device)
{
    // This function should print the station structure to the console
    // EXPECTED OUTPUT :
    //  ------------------Station 1------------------
    //  MAC Address : 00:1A:2B:3C:4D:5E
    //  IP Address : 130.194.30.2
    printf("------------------Station %d------------------\n", device->index);
    print_mac_address(&device->mac_address);
    print_ip_address(&device->station_info.ip_address);
}

void print_switch(Device *device)
{
    // This function should print the switch structure to the console
    // EXPECTED OUTPUT :
    //  ------------------Switch 1--------------------
    //  MAC Address : 00:1A:2B:3C:4D:5E
    //  Priority : 100
    //  Number of Ports : 4
    printf("------------------Switch %d--------------------\n", device->index);
    print_mac_address(&device->mac_address);
    printf("Priority : %d\n", device->switch_info.priority);
    printf("Number of Ports : %d\n", device->switch_info.num_ports);
}

void print_device(Device *device)
{
    // This function should print the device structure to the console
    // based on the device type, print either the station or the switch
    // If the device is a station, call print_station
    if (device->type == STATION)
    {
        print_station(device);
    }
    // If the device is a switch, call print_switch
    if (device->type == SWITCH)
    {
        print_switch(device);
    }
}

void switch_print_table(Device *device)
{
    // This function should print the switch table to the console
    printf("---------------SWITCHING TABLE---------------\n");
    for(long unsigned int i = 0; i < sizeof(device->switch_info.switching_table); i++)
    {
        printf(" => Port %lu : \n", (i + 1));
        print_mac_address(&device->switch_info.switching_table[i]);
    }
}