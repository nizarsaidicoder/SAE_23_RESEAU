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
    mac_address.address[0] = 23;
    mac_address.address[1] = 255;
    mac_address.address[2] = 11;
    mac_address.address[3] = 44;
    mac_address.address[4] = 31;
    mac_address.address[5] = 1;
    print_mac_address(&mac_address);
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
    // network_print(&network);

    Device dev2;
    device_from_config(&dev2, "2;01:45:23:a6:f7:01;8;1024");
    print_device(&dev2);

    printf("%s\n", device_to_config(&dev2));

    Device dev3;
    device_from_config(&dev3, "1;54:d6:a6:82:c5:01;130.79.80.1");
    print_device(&dev3);

    printf("%s\n", device_to_config(&dev3));
    return 0;
}