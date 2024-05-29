#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
int main()
{
    Network network;
    network_init(&network);
    Device device;
    MACAddress mac_address;
    mac_address.address[0] = 0x00;
    mac_address.address[1] = 0x11;
    mac_address.address[2] = 0x22;
    mac_address.address[3] = 0x33;
    mac_address.address[4] = 0x44;
    mac_address.address[5] = 0x55;
    device.mac_address = mac_address;
    device.index = 0;
    device.type = STATION;
    IPAddress ip_address;
    ip_address.address[0] = 192;
    ip_address.address[1] = 168;
    ip_address.address[2] = 1;
    ip_address.address[3] = 1;
    Station station;
    station.ip_address = ip_address;
    device.type = STATION;
    device.station_info = station;
    network_add_device(&network, &device);
    network_print(&network);
    char hex[10];
    return 0;
}