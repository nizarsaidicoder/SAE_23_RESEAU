#include "headers/types.h"
#include "headers/frame.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"

void frame_print(Frame *trame)
{
    printf("------------------Trame------------------\n");
    printf("Preambule: %d.%d.%d.%d.%d.%d.%d\n", trame->preambule[0], trame->preambule[1], trame->preambule[2], trame->preambule[3], trame->preambule[4], trame->preambule[5], trame->preambule[6]);
    printf("SFD: %d\n", trame->SFD);
    printf("Destinataire: ");
    print_mac_address(&trame->dest);
    printf("Source: ");
    print_ip_address(&trame->src);
    printf("Type: %d", trame->type);
}

void frame_init(Frame *trame, MACAddress src, MACAddress dest, uint16_t type, uint8_t *data)
{
    // Initialisation de preambule
    for (int i = 0; i < 7; i++)
    {
        trame->preambule[i] = 170;
    }
    trame->SFD = 171;
    trame->src = src;
    trame->dest = dest;
    trame->type = type;
    trame->data = data;
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
    for (int i = 0; i < num_connected_devices; i++)
    {
        if (connected_devices[i]->mac_address.address == device->mac_address.address)
        {
            switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].mac_address = device->mac_address;
            switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].port_number = i;
            switch_->switch_info.switching_table_entries++;
        }
    }
}
void update_switching_table_with_port(Device *switch_, MACAddress mac_address, uint8_t port_number)
{
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].mac_address = mac_address;
    switch_->switch_info.switching_table[switch_->switch_info.switching_table_entries].port_number = port_number;
    switch_->switch_info.switching_table_entries++;
}

bool send_frame_from_station(Network *network, Device *source, Device *destination, Frame *frame)
{
    // This function should send a frame from a source device to a destination device
    // in a non-cycle network simulating the switching table population and the frame forwarding
    // 1. The source device sends the frame to the connected switch
    //    the switch updates its switching table with the source device mac address and its corresponding port
    // 2. The switch recieves the frame and checks if it have the destination device in its switching table
    //    a. If the destination device is in the switching table
    //       The switch gets the device from the mac address and sends the frame to the device
    //       -> The device receives the frame
    //    b. If the destination device is not in the switching table
    //      The switch sends the frame to all the devices connected to it except the source device
    //      -> The destination device receives the frame
    //      -> and based on the destination mac address the function returns true if the destination device is the receiver
    //     -> and false if the destination device is not the receiver
    Device *connected_devices[256];
    uint16_t nb = find_connected_devices(network, source->index, connected_devices);
    // update_switching_table(network, connected_devices[0], source);
    // color green console
    printf("\033[0;32m");
    printf("TRANSMITING FRAME FROM\n");
    print_mac_address(&source->mac_address);
    printf("TO\n");
    print_mac_address(&destination->mac_address);
    printf("\n\n");
    // color reset console
    printf("\033[0m");
    return send_frame(network, connected_devices[0], source, frame);
}

bool send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame)
{
    // Check if the device is a switch
    if (new_source->type == SWITCH)
    {
        // Check if the destination device is in the switching table
        for (int i = 0; i < new_source->switch_info.switching_table_entries; i++)
        {
            if (compare_mac_address(&new_source->switch_info.switching_table[i].mac_address, &frame->dest))
            {
                printf("DESTINATION DEVICE FOUND IN THE SWITCHING TABLE\n");
                Device *connected_devices[256];
                uint16_t num_connected_devices = find_connected_devices(network, new_source->index, connected_devices);
                Device *reciever = connected_devices[new_source->switch_info.switching_table[i].port_number];
                return send_frame(network, reciever, new_source, frame);
            }
        }
        // If the destination device is not in the switching table
        // Send the frame to all the devices connected to the switch except the previous source
        Device *connected_devices[256];
        uint16_t num_connected_devices = find_connected_devices(network, new_source->index, connected_devices);
        printf("\n\n");
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
                // UPDATING THE SWITCHING TABLE OF THE CURRENT AND PREVIOUS SWITCH
                update_switching_table(network, new_source, previous_source);
                update_switching_table_with_port(new_source, frame->dest, i);
                // update_switching_table(network, previous_source, new_source);
                return true;
            }
            i++;
        }
        return false;
    }
    return receive_frame(new_source, frame);
}

bool receive_frame(Device *device, Frame *frame)
{
    // This function should check if the device is the receiver of the frame
    // and return true if the device is the receiver
    // and false if the device is not the receiver
    if (compare_mac_address(&device->mac_address, &frame->dest))
    {
        printf("\033[0;32m");
        printf("RECEIVING FRAME\n");
        print_mac_address(&device->mac_address);
        printf("\n\n");
        printf("\033[0m");
        return true;
    }
    return false;
}

bool compare_mac_address(MACAddress *mac1, MACAddress *mac2)
{
    // This function should compare two MAC addresses
    // and return true if they are equal
    // and false if they are not equal
    for (int i = 0; i < 6; i++)
    {
        if (mac1->address[i] != mac2->address[i])
        {
            return false;
        }
    }
    return true;
}

// Sending a frame from a source device to a destination device in a non-cycle network simulating the switching table population and the frame forwarding
// HOW IT WORKS:
// 1. The source device sends the frame to the connected switch
//    the switch updates its switching table with the source device mac address and its corresponding port
// 2. The switch recieves the frame and checks if it have the destination device in its switching table
//    a. If the destination device is in the switching table
//       The switch gets the device from the mac address and sends the frame to the device
//       -> The device receives the frame
//    b. If the destination device is not in the switching table
//      The switch sends the frame to all the devices connected to it except the source device
//      -> The destination device receives the frame
//      -> and based on the destination mac address the function returns true if the destination device is the receiver
//     -> and false if the destination device is not the receiver

// In Pseudo code the function will look like this
//    send_frame_from_station(Network *network,Device *source, Device *destination, Frame *frame)
//    {
//       switch = find_connected_switch(network, source)
//       update_switching_table(switch, source)
//       send_frame_from_switch(network, switch, source, destination, frame)
//    }
//    send_frame_from_switch(Network *network, Switch *switch, Device *source, Device *destination, Frame *frame)
//    {
//       if (destination in switch->switching_table)
//       {
//          device = get_device_from_mac_address(destination)
//          return (recieve_frame(device, frame))
//       }
//       else
//       {
//          for (int i = 0; i < switch->connected_devices; i++)
//          {
//             if (switch->connected_devices[i] != source)
//             {
//                if (recieve_frame(switch->connected_devices[i], frame))
//                {
//                   update_switching_table(switch, destination)
//                   return true
//                }
//                else
//                {
//                   if(switch->connected_devices[i]->type == SWITCH)
//                   {
//                      send_frame_from_switch(network, switch->connected_devices[i], source, destination, frame)
//                   }
//                   else
//                   {
//                     return false
//                   }
//                }
//             }
//          }
//       }
//    }
//   recieve_frame(Device *device, Frame *frame)
//   {
//      if (device->mac_address == frame->dest)
//      {
//         return true
//      }
//      else
//      {
//         return false
//      }
//   }
//   update_switching_table(Switch *switch, Device *device)
//   {
//       connected_device = find_connected_device(switch, device)
//       switch->switching_table.add(device->mac_address, connected_device->port)
//   }
//   find_connected_device(Switch *switch, Device *device)
