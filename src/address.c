#include "headers/address.h"

char *ip_address_to_string(IPAddress *ip_address)
{
    // This function should convert the IP address to a string
    // EXPECTED INPUT : {130, 79, 80, 1}
    // EXPECTED OUTPUT : "130.79.80.1"
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
}

MACAddress mac_address_from_string(char *string)
{
    // This function should convert a string to a MAC address

    // EXPECTED INPUT : "00:1A:2B:3C:4D:5E"
    // EXPECTED OUTPUT : {0, 26, 43, 60, 77, 94}
}