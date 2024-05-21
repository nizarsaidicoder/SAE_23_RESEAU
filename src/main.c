#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
int main()
{
    IPAddress ip = {{192, 168, 1, 1}};
    printf("IP Address: %d.%d.%d.%d\n", ip.address[0], ip.address[1], ip.address[2], ip.address[3]);
    return 0;
}