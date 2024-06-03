#include "headers/types.h"
#include "headers/frame.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"

void frame_print(Frame *frame)
{
    printf("------------------frame------------------\n");
    printf("Preambule: %d.%d.%d.%d.%d.%d.%d\n", frame->preambule[0], frame->preambule[1], frame->preambule[2], frame->preambule[3], frame->preambule[4], frame->preambule[5], frame->preambule[6]);
    printf("SFD: %d\n", frame->SFD);
    printf("Destinataire: ");
    print_mac_address(&frame->dest);
    printf("Source: ");
    print_mac_address(&frame->src);
    printf("Type: %d", frame->type);
}

void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, uint8_t *data)
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
    // Change the type of data to char *, because the data is basically a string of characters, so you should for each character of the string given
    // in the parameter data, copy it to the frame->data[i] where i is the index of the character in the string
    frame->data = data;
}

void frame_print_data_user_mode(Frame *frame)
{
    // Print the data of the frame
    // The source and destination MAC addresses
    // The type of the frame
    // The data of the frame
    printf("------------------Trame------------------\n");
    printf("Preambule: %d.%d.%d.%d.%d.%d.%d\n", frame->preambule[0], frame->preambule[1], frame->preambule[2], frame->preambule[3], frame->preambule[4], frame->preambule[5], frame->preambule[6]);
    printf("SFD: %d\n", frame->SFD);
    printf("Destinataire: ");
    print_mac_address(&frame->dest);
    printf("Source: ");
    print_mac_address(&frame->src);
    printf("Type: %d\n", frame->type);
    // printf("Data: %s", frame->data);
}

void frame_print_data_hex_mode(Frame *frame)
{
    // Print the data of the frame in hexadecimal format
    // The source and destination MAC addresses
    // The type of the frame
    // The data of the frame
    printf("------------------Trame------------------\n");
    printf("Preambule: %02x.%02x.%02x.%02x.%02x.%02x.%02x\n", frame->preambule[0], frame->preambule[1], frame->preambule[2], frame->preambule[3], frame->preambule[4], frame->preambule[5], frame->preambule[6]);
    printf("SFD: %02x\n", frame->SFD);
    printf("Destinataire: ");
    print_mac_address(&frame->dest);
    printf("Source: ");
    print_mac_address(&frame->src);
    printf("Type: %02x\n", frame->type);
    // printf("Data: %d", frame->data);
}

uint16_t find_connected_devices(Network *network, uint16_t device_index, Device *connected_devices[])
{
    // This function should return the number of devices connected to the given device
    // and populate the connected_devices array with the connected devices
    // (excluding the given device)
    if (device_index >= network->num_devices)
    {
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

void update_switching_table(Network *network, Device *switch_, Device *device)
{
    // This function should update the switching table of the switch
    // with the MAC address of the device and the port number
    // connected to the device
    Device *connected_devices[256];
    uint16_t num_connected_devices = find_connected_devices(network, switch_->index, connected_devices);
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, &device->mac_address))
        {
            return;
        }
    }
    for (int i = 0; i < num_connected_devices; i++)
    {
        if (compare_mac_address(&connected_devices[i]->mac_address, &device->mac_address))
        {
            switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].mac_address = device->mac_address;
            switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].port_number = i;
            switch_->switch_info.switching_table_entries++;
        }
    }
}
void add_entry_to_switching_table(Device *switch_, MACAddress *mac_address, uint8_t port_number)
{
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, mac_address))
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
        if (compare_mac_address(&connected_devices[i]->mac_address, &device->mac_address))
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
            receive_frame(new_source, frame);
    }
    // Check if the device is a switch
    if (new_source->type == SWITCH)
    {
        if (previous_source != NULL)
        {
            uint8_t port = get_port_number(network, new_source, previous_source);
            add_entry_to_switching_table(new_source, &frame->src, port);
        }
        if (!receive_frame(new_source, frame))
        {
            int port_number = check_switching_table_entries(new_source, &frame->dest);
            if (port_number != -1)
            {
                // printf("Redirecting\n");
                Device *connected_devices[256];
                find_connected_devices(network, new_source->index, connected_devices);
                Device *reciever = connected_devices[port_number];
                send_frame(network, reciever, new_source, frame);
            }
            else
                flood_frame(network, new_source, previous_source, frame);
        }
    }
}
int check_switching_table_entries(Device *switch_, MACAddress *mac_address)
{
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, mac_address))
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
            send_frame(network, connected_devices[i], source, frame);
        }
    }
}

// bool send_frame_from_switch(Network *network, Device *switch_, Device *destination, Frame *frame)
// {
//     // Device *connected_devices[256];
//     // uint16_t num_connected_devices = find_connected_devices(network, switch_->index, connected_devices);
//     // for (int i = 0; i < num_connected_devices; i++)
//     // {
//     //     if (send_frame(network, connected_devices[i], switch_, frame))
//     //     {
//     //         update_switching_table(network, switch_, connected_devices[i]);
//     //         update_switching_table_with_port(switch_, &frame->dest, i);
//     //         return true;
//     //     }
//     // }
// }

bool receive_frame(Device *device, Frame *frame)
{
    // This function should check if the device is the receiver of the frame
    // and return true if the device is the receiver
    // and false if the device is not the receiver
    if (compare_mac_address(&device->mac_address, &frame->dest))
    {
        frame_print_data_user_mode(frame);
        return true;
    }
    return false;
}