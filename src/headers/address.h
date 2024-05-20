#pragma once
#include "types.h"

void print_ip_address(IPAddress *ip_address);
void print_mac_address(MACAddress *mac_address);

char *ip_address_to_string(IPAddress *ip_address);
char *mac_address_to_string(MACAddress *mac_address);

IPAddress ip_address_from_string(char *string);
MACAddress mac_address_from_string(char *string);