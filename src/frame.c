#include "headers/types.h"
#include "headers/frame.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/spanning_tree_protocol.h"

void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, char *data)
{
    // Initialisation de preambule
    for (int i = 0; i < 7; i++)
    {
        frame->preambule[i] = 170;
    }
    frame->SFD = 171;
    frame->src = src;
    frame->dest = dest;
    frame->type = type;
    for (size_t i = 0; i < strlen(data); i++)
    {
        frame->data[i] = data[i];
    }
    frame->data[strlen(data)] = '\0';
    frame->FCS = 0;
}

void frame_print_data_user_mode(Frame *frame)
{
    // Print the data of the frame
    // The source and destination MAC addresses
    // The type of the frame
    // The data of the frame
    // color green
    printf("\033[0;33m");
    printf("------------------Trame : USER MODE------------------\n");
    printf("Destination ");
    print_mac_address(&frame->dest);
    printf("Source ");
    print_mac_address(&frame->src);
    switch (frame->type)
    {
    case 0x0800:
        printf("Type: IP\n");
        break;
    case 0x0806:
        printf("Type: ARP\n");
        break;
    case 0x4242:
        printf("Type: BPDU\n");
        break;
    default:
        printf("Type: Unknown\n");
        break;
    }
    printf("Data: \n%s\n", frame->data);
    printf("\n");
    printf("\033[0m");
}

void frame_print_data_hex_mode(Frame *frame)
{
    // Print the data of the frame in hexadecimal format
    // The source and destination MAC addresses
    // The type of the frame
    // The data of the frame
    // 00 00 00 00 00 00 00 00 00 00 00 00 08 00 45 00
    // 00 3C 00 00 40 00 40 06 00 00 7F 00 00 01 7F 00
    // 00 01 08 00 27 10 00 02 00 00 00 00 00 00 00 00
    // 50 02 20 00 00 00 00 00
    printf("\033[0;34m"); // color blue
    int printed_bytes = 0;
    printf("------------------Trame : HEX MODE------------------\n");
    for (int i = 0; i < 7; i++)
    {
        printf("%02x ", frame->preambule[i]);
        printed_bytes++;
        if (printed_bytes % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("%02x ", frame->SFD);
    printed_bytes++;
    if (printed_bytes % 16 == 0)
    {
        printf("\n");
    }
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", frame->dest.address[i]);
        printed_bytes++;
        if (printed_bytes % 16 == 0)
        {
            printf("\n");
        }
    }
    for (int i = 0; i < 6; i++)
    {
        printf("%02x ", frame->src.address[i]);
        printed_bytes++;
        if (printed_bytes % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("%02x ", frame->type);
    printed_bytes++;
    if (printed_bytes % 16 == 0)
    {
        printf("\n");
    }
    for (size_t i = 0; frame->data[i] != '\0'; i++)
    {
        printf("%02x ", frame->data[i]);
        printed_bytes++;
        if (printed_bytes % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
    printf("\033[0m");
}

uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[])
{
    // This function should return the number of devices connected to the given device
    // and populate the connected_devices array with the connected devices
    // (excluding the given device)
    if (device_index >= network->num_devices)
    {
        printf("Device index out of bounds\n");
        return 0;
    }
    uint16_t connected_devices_count = 0;
    for (int i = 0; i < network->num_links; i++)
    {
        if (network->links[i].device1_index == device_index)
        {
            connected_devices[connected_devices_count] = &network->devices[network->links[i].device2_index];
            connected_devices_count++;
        }
        else if (network->links[i].device2_index == device_index)
        {
            connected_devices[connected_devices_count] = &network->devices[network->links[i].device1_index];
            connected_devices_count++;
        }
    }
    return connected_devices_count;
}
void add_entry_to_switching_table(Device *switch_, MACAddress *mac_address, uint8_t port_number)
{
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, mac_address) == 0)
        {
            return;
        }
    }
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].mac_address = *mac_address;
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].port_number = port_number;
    switch_->switch_info.switching_table_entries++;
}

uint8_t get_port_number(Network *network, Device *switch_, Device *device)
{
    Device *connected_devices[256];
    uint16_t num_connected_devices = find_connected_devices(network, switch_->index, connected_devices);
    uint8_t index = 0;
    for (int i = 0; i < num_connected_devices; i++)
    {
        if (compare_mac_address(&connected_devices[i]->mac_address, &device->mac_address) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

void send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame)
{

    if (new_source->type == STATION)
    {
        if (previous_source == NULL)
        {
            Device *connected_devices[256];
            uint16_t nb = find_connected_devices(network, new_source->index, connected_devices);
            if (nb != 0)
                send_frame(network, connected_devices[0], new_source, frame);
        }
        else
            receive_frame(network, new_source, previous_source, frame);
    }
    // Check if the device is a switch
    if (new_source->type == SWITCH)
    {
        Device *connected_devices[256];
        int nb = find_connected_devices(network, new_source->index, connected_devices);
        bool is_blocked = false;
        for (int i = 0; i < nb; i++)
        {
            if (compare_mac_address(&connected_devices[i]->mac_address, &new_source->mac_address) == 0)
            {
                printf("I am the switch %d\n", new_source->index);
                if (new_source->switch_info.ports[i].state == 'B')
                    is_blocked = true;
            }
        }
        if (is_blocked)
            return;
        if (previous_source != NULL)
        {
            uint8_t port = get_port_number(network, new_source, previous_source);
            add_entry_to_switching_table(new_source, &frame->src, port);
        }

        if (!receive_frame(network, new_source, previous_source, frame))
        {
            int port_number = check_switching_table_entries(new_source, &frame->dest);
            if (port_number != -1)
            {
                Device *reciever = connected_devices[port_number];
                // CHECK IF THE PORT IS ACTIVE
                if (new_source->switch_info.ports[port_number].state == 'F')
                {
                    send_frame(network, reciever, new_source, frame);
                }
            }
            else
            {
                flood_frame(network, new_source, previous_source, frame);
            }
        }
    }
}
int check_switching_table_entries(Device *switch_, MACAddress *mac_address)
{
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, mac_address) == 0)
        {
            return switch_->switch_info.switching_table[i].port_number;
        }
    }
    return -1;
}

void flood_frame(Network *network, Device *source, Device *previous_source, Frame *frame)
{
    Device *connected_devices[256];
    uint16_t num_connected_devices = find_connected_devices(network, source->index, connected_devices);
    for (int i = 0; i < num_connected_devices; i++)
    {
        if (connected_devices[i]->mac_address.address != previous_source->mac_address.address)
        {
            if (source->switch_info.ports[i].state == 'F')
            {
                send_frame(network, connected_devices[i], source, frame);
            }
        }
    }
}

bool receive_frame(Network *network, Device *device, Device *previous_device, Frame *frame)
{
    MACAddress broadcast;
    for (int i = 0; i < 6; i++)
    {
        broadcast.address[i] = 255;
    }

    if (compare_mac_address(&device->mac_address, &frame->dest) == 0 || compare_mac_address(&broadcast, &frame->dest) == 0)
    {
        if (frame->type == BPDU_FRAME_TYPE && device->type == SWITCH)
        {
            BPDU bpdu;
            frame_to_bpdu(frame, &bpdu);
            if (update_bpdu(network, device, previous_device, &bpdu))
            {
                Device *neighbour_switches[256];
                uint16_t nb = find_connected_devices(network, previous_device->index, neighbour_switches);
                int current_distance = device->switch_info.bpdu.root_path_cost;
                for (int i = 0; i < nb; i++)
                {
                    if (device->switch_info.ports[i].state == 'L' && neighbour_switches[i]->switch_info.bpdu.root_path_cost < current_distance)
                    {
                        device->switch_info.ports[i].state = 'D';
                    }
                }
                // MAKE THE PORT FROM WHICH THE BPDU WAS RECEIVED A ROOT PORT
                device->switch_info.ports[get_port_number(network, device, previous_device)].state = 'L';
            }
        }
        else if (frame->type != BPDU_FRAME_TYPE)
        {
            frame_print_data_user_mode(frame);
            frame_print_data_hex_mode(frame);
        }
        return true;
    }
    return false;
}
