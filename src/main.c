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
    printf("Welcome to the network simulator!\n");
    sleep(1);
    printf("This is a simple network simulator that simulates a network with devices and switches.\n");
    sleep(1);
    printf("It can simulate frame transmission and Spanning Tree Protocol.\n");
    sleep(1);
    printf("It can also simulate a network with no cycle and a network with cycle.\n");
    sleep(1);
    printf("Press a key to continue...");
    getchar();
    int choice = -1;
    while (choice != 1 && choice != 2)
    {
        printf("Press (1) to simulate a network with no cycle\nPress (2) to simulate a network with cycle\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input!\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (choice == 1)
            network_no_cycle();
        else if (choice == 2)
            network_cycle();
        else
            printf("Invalid choice!\n");
    }
    printf("Closing the network simulator...\n Goodbye!\n");
    return 0;
}

void network_no_cycle()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan_no_cycle.lan");
    printf("Press a key to see the network before sending a frame...");
    getchar();
    getchar();
    network_print(&network);
    Frame frame;
    printf("Press a key to see to send a frame...");
    getchar();
    getchar();
    frame_init(&frame, network.devices[8].mac_address, network.devices[14].mac_address, 0x800, "Hello from the other side!\nI must have called a thousand times!\nTo tell you I'm sorry for all frames I've sent!\nBut when I call you never seem to be home!\n So I BROADCAST!");
    send_frame(&network, &network.devices[8], NULL, &frame);
    printf("Press a key to see the network after sending a frame...");
    getchar();
    network_print(&network);
    printf("Press a key to exit...");
    getchar();
    network_free(&network);
}

void network_cycle()
{
    Network network;
    network_init(&network);
    network_from_config(&network, "../config/mylan.lan");
    printf("Press a key to see the network before activating the Spanning tree protocol...");
    getchar();
    getchar();
    network_print(&network);
    printf("Press a key to see the network after activating the Spanning tree protocol...");
    getchar();
    spanning_tree_protocol(&network);
    network_print(&network);
    printf("Press a key to send a frame...");
    getchar();
    Frame frame;
    frame_init(&frame, network.devices[7].mac_address, network.devices[14].mac_address, 0x900, "Well ! This SAE proved that switches are very dumb!");
    send_frame(&network, &network.devices[7], NULL, &frame);
    printf("Press a key to see the network after sending a frame...");
    getchar();
    network_print(&network);
    printf("Press a key to exit...");
    getchar();
    network_free(&network);
}