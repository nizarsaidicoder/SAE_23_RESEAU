#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/spanning_tree_protocol.h"
void network_no_cycle();
void network_cycle();
int main()
{
    // network_no_cycle();
    network_cycle();
    return 0;
}

void network_no_cycle()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan_no_cycle.lan");
    Frame frame;
    frame_init(&frame, network.devices[8].mac_address, network.devices[14].mac_address, 0x900, "Hello from the other side!\nI must have called a thousand times!\nTo tell you I'm sorry for all frames I've sent!\nBut when I call you never seem to be home!\n So I BROADCAST!");
    send_frame(&network, &network.devices[8], NULL, &frame);
    network_print(&network);
    network_free(&network);
}

void network_cycle()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan.lan");
    // Sending BPDU frames
    spanning_tree_protocol(&network);
    Frame frame;
    frame_init(&frame, network.devices[7].mac_address, network.devices[14].mac_address, 0x900, "Well ! This SAE proved that switches are very dumb!");
    send_frame(&network, &network.devices[7], NULL, &frame);
    network_print(&network);
    network_free(&network);
}