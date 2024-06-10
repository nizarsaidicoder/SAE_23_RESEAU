#include "headers/device.h"
/**
 * @brief Initializes a device
 * @param device The device to initialize
 * @param mac_address The MAC address of the device
 * @param type The type of the device (STATION or SWITCH)
 */
void device_init(Device *device, MACAddress mac_address, DeviceType type)
{
    device->mac_address = mac_address;
    device->type = type;
}
/**
 * @brief Frees the memory allocated for a device
 * @param device The device to free
 */
void device_free(Device *device)
{
    if (device->type == SWITCH)
    {
        free(device->switch_info.ports);
    }
}
/**
 * @brief Initializes a station
 * @param device The device to initialize
 * @param mac_address The MAC address of the station
 * @param ip_address The IP address of the station
 */
void station_init(Device *device, MACAddress mac_address, IPAddress ip_address)
{
    device_init(device, mac_address, STATION);
    device->station_info.ip_address = ip_address;
}
/**
 * @brief Initializes a switch
 * @param device The device to initialize
 * @param mac_address The MAC address of the switch
 * @param priority The priority of the switch
 * @param num_ports The number of ports on the switch
 */
void switch_init(Device *device, MACAddress mac_address, uint16_t priority, uint8_t num_ports)
{
    device_init(device, mac_address, SWITCH);
    device->switch_info.priority = priority;
    device->switch_info.num_ports = num_ports;
    device->switch_info.ports = (Port *)malloc(sizeof(Port) * num_ports);
    for (int i = 0; i < num_ports; i++)
    {
        device->switch_info.ports[i].state = 'F';
        device->switch_info.ports[i].role = 'U';
    }
    device->switch_info.switching_table_entries = 0;
    assign_bpdu(device);
}
/**
 * @brief Initialise a device from a configuration string
 * @param device The device to initialize
 * @param info The configuration string
 * @note The configuration string is of the format "type;mac_address;ip_address" for stations and "type;mac_address;num_ports;priority" for switches
 */
void device_from_config(Device *device, char *info)
{
    char *output[4];
    split(info, ";", output);

    if (output[0][0] == '1')
    {
        station_init(device, mac_address_from_string(output[1]), ip_address_from_string(output[2]));
    }

    if (output[0][0] == '2')
    {
        switch_init(device, mac_address_from_string(output[1]), atoi(output[3]), atoi(output[2]));
    }
}

/**
 * @brief Converts a device to a configuration string
 * @param device The device to convert
 * @param out The string to output the device to
 * @return The device as a string
 * @note This function is to use for advanced debugging purposes like saving the network to a file and viewing it later
 */
char *device_to_config(Device *device, char *out)
{
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

/**
 * @brief Checks if a device is a switch
 * @param device The device to check
 * @return true if the device is a switch
 */
bool device_is_switch(Device *device)
{
    return device->type == SWITCH;
}
/**
 * @brief Checks if a device is a station
 * @param device The device to check
 * @return true if the device is a station
 */
bool device_is_station(Device *device)
{
    return device->type == STATION;
}
/**
 * @brief Prints a station
 * @param device The station to print
 */
void print_station(Device *device)
{
    printf("------------------Station %d------------------\n", device->index);
    print_mac_address(&device->mac_address);
    print_ip_address(&device->station_info.ip_address);
}
/**
 * @brief Prints a switch
 * @param device The switch to print
 */
void print_switch(Device *device)
{
    printf("------------------Switch %d--------------------\n", device->index + 1);
    print_mac_address(&device->mac_address);
    printf("Priority : %d\n", device->switch_info.priority);
    printf("Number of Ports : %d\n", device->switch_info.num_ports);
    if (device->switch_info.switching_table_entries > 0)
        switch_print_table(device->switch_info);
    else if (device->switch_info.switching_table_entries == 0)
    {
        printf("\033[1;31m");
        printf("\t+---------------------+-------+\n");
        printf("\t| %-19s | %5s |\n", "MAC Address", "Port");
        printf("\t+---------------------+-------+\n");
        printf("\t| %-19s | %5s |\n", "No entries", "");
        printf("\t+---------------------+-------+\n");
        printf("\033[0m");
    }
    switch_print_ports(device->switch_info);
}
/**
 * @brief Prints a device
 * @param device The device to print
 * @note this function call either print_station or print_switch based on the device type
 */
void print_device(Device *device)
{
    // This function should print the device structure to the console
    // based on the device type, print either the station or the switch
    if (device->type == STATION)
    {
        print_station(device);
    }
    if (device->type == SWITCH)
    {
        print_switch(device);
    }
}
/**
 * @brief prints a link
 * @param link The link to print
 */
void print_link(Link *link)
{
    // This function should print the link structure to the console
    printf("------------------Link------------------\n");
    printf("Link between Device %d and Device %d\n", link->device1_index, link->device2_index);
    printf("Weight : ");
    if (link->weight == 0)
    {
        printf("\033[1;31m");
        printf("%d", link->weight);
        printf("\033[0m");
    }
    if (link->weight > 0)
    {
        printf("\033[1;33m");
        printf("%d", link->weight);
        printf("\033[0m");
    }
    if (link->weight > 4)
    {
        printf("\033[1;32m");
        printf("%d", link->weight);
        printf("\033[0m");
    }
    printf("\n");
}
/**
 * @brief prints the switch mac address table
 * @param switch_ The switch to print the table of
 */
void switch_print_table(Switch switch_)
{
    printf("+---------------------+-------+\n");
    printf("| %-19s | %5s |\n", "MAC Address", "Port");
    printf("+---------------------+-------+\n");
    char out[MAC_BUFFER_SIZE];
    for (int i = 0; i < switch_.switching_table_entries; i++)
    {
        printf("| ");
        printf("\033[1;32m");
        printf("%-19s", mac_address_to_string(&switch_.switching_table[i].mac_address, out));
        printf("\033[0m");
        printf(" | ");
        printf("\033[1;33m");
        printf("%5d", i + 1);
        printf("\033[0m");
        printf(" |\n");
    }
    printf("+---------------------+-------+\n");
}
/**
 * @brief prints the switch ports
 * @param switch_ The switch to print the ports of
 */
void switch_print_ports(Switch switch_)
{
    printf("+-------+-------+-------+\n");
    printf("| Port  | State | Role  |\n");
    printf("+-------+-------+-------+\n");
    for (int i = 0; i < switch_.num_ports; i++)
    {
        printf("|   %d   |   ", i + 1);
        if (switch_.ports[i].state == 'F')
        {
            printf("\033[1;32m");
            printf("%c", switch_.ports[i].state);
            printf("\033[0m");
        }
        if (switch_.ports[i].state == 'B')
        {
            printf("\033[1;31m");
            printf("%c", switch_.ports[i].state);
            printf("\033[0m");
        }
        printf("   |   ");
        if (switch_.ports[i].role == 'D')
        {
            printf("\033[1;34m");
            printf("%c", switch_.ports[i].role);
            printf("\033[0m");
        }
        if (switch_.ports[i].role == 'R')
        {
            printf("\033[1;33m");
            printf("%c", switch_.ports[i].role);
            printf("\033[0m");
        }
        if (switch_.ports[i].role == 'B')
        {
            printf("\033[1;31m");
            printf("%c", switch_.ports[i].role);
            printf("\033[0m");
        }
        if (switch_.ports[i].role == 'U')
        {
            printf("\033[1;35m");
            printf(" ");
            printf("\033[0m");
        }
        printf("   |\n");
    }
    printf("+-------+-------+-------+\n");
}