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
    // TESTING THE FRAME SENDING
    // for (int i = 0; i < network.num_devices; i++)
    // {
    //     if (network.devices[i].type == STATION)
    //     {
    //         Frame frame;
    //         for (int j = 0; j < network.num_devices; j++)
    //         {
    //             if (network.devices[j].type == STATION)
    //             {
    //                 frame_init(&frame, network.devices[i].mac_address, network.devices[j].mac_address, 0x0800, (uint8_t *)"Hello World");
    //                 send_frame_from_station(&network, &network.devices[i], &network.devices[j], &frame);
    //             }
    //         }
    //     }
    //     else
    //     {
    //         Frame frame;
    //         for (int j = 0; j < network.num_devices; j++)
    //         {
    //             frame_init(&frame, network.devices[i].mac_address, network.devices[j].mac_address, 0x0800, (uint8_t *)"Hello World");
    //             send_frame_from_switch(&network, &network.devices[i], &network.devices[j], &frame);
    //         }
    //     }
    // }
    Frame frame;
    frame_init(&frame, network.devices[7].mac_address, network.devices[8].mac_address, 0x0800, (uint8_t *)"Hello World");
    send_frame(&network, &network.devices[7], NULL, &frame);
    // send_frame(&network, &network.devices[7], NULL, &frame);

    Frame frame2;
    frame_init(&frame2, network.devices[8].mac_address, network.devices[7].mac_address, 0x0800, (uint8_t *)"Hello World");
    // send_frame(&network, &network.devices[8], NULL, &frame2);
    // send_frame(&network, &network.devices[8], NULL, &frame2);

    network_print(&network);
    network_free(&network);
    return 0;
}