#include "headers/types.h"
#include "headers/network.h"
#include "headers/device.h"
#include "headers/utils.h"
void network_init(Network *network)
{
    // This function should initialize the network structure
    // Initialize the number of devices and links to 0
    network->num_devices = 0;
    network->num_links = 0;
    network->num_stations = 0;
    network->num_switches = 0;
    // Allocate memory for the devices and links
    network->devices = (Device *)malloc(10 * sizeof(Device));
    network->links = (Link *)malloc(10 * sizeof(Link));
    // Set the link capacity to 10
    network->link_capacity = 10;
    network->device_capacity = 10;
}
void network_free(Network *network)
{
    // This function should free the memory allocated for the network structure
    // Free the memory allocated for the devices and links
    free(network->devices);
    free(network->links);
    network->devices = NULL;
    network->links = NULL;
    // Set the number of devices and links to 0
    network->num_devices = 0;
    network->num_links = 0;
    network->num_stations = 0;
    network->num_switches = 0;
    network->link_capacity = 0;
    network->device_capacity = 0;
}

uint8_t network_num_devices(Network *network)
{
    // This function should return the number of devices in the network
    return network->num_devices;
}
uint8_t network_num_stations(Network *network)
{
    // This function should return the number of stations in the network
    return network->num_stations;
}
uint8_t network_num_switches(Network *network)
{
    // This function should return the number of switches in the network
    return network->num_switches;
}

uint16_t network_num_links(Network *network)
{
    // This function should return the number of links in the network
    return network->num_links;
}

void network_add_device(Network *network, Device *device)
{
    // This function should add a device to the network
    // Check if the devices array is full
    // If the devices array is full, reallocate memory for the devices array
    if (network->num_devices >= network->device_capacity)
    {
        network->device_capacity *= 2;
        network->devices = (Device *)realloc(network->devices, network->device_capacity * sizeof(Device));
    }
    // Add the device to the devices array
    network->devices[network->num_devices] = *device;
    // Increment the number of devices in the network
    network->num_devices++;
    // if the device is a station, increment the number of stations
    if (device->type == STATION)
        network->num_stations++;
    // if the device is a switch, increment the number of switches
    if (device->type == SWITCH)
        network->num_switches++;
}
bool network_link_exists(Network *network, Link link)
{
    // This function should check if a link already exists in the network
    for (int i = 0; i < network->num_links; i++)
    {
        if (network->links[i].device1_index == link.device1_index && network->links[i].device2_index == link.device2_index)
            return true;
        if (network->links[i].device2_index == link.device1_index && network->links[i].device1_index == link.device2_index)
            return true;
    }
    return false;
}
bool network_add_link(Network *network, Link link)
{
    // This function should add a link to the network
    if (network_link_exists(network, link))
        return false;

    if (network->num_links >= network->link_capacity)
    {
        network->link_capacity *= 2;
        network->links = (Link *)realloc(network->links, network->link_capacity * sizeof(Link));
    }
    // Add the link to the links array
    network->links[network->num_links] = link;
    network->num_links++;
    return true;
}
Device *network_find_device(Network *network, MACAddress mac_address)
{
    // This function should return a pointer to the device at the given index
    for (int i = 0; i < network->num_devices; i++)
    {
        if (network->devices[i].mac_address.address == mac_address.address)
            return &network->devices[i];
    }
    return NULL;
}
void network_print(Network *network)
{
    // This function should print the network structure to the console
    // Print the number of devices, stations, switches, and links
    printf("+---------------------+-------+\n");
    printf("| %-19s | %5s |\n", "Category", "Count");
    printf("+---------------------+-------+\n");
    printf("| %-19s | %5d |\n", "Devices", network->num_devices);
    printf("| %-19s | %5d |\n", "Stations", network->num_stations);
    printf("| %-19s | %5d |\n", "Switches", network->num_switches);
    printf("| %-19s | %5d |\n", "Links", network->num_links);
    printf("+---------------------+-------+\n");
    // Print the details of each device in the network
    for (int i = 0; i < network->num_devices; i++)
    {
        print_device(&network->devices[i]);
    }
}
void network_from_config(Network *network, char *filename)
{
    // This function should read the network structure from a file and populate the network structure
    char **lines = (char **)malloc(100 * sizeof(char *));
    int num_lines = read_lines(filename, lines);
    // Read the number of devices and links from the file
    char *config_header[2];
    split(lines[0], ' ', config_header);
    uint8_t num_devices = atoi(config_header[0]);
    uint16_t num_links = atoi(config_header[1]);
    network_init(network);
    // Read the details of each device from the file
    for (int i = 1; i < num_devices; i++)
    {
        Device device;
        device_from_config(&device, lines[i]);
        network_add_device(network, &device);
    }
    // Read the details of each link from the file
    for (int i = num_devices + 1; i < num_lines; i++)
    {
        Link link;
        char *link_info[3];
        split(lines[i], ';', link_info);
        link.device1_index = atoi(link_info[0]);
        link.device2_index = atoi(link_info[1]);
        link.weight = atoi(link_info[2]);
        network_add_link(network, link);
    }
}
void network_to_config(Network *network, char *filename)
{
    // This function should write the network structure to a file
    // Write the number of devices and links to the file
    char network_header[256];
    sprintf(network_header, "%d;%d", network->num_devices, network->num_links);
    char **lines = (char **)malloc((network->num_devices + network->num_links + 1) * sizeof(char *));
    lines[0] = strdup(network_header);
    for (int i = 1; i < network->num_devices; i++)
    {
        char *device_config = device_to_config(&network->devices[i]);
        lines[i] = strdup(device_config);
    }
    for (int i = network->num_devices; i < network->num_links; i++)
    {
        char link_info[256];
        sprintf(link_info, "%d;%d;%d", network->links[i].device1_index, network->links[i].device2_index, network->links[i].weight);
        lines[i] = strdup(link_info);
    }
    write_lines(filename, lines);
}
