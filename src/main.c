#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
#include "headers/frame.h"
int main()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan_no_cycle.lan");
    // network_print(&network);
    // TESTING THE FRAME SENDING
    Frame frame;
    frame_init(&frame, network.devices[8].mac_address, network.devices[14].mac_address, 0x0800, (uint8_t *)"Hello World");
    send_frame_from_station(&network, &network.devices[8], &network.devices[14], &frame);
    print_switching_tables(&network);

    Frame frame2;
    frame_init(&frame2, network.devices[14].mac_address, network.devices[8].mac_address, 0x0800, (uint8_t *)"Hello World");
    send_frame_from_station(&network, &network.devices[14], &network.devices[8], &frame2);
    send_frame_from_station(&network, &network.devices[14], &network.devices[8], &frame2);
    send_frame_from_station(&network, &network.devices[8], &network.devices[14], &frame);

    print_switching_tables(&network);
    return 0;
}