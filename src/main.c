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
    if (network_from_config(&network, "../config/mylan_no_cycle.lan") != true)
    {
        // Handle error if configuration fails
        network_free(&network);
        return 1; // or appropriate error code
    }
    Frame frame;
    frame_init(&frame, network.devices[0].mac_address, network.devices[1].mac_address, 0x00, "Finnaly it works !\nAfter a lot of debugging and testing !\nAfter drinking a lot of coffee !\nAfter drinking a lot of paracetamol !\nAfter drinking a lot of Whisky !!!!!!!!!!!!");
    send_frame(&network, &network.devices[0], NULL, &frame);
    network_print(&network);
    network_free(&network);
    return 0;
}