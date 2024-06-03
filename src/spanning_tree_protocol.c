#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);
}

void elect_root_bridge(Network *network)
{

}

uint16_t find_shortest_path(Network * network, Device *device, Device *destination, int path[])
{
    return 0;
}