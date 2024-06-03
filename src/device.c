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

void device_free(Device *device)
{
    // This function should free the memory allocated for the device structure
    // Free the memory allocated for the ports if the device is a switch
    if (device->type == SWITCH)
    {
        free(device->switch_info.ports);
    }
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
    for (int i = 0; i < num_ports; i++)
    {
        device->switch_info.ports[i].state = 'F';
        device->switch_info.ports[i].role = 'D';
    }
    // Set the number of switching table entries to 0
    device->switch_info.switching_table_entries = 0;
    // Assign the BPDU
    assign_bpdu(device);
}

void assign_bpdu(Device *device)
{
    BPDU bpdu;
    bpdu.root_bridge_priority = device->switch_info.priority;
    bpdu.root_bridge_mac_address = device->mac_address;
    bpdu.root_path_cost = 0;
    bpdu.sender_mac_address = device->mac_address;
    bpdu.sender_priority = device->switch_info.priority;
    device->switch_info.bpdu = bpdu;
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

    char *output[4];
    char *info_copy = strdup(info);
    split(info_copy, ";", output);

    if (output[0][0] == '1')
    {
        station_init(device, mac_address_from_string(output[1]), ip_address_from_string(output[2]));
    }

    if (output[0][0] == '2')
    {
        switch_init(device, mac_address_from_string(output[1]), atoi(output[3]), atoi(output[2]));
    }
}

char *device_to_config(Device *device, char *out)
{
    // This function should convert the device structure to a string and return it

    // EXPECTED OUTPUT :
    // 2;01:45:23:a6:f7:01;8;1024" <-- Switch
    // 1;54:d6:a6:82:c5:01;130.79.80.1 <-- Station

    if (device->type == STATION)
    {
        char mac_address[MAC_BUFFER_SIZE];
        char ip_address[IP_BUFFER_SIZE];
        mac_address_to_string(&device->mac_address, mac_address);
        ip_address_to_string(&device->station_info.ip_address, ip_address);
        sprintf(out, "1;%s;%s", mac_address, ip_address);
    }

    if (device->type == SWITCH)
    {
        char *priority = (char *)malloc(sizeof(char) * 256);
        sprintf(priority, "%d", device->switch_info.priority);
        char *num_ports = (char *)malloc(sizeof(char) * 256);
        sprintf(num_ports, "%d", device->switch_info.num_ports);
        char mac_address[MAC_BUFFER_SIZE];
        mac_address_to_string(&device->mac_address, mac_address);
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
    if (device->switch_info.switching_table_entries > 0)
        switch_print_table(device->switch_info);
    else if (device->switch_info.switching_table_entries == 0)
    {
        // In red color
        printf("\033[1;31m");
        printf("Switching table is empty\n");
        // Reset color
        printf("\033[0m");
    }
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

void print_link(Link *link)
{
    // This function should print the link structure to the console
    // EXPECTED OUTPUT :
    //  Link between Station 1 and Station 2
    //  Weight : 10
    printf("------------------Link------------------\n");
    printf("Link between Device %d and Device %d\n", link->device1_index, link->device2_index);
    printf("Weight : %d\n", link->weight);
}

void switch_print_table(Switch switch_)
{
    // This function should print the switch table to the console
    printf("+---------------------+-------+\n");
    printf("| %-19s | %5s |\n", "MAC Address", "Port");
    printf("+---------------------+-------+\n");
    for (int i = 0; i < switch_.switching_table_entries; i++)
    {
        char output[MAC_BUFFER_SIZE];
        printf("| %-19s | %5d |\n", mac_address_to_string(&switch_.switching_table[i].mac_address, output), switch_.switching_table[i].port_number + 1);
    }
    printf("+---------------------+-------+\n");
}