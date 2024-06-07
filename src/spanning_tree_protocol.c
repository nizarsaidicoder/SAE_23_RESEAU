#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);
    // for (int i = 0; i < network->num_switches; i++)
    // {
    //     switch_print_ports(network->devices[i].switch_info);
    // }

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

    // BLOCK EVERY PORT THAT IS NOT A ROOT PORT IN OTHER SWITCHES
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
                if (connected_devices[j]->switch_info.ports[previous_device_index].role == 'R' && network->devices[i].switch_info.ports[j].role != 'R')
                {
                    network->devices[i].switch_info.ports[j].state = 'F';
                    network->devices[i].switch_info.ports[j].role = 'D';
                }
            }
            // else
            // {
            //     // CHECK IF THE
            //     // int distance = network->devices[i].switch_info.bpdu.root_path_cost;
            //     // int distance2 = connected_devices[j]->switch_info.bpdu.root_path_cost;
            //     // int distance_total = distance + distance2;
            //     // if (distance_total > distance)
            //     // {
            //     //     network->devices[i].switch_info.ports[j].state = 'B';
            //     //     network->devices[i].switch_info.ports[j].role = 'B';
            //     // }
            // }
            // CHECK IF THE CURRENT PORT IS A ROOT PORT
        }
    }
    for (int i = 0; i < network->num_switches; i++)
    {
        switch_print_ports(network->devices[i].switch_info);
    }
    // switch_print_ports(network->devices[6].switch_info);
}

void elect_root_bridge(Network *network)
{
    // for (int i = 0; i < network->num_switches; i++)
    // {
    //     send_bpdu(network, &network->devices[i]);
    // }
    for (int i = 0; i < network->num_switches; i++)
    {
        Device *device = &network->devices[i];
        Device *connected_devices[256];
        uint16_t nb = find_connected_devices(network, device->index, connected_devices);
        for (int j = 0; j < nb; j++)
        {
            if (connected_devices[j]->type != STATION && device->switch_info.ports[j].state == 'F')
            {
                Frame frame;
                bpdu_to_frame(&device->switch_info.bpdu, &frame);
                send_frame(network, connected_devices[j], device, &frame);
            }
        }
    }
}

int get_port_from_switching_table(Switch *switch_info, MACAddress mac_address)
{
    for (int i = 0; i < switch_info->switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_info->switching_table[i].mac_address, &mac_address) == 0)
        {
            return switch_info->switching_table[i].port_number;
        }
    }
    return -1;
}