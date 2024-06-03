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
    if (network_from_config(&network, "../config/mylan_no_cycle.lan") != true)
    {
        // Handle error if configuration fails
        network_free(&network);
        return 1; // or appropriate error code
    }
    // Sending BPDU frames
    send_bpdu(&network, &network.devices[0]);    
    network_print(&network);
    network_free(&network);
    return 0;
}