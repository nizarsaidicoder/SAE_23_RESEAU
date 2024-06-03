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
    print_ip_address(&frame->src);
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
    //printf("Data: %s", frame->data);
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
    //printf("Data: %d", frame->data);
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
void update_switching_table_with_port(Device *switch_, MACAddress *mac_address, uint8_t port_number)
{
    for (int i = 0; i < switch_->switch_info.switching_table_entries; i++)
    {
        if (compare_mac_address(&switch_->switch_info.switching_table[i].mac_address, &mac_address))
        {
            return;
        }
    }
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].mac_address = *mac_address;
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].port_number = port_number;
    switch_->switch_info.switching_table_entries++;
}

bool send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame)
{
    if (receive_frame(new_source, frame))
        return true;
    if (new_source->type == STATION)
    {
        if (compare_mac_address(&new_source->mac_address, &frame->src))
        {
            Device *connected_devices[256];
            uint16_t nb = find_connected_devices(network, new_source->index, connected_devices);
            return send_frame(network, connected_devices[0], new_source, frame);
        }
    }
    // Check if the device is a switch
    if (new_source->type == SWITCH)
    {
        // Check if the destination device is in the switching table
        for (int i = 0; i < new_source->switch_info.switching_table_entries; i++)
        {
            if (compare_mac_address(&new_source->switch_info.switching_table[i].mac_address, &frame->dest))
            {
                print_device(new_source);
                printf("OIIIIIIIIIIIIIIIIIIIIIIII\n");
                printf("DESTINATION DEVICE FOUND IN THE SWITCHING TABLE\n");
                printf("IN PORT NUMBER %d\n", new_source->switch_info.switching_table[i].port_number);
                Device *connected_devices[256];
                uint16_t num_connected_devices = find_connected_devices(network, new_source->index, connected_devices);
                Device *reciever = connected_devices[new_source->switch_info.switching_table[i].port_number];
                return send_frame(network, reciever, new_source, frame);
            }
        }
        // If the destination device is not in the switching table
        // Send the frame to all the devices connected to the switch except the previous source
        Device *connected_devices[256];
        int index;
        uint16_t num_connected_devices = find_connected_devices(network, new_source->index, connected_devices);
        for (int i = 0; i < num_connected_devices; i++)
        {
            if (compare_mac_address(&connected_devices[i]->mac_address, &previous_source->mac_address))
            {
                index = i;
                update_switching_table_with_port(new_source, &frame->dest, index);
            }
        }
        bool is_found = false;
        int i = 0;
        while (!is_found && i < num_connected_devices)
        {
            if (connected_devices[i]->mac_address.address != previous_source->mac_address.address)
            {
                is_found = send_frame(network, connected_devices[i], new_source, frame);
            }
            if (is_found)
            {
                return true;
            }
            i++;
        }
    }
    return false;
}

bool send_frame_from_switch(Network *network, Device *switch_, Device *destination, Frame *frame)
{
    Device *connected_devices[256];
    uint16_t num_connected_devices = find_connected_devices(network, switch_->index, connected_devices);
    for (int i = 0; i < num_connected_devices; i++)
    {
        if (send_frame(network, connected_devices[i], switch_, frame))
        {
            update_switching_table(network, switch_, connected_devices[i]);
            update_switching_table_with_port(switch_, &frame->dest, i);
            return true;
        }
    }
}

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