#include "headers/address.h"
#include <stdlib.h>
#include <string.h>
char *ip_address_to_string(IPAddress *ip_address)
{
    // This function should convert the IP address to a string
    // EXPECTED INPUT : {130, 79, 80, 1}
    // EXPECTED OUTPUT : "130.79.80.1"
    char * output;
    sprintf(output, "%d.%d.%d.%d", ip_address->address[0], ip_address->address[1], ip_address->address[2] ,ip_address->address[3]);
    return output;
    
}

char *mac_address_to_string(MACAddress *mac_address)
{
    // This function should convert the MAC address to a string
    // EXPECTED INPUT : {0, 26, 43, 60, 77, 94}
    // EXPECTED OUTPUT : "00:1A:2B:3C:4D:5E"
}

void print_ip_address(IPAddress *ip_address)
{
    // This function should print the IP address to the console
    // EXPECTED INPUT : {130, 79, 80, 1}
    // EXPECTED OUTPUT : 130.79.80.1
}

void print_mac_address(MACAddress *mac_address)
{
    // This function should print the MAC address to the console
    // EXPECTED INPUT : {0, 26, 43, 60, 77, 94}
    // EXPECTED OUTPUT : 00:1A:2B:3C:4D:5E
}

IPAddress ip_address_from_string(char *string)
{
    // This function should convert a string to an IP address

    // EXPECTED INPUT : "130.79.80.1"
    // EXPECTED OUTPUT : {130, 79, 80, 1}
    char ** out;
    IPAddress result;
    uint8_t num = split(string, '.', out);
    for(int i = 0; i < num; i++)
    {
        result.address[i] = atoi(out[i]);
    }

    return result;
}

MACAddress mac_address_from_string(char *string)
{
    // This function should convert a string to a MAC address

    // EXPECTED INPUT : "00:1A:2B:3C:4D:5E"
    // EXPECTED OUTPUT : {0, 26, 43, 60, 77, 94}

    char ** out;
    MACAddress result;
    uint8_t num = split(string, ':', out);
    for(int i = 0; i < num; i++)
    {
        result.address[i] = hex_to_int(out[i]);
    }
    return result;
}