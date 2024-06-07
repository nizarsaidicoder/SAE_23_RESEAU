#include "headers/address.h"
#include "headers/utils.h"
#include <stdlib.h>
#include <string.h>
/**
 * @brief Converts an IP address to a string
 * @param ip_address The IP address to convert
 * @param output The string to output the IP address to
 * @return The IP address as a string
 */
char *ip_address_to_string(IPAddress *ip_address, char *output)
{
    sprintf(output, "%d.%d.%d.%d", ip_address->address[0], ip_address->address[1], ip_address->address[2], ip_address->address[3]);
    return output;
}
/**
 * @brief Converts a MAC address to a string
 * @param mac_address The MAC address to convert
 * @param output The string to output the MAC address to
 * @return The mac address as a string
 */
char *mac_address_to_string(MACAddress *mac_address, char *output)
{
    sprintf(output, "%02x:%02x:%02x:%02x:%02x:%02x", mac_address->address[0], mac_address->address[1], mac_address->address[2], mac_address->address[3], mac_address->address[4], mac_address->address[5]);
    return output;
}
/**
 * @brief Prints an IP address
 * @param ip_address The IP address to print
 */
void print_ip_address(IPAddress *ip_address)
{
    char output[IP_BUFFER_SIZE];
    ip_address_to_string(ip_address, output);
    printf("IP ADDRESS : \t %s\n", output);
}

/**
 * @brief Prints a MAC address
 * @param mac_address The MAC address to print
 */
void print_mac_address(MACAddress *mac_address)
{
    char output[MAC_BUFFER_SIZE];
    mac_address_to_string(mac_address, output);
    printf("MAC ADDRESS : \t %s\n", output);
}
/**
 * @brief Converts a string to an IP address
 * @param string The string to convert
 * @return The IP address
 */
IPAddress ip_address_from_string(char *string)
{
    char *out[4];
    IPAddress result;
    uint8_t num = split(string, ".", out);
    for (int i = 0; i < num; i++)
    {
        result.address[i] = atoi(out[i]);
    }
    for (int i = num; i < 4; i++)
    {
        free(out[i]);
    }
    return result;
}

/**
 * @brief Converts a string to a MAC address
 * @param string The string to convert
 * @return The MAC address
 */
MACAddress mac_address_from_string(char *string)
{
    char *out[6];
    MACAddress result;
    uint8_t num = split(string, ":", out);
    for (int i = 0; i < num; i++)
    {
        result.address[i] = hex_to_int(out[i]);
    }
    for (int i = num; i < 6; i++)
    {
        free(out[i]);
    }
    return result;
}
/**
 * @brief Compares two MAC addresses
 * @param mac1 The first MAC address
 * @param mac2 The second MAC address
 * @return 1 if mac1 > mac2, -1 if mac1 < mac2, 0 if mac1 == mac2
 */
int compare_mac_address(MACAddress *mac1, MACAddress *mac2)
{
    for (int i = 0; i < 6; i++)
    {
        if (mac1->address[i] > mac2->address[i])
        {
            return 1;
        }
        if (mac1->address[i] < mac2->address[i])
        {
            return -1;
        }
    }
    return 0;
}
