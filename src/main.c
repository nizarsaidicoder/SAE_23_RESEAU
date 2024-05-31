#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
int main()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan.lan");
    network_print(&network);
    return 0;
}