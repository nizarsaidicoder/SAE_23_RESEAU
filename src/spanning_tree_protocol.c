#include "headers/spanning_tree_protocol.h"
/**
 * @brief This function activates the Spanning Tree Protocol
 * @param network The network
 * @note This function should be called after the network has been initialized and the devices have been added to the network
 */
void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);
    set_block_ports(network);
    set_designated_ports(network);
}

/**
 * @brief Elects the root bridge
 * @param network The network
 */
void elect_root_bridge(Network *network)
{
    // SENDING BPDUS
    for (int i = 0; i < network->num_switches; i++)
    {
        send_bpdu(network, &network->devices[i]);
    }

    // MODIFYING THE PORTS OF NON-ROOT BRIDGES FROM LISTENING STATE TO FORWARDING
    for (int i = 0; i < network->num_switches; i++)
    {
        for (int j = 0; j < network->devices[i].switch_info.num_ports; j++)
        {
            if (network->devices[i].switch_info.ports[j].state == 'L')
            {
                network->devices[i].switch_info.ports[j].role = 'R';
                network->devices[i].switch_info.ports[j].state = 'F';
            }
        }
    }
}
/**
 * @brief Blocks the ports of non-root bridges that are not root ports
 * @param network The network
 */
void set_block_ports(Network *network)
{
    for (int i = 0; i < network->num_switches; i++)
    {
        if (network->devices[i].switch_info.bpdu.root_bridge_priority == network->devices[i].switch_info.priority && compare_mac_address(&network->devices[i].switch_info.bpdu.root_bridge_mac_address, &network->devices[i].mac_address) == 0)
            continue;
        for (int j = 0; j < network->devices[i].switch_info.num_ports; j++)
        {

            if (network->devices[i].switch_info.ports[j].role != 'R')
            {
                network->devices[i].switch_info.ports[j].state = 'B';
                network->devices[i].switch_info.ports[j].role = 'B';
            }
        }
    }
}
/**
 * @brief Set the designated ports of the non-root bridges
 */
void set_designated_ports(Network *network)
{
    for (int i = 0; i < network->num_switches; i++)
    {
        if (network->devices[i].switch_info.bpdu.root_bridge_priority == network->devices[i].switch_info.priority && compare_mac_address(&network->devices[i].switch_info.bpdu.root_bridge_mac_address, &network->devices[i].mac_address) == 0)
            continue;
        Device *connected_devices[256];
        uint16_t nb = find_connected_devices(network, i, connected_devices);
        for (int j = 0; j < nb; j++)
        {
            if (connected_devices[j]->type == STATION)
            {
                network->devices[i].switch_info.ports[j].state = 'F';
                network->devices[i].switch_info.ports[j].role = 'D';
            }
            else
            {
                Device *connected_devices2[256];
                uint16_t nb2 = find_connected_devices(network, connected_devices[j]->index, connected_devices2);
                int previous_device_index = -1;
                for (int k = 0; k < nb2; k++)
                {
                    if (network->devices[i].index == connected_devices2[k]->index)
                    {
                        previous_device_index = k;
                    }
                }
                if (network->devices[i].switch_info.ports[j].role == 'R')
                {
                    connected_devices[j]->switch_info.ports[previous_device_index].state = 'F';
                    connected_devices[j]->switch_info.ports[previous_device_index].role = 'D';
                }
            }
        }
    }
}