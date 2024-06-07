#include "headers/device.h"
#include "headers/utils.h"

void device_init(Device *device, MACAddress mac_address, DeviceType type)
{
    // This function should initialize the device structure
    device->mac_address = mac_address;
    device->type = type;
}

void device_free(Device *device)
{
    // This function should free the memory allocated for the device structure
    if (device->type == SWITCH)
    {
        free(device->switch_info.ports);
    }
}

void station_init(Device *device, MACAddress mac_address, IPAddress ip_address)
{
    // This function should initialize the station structure
    device_init(device, mac_address, STATION);
    device->station_info.ip_address = ip_address;
}

void switch_init(Device *device, MACAddress mac_address, uint16_t priority, uint8_t num_ports)
{
    // This function should initialize the device structure
    device_init(device, mac_address, SWITCH);
    device->switch_info.priority = priority;
    device->switch_info.num_ports = num_ports;
    device->switch_info.ports = (Port *)malloc(sizeof(Port) * num_ports);
    for (int i = 0; i < num_ports; i++)
    {
        device->switch_info.ports[i].state = 'F';
        device->switch_info.ports[i].role = 'D';
    }
    device->switch_info.switching_table_entries = 0;
    assign_bpdu(device);
}

void device_from_config(Device *device, char *info)
{
    // This function should read the device structure from a line and populate the device structure
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

char *device_to_config(Device *device, char *out)
{
    // This function should convert the device structure to a string and return it

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
    printf("------------------Station %d------------------\n", device->index);
    print_mac_address(&device->mac_address);
    print_ip_address(&device->station_info.ip_address);
}

void print_switch(Device *device)
{
    // This function should print the switch structure to the console

    printf("------------------Switch %d--------------------\n", device->index + 1);
    print_mac_address(&device->mac_address);
    printf("Priority : %d\n", device->switch_info.priority);
    printf("Number of Ports : %d\n", device->switch_info.num_ports);
    if (device->switch_info.switching_table_entries > 0)
        switch_print_table(device->switch_info);
    else if (device->switch_info.switching_table_entries == 0)
    {
        // In red color INSIDE a rectangle
        printf("\033[1;31m");
        printf("\t+---------------------+-------+\n");
        printf("\t| %-19s | %5s |\n", "MAC Address", "Port");
        printf("\t+---------------------+-------+\n");
        printf("\t| %-19s | %5s |\n", "No entries", "No entries");
        printf("\t+---------------------+-------+\n");
    }
    switch_print_ports(device->switch_info);
}

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

void switch_print_table(Switch switch_)
{
    // This function should print the switch table to the console
    printf("+---------------------+-------+\n");
    printf("| %-19s | %5s |\n", "MAC Address", "Port");
    printf("+---------------------+-------+\n");
    // In green color mac address
    // In yellow color port
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

void switch_print_ports(Switch switch_)
{
    // This function should print the switch ports to the console
    printf("+-------+-------+-------+\n");
    printf("| Port  | State | Role  |\n");
    printf("+-------+-------+-------+\n");
    for (int i = 0; i < switch_.num_ports; i++)
    {
        // if state is F, print in green color
        // if state is B, print in red color
        // if role is D, print in blue color
        // if role is R, print in yellow color
        // if role is B, print in red color
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
        printf("   |\n");
    }
    printf("+-------+-------+-------+\n");
}