#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/spanning_tree_protocol.h"
int main()
{
    Network network;
    network_init(&network);
    if (!network_from_config(&network, "../config/mylan.lan"))
    {
        // Handle error if configuration fails
        network_free(&network);
        return 1; // or appropriate error code
    }
    // Sending BPDU frames
    spanning_tree_protocol(&network);

    printf("---------------------------------------------------------------------------------\n");

    Frame frame;
    frame_init(&frame, network.devices[7].mac_address, network.devices[14].mac_address, 0x800, "Hello");
    // frame_print_data_user_mode(&frame);
    send_frame(&network, &network.devices[7], NULL, &frame);
    // network_print(&network);
    network_free(&network);
    return 0;
}