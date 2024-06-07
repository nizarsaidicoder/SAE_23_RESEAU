#include "headers/types.h"
#include "headers/network.h"
#include "headers/device.h"
#include "headers/utils.h"

#define MAX_CAPACITY 10
/**
 * @brief Initializes a network
 * @param network The network to initialize
 */
void network_init(Network *network)
{

    network->num_devices = 0;
    network->num_links = 0;
    network->num_stations = 0;
    network->num_switches = 0;
    network->link_capacity = MAX_CAPACITY;
    network->device_capacity = MAX_CAPACITY;
    network->devices = (Device *)malloc(network->device_capacity * sizeof(Device));
    network->links = (Link *)malloc(network->link_capacity * sizeof(Link));
}
/**
 * @brief Frees the memory allocated for a network
 * @param network The network to free
 */
void network_free(Network *network)
{
    for (int i = 0; i < network->num_devices; i++)
    {
        if (network->devices[i].type == SWITCH)
            free(network->devices[i].switch_info.ports);
    }
    free(network->devices);
    network->devices = NULL;
    free(network->links);
    network->links = NULL;
    network->num_devices = 0;
    network->num_links = 0;
    network->num_stations = 0;
    network->num_switches = 0;
    network->link_capacity = 0;
    network->device_capacity = 0;
}
/**
 * @brief Returns the number of devices in the network
 * @param network The network
 */
uint8_t network_num_devices(Network *network)
{
    return network->num_devices;
}
/**
 * @brief Returns the number of stations in the network
 * @param network The network

 */
uint8_t network_num_stations(Network *network)
{
    return network->num_stations;
}
/**
 * @brief Returns the number of switches in the network
 * @param network The network
 */
uint8_t network_num_switches(Network *network)
{
    return network->num_switches;
}
/**
 * @brief Returns the number of links in the network
 * @param network The network
 */
uint16_t network_num_links(Network *network)
{
    // This function should return the number of links in the network
    return network->num_links;
}
/**
 * @brief Adds a device to the network
 * @param network The network
 * @param device The device to add
 * @note The device should be already initialized
 */
void network_add_device(Network *network, Device *device)
{
    // Check if the devices array is full
    // If the devices array is full, reallocate memory for the devices array
    if (network->num_devices >= network->device_capacity)
    {
        network->device_capacity *= 2;
        network->devices = (Device *)realloc(network->devices, network->device_capacity * sizeof(Device));
    }
    device->index = network->num_devices;
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
/**
 * @brief Checks if a link already exists in the network
 * @param network The network
 * @param link The link to check
 * @return true if the link already exists in the network
 */
bool network_link_exists(Network *network, Link *link)
{
    for (int i = 0; i < network->num_links; i++)
    {
        if (network->links[i].device1_index == link->device1_index && network->links[i].device2_index == link->device2_index)
            return true;
        if (network->links[i].device2_index == link->device1_index && network->links[i].device1_index == link->device2_index)
            return true;
    }
    return false;
}
/**
 * @brief Returns the index of a link in the network
 * @param network The network
 * @param link The link to find
 * @return The index of the link in the network
 */
uint8_t network_link_index(Network *network, Link *link)
{
    for (int i = 0; i < network->num_links; i++)
    {
        if (network_link_exists(network, link))
            return i;
    }
    return -1;
}
/**
 * @brief Returns the weight of the link between two devices
 * @param network The network
 * @param device1_index The index of the first device
 * @param device2_index The index of the second device
 * @return The weight of the link between the two devices
 */
uint16_t network_link_weight(Network *network, uint8_t device1_index, uint8_t device2_index)
{
    for (int i = 0; i < network->num_links; i++)
    {
        if (network->links[i].device1_index == device1_index && network->links[i].device2_index == device2_index)
            return network->links[i].weight;
        if (network->links[i].device2_index == device1_index && network->links[i].device1_index == device2_index)
            return network->links[i].weight;
    }
    return -1;
}
/**
 * @brief Adds a link to the network
 * @param network The network
 * @param link The link to add
 * @return true if the link was added successfully
 */
bool network_add_link(Network *network, Link *link)
{
    if (network_link_exists(network, link))
        return false;
    // Reallocate memory for the links array if it is full
    if (network->num_links >= network->link_capacity)
    {
        network->link_capacity *= 2;
        network->links = (Link *)realloc(network->links, network->link_capacity * sizeof(Link));
    }
    // Add the link to the links array
    network->links[network->num_links] = *link;
    network->num_links++;
    return true;
}
/**
 * @brief Finds a device in the network by its MAC address
 * @param network The network
 * @param mac_address The MAC address of the device to find
 * @return A pointer to the device if found, NULL otherwise
 */
Device *network_find_device(Network *network, MACAddress *mac_address)
{
    // This function should return a pointer to the device at the given index
    for (int i = 0; i < network->num_devices; i++)
    {
        if (compare_mac_address(&network->devices[i].mac_address, mac_address))
            return &network->devices[i];
    }
    return NULL;
}
/**
 * @brief Prints the network structure
 * @param network The network to print
 */
void network_print(Network *network)
{

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
    // Print the details of each link in the network
    for (int i = 0; i < network->num_links; i++)
    {
        print_link(&network->links[i]);
    }
}
/**
 * @brief Configure a network structure from a file
 * @param network The network to read into
 * @param filename The name of the file to read from
 * @return true if the network was read successfully
 */
bool network_from_config(Network *network, char *filename)
{
    char *lines[100];
    uint16_t num_lines = read_lines(filename, lines);
    // Read the number of devices and links from the file
    char *config_header[2];
    split(lines[0], " ", config_header);
    uint8_t num_devices = atoi(config_header[0]);

    // Read the details of each device from the file
    for (int i = 1; i <= num_devices; i++)
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
        split(lines[i], ";", link_info);
        link.device1_index = atoi(link_info[0]);
        link.device2_index = atoi(link_info[1]);
        link.weight = atoi(link_info[2]);
        network_add_link(network, &link);
    }
    for (int i = 0; i < num_lines; i++)
    {
        free(lines[i]);
    }
    return true;
}
/**
 * @brief Finds the devices connected to a device
 * @param network The network
 * @param device_index The index of the device
 * @param connected_devices The array of connected devices to fill
 * @return The number of connected devices
 */
uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[])
{
    if (device_index >= network->num_devices)
    {
        printf("Device index out of bounds\n");
        return 0;
    }
    uint16_t connected_devices_count = 0;
    for (int i = 0; i < network->num_links; i++)
    {
        if (network->links[i].device1_index == device_index)
        {
            connected_devices[connected_devices_count] = &network->devices[network->links[i].device2_index];
            connected_devices_count++;
        }
        else if (network->links[i].device2_index == device_index)
        {
            connected_devices[connected_devices_count] = &network->devices[network->links[i].device1_index];
            connected_devices_count++;
        }
    }
    return connected_devices_count;
}