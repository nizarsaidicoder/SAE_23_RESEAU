#include "headers/types.h"
#include "headers/network.h"
void network_init(Network *network)
{
    // This function should initialize the network structure
    // Initialize the number of devices and links to 0
    // Allocate memory for the devices and links
    // Set the device and link pointers to NULL for memory safety
}
uint8_t network_num_devices(Network *network)
{
    // This function should return the number of devices in the network
    return 0;
}
uint8_t network_num_stations(Network *network)
{
    // This function should return the number of stations in the network
    return 0;
}
uint8_t network_num_switches(Network *network)
{
    // This function should return the number of switches in the network
    return 0;
}

uint16_t network_num_links(Network *network)
{
    // This function should return the number of links in the network
    return 0;
}

void network_add_device(Network *network, Device *device)
{
    // This function should add a device to the network
    // Increment the number of devices in the network
    // if the device is a station, increment the number of stations
}
void network_add_link(Network *network, uint8_t device1_index, uint8_t device2_index)
{
    // This function should add a link to the network
    // Increment the number of links in the network
    // Add the link to the links array
}
Device *network_find_device(Network *network, uint8_t device_index)
{
    // This function should return a pointer to the device at the given index
    return NULL;
}
void network_print(Network *network)
{
    // This function should print the network structure to the console
    // Print the number of devices, stations, switches, and links
    // Print the details of each device in the network
    // Print the details of each link in the network
}
void network_from_config(Network *network, char *filename)
{
    // This function should read the network structure from a file and populate the network structure
    // Read the number of devices and links from the file
    // Read the details of each device from the file
    // Read the details of each link from the file
    // Populate the network structure with the read data
}
void network_to_config(Network *network, char *filename)
{
    // This function should write the network structure to a file
    // Write the number of devices and links to the file
    // Write the details of each device to the file
    // Write the details of each link to the file
}
