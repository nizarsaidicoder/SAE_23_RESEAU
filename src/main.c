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
    network_free(&network);
    return 0;
}