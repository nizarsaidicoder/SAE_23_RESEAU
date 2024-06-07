#include "headers/frame.h"
/**
 * @brief Initializes a frame
 * @param frame The frame to initialize
 * @param src The source MAC address
 * @param dest The destination MAC address
 * @param type The type of the frame
 * @param data The data of the frame
 */
void frame_init(Frame *frame, MACAddress src, MACAddress dest, uint16_t type, char *data)
{
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
/**
 * @brief Prints the data of a frame in user mode
 * @param frame The frame to print
 */
void frame_print_data_user_mode(Frame *frame)
{
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
/**
 * @brief Prints the data of a frame in hexadecimal mode
 * @param frame The frame to print
 */
void frame_print_data_hex_mode(Frame *frame)
{

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

/**
 * @brief Adds an entry to the switching table of a switch
 * @param switch_ The switch
 * @param mac_address The MAC address to add
 * @param port_number The port number to add
 * @note You should pass in a pointer to the device not to the switch itself
 */
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
/**
 * @brief Checks if a MAC address is in the switching table of a switch
 * @param switch_ The switch
 * @param mac_address The MAC address to check
 * @return The port number if the MAC address is in the switching table, -1 otherwise
 */
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
/**
 * @brief Gets the port number of a device connected to a switch
 * @param network The network
 * @param switch_ The switch
 * @param device The device
 * @return The port number
 */
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
/**
 * @brief Sends a frame
 * @param network The network
 * @param new_source The new source device that received the frame
 * @param previous_source The previous source device that sent the frame
 * @param frame The frame to send
 */
void send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame)
{
    // Check if the device is a station
    if (new_source->type == STATION)
    {
        // Check if the station is itself the source
        if (previous_source == NULL)
        {
            Device *connected_devices[256];
            uint16_t nb = find_connected_devices(network, new_source->index, connected_devices);
            // check if the station is connected to a switch, if yes send the frame to the switch
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
        // check if the device has recieved the frame from a blocked port
        bool is_blocked = false;
        for (int i = 0; i < nb; i++)
        {
            if (compare_mac_address(&connected_devices[i]->mac_address, &new_source->mac_address) == 0)
            {
                if (new_source->switch_info.ports[i].state == 'B')
                    is_blocked = true;
            }
        }
        // if the port is blocked, do nothing
        if (is_blocked)
            return;
        if (previous_source != NULL)
        {
            // if the device is a switch and the previous source is not null, add the entry (source MAC Address of the frame) to the switching table
            uint8_t port = get_port_number(network, new_source, previous_source);
            add_entry_to_switching_table(new_source, &frame->src, port);
        }
        if (!receive_frame(network, new_source, previous_source, frame))
        {
            // if the device is a switch and the frame is not for the switch, check the switching table entries
            int port_number = check_switching_table_entries(new_source, &frame->dest);
            if (port_number != -1)
            {
                // if the MAC address is in the switching table, send the frame to the corresponding port
                Device *reciever = connected_devices[port_number];
                if (new_source->switch_info.ports[port_number].state == 'F')
                    send_frame(network, reciever, new_source, frame);
            }
            // if the MAC address is not in the switching table, flood the frame
            else
            {
                flood_frame(network, new_source, previous_source, frame);
            }
        }
    }
}

/**
 * @brief Floods a frame in all the ports of a switch except the port from which the frame was received
 * @param network The network
 * @param source The source device
 * @param previous_source The previous source device
 * @param frame The frame to flood
 */
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
/**
 * @brief Receives a frame
 * @param network The network
 * @param device The device that received the frame
 * @param previous_device The previous device that sent the frame
 * @param frame The frame to receive
 * @return true if the frame was received, false otherwise
 */
bool receive_frame(Network *network, Device *device, Device *previous_device, Frame *frame)
{
    MACAddress broadcast;
    for (int i = 0; i < 6; i++)
        broadcast.address[i] = 255;
    // check if the frame is for the device or if it is a broadcast frame
    if (compare_mac_address(&device->mac_address, &frame->dest) == 0 || compare_mac_address(&broadcast, &frame->dest) == 0)
    {
        if (frame->type == BPDU_FRAME_TYPE && device->type == SWITCH)
        {
            // Convert the frame to a BPDU
            BPDU bpdu;
            frame_to_bpdu(frame, &bpdu);
            // If the BPDU is better than the current BPDU, update the BPDU
            if (update_bpdu(network, device, previous_device, &bpdu))
            {
                Device *neighbour_switches[256];
                uint16_t nb = find_connected_devices(network, previous_device->index, neighbour_switches);
                for (int i = 0; i < nb; i++)
                {
                    if (device->switch_info.ports[i].state == 'L')
                    {
                        device->switch_info.ports[i].state = 'D';
                    }
                }
                // MAKE THE PORT FROM WHICH THE BPDU WAS RECEIVED A ROOT PORT (STATE = 'LISTENING')
                device->switch_info.ports[get_port_number(network, device, previous_device)].state = 'L';
                send_bpdu(network, device);
            }
        }
        // if the frame is not a BPDU frame, print the data
        else if (frame->type != BPDU_FRAME_TYPE)
        {
            frame_print_data_user_mode(frame);
            frame_print_data_hex_mode(frame);
        }
        return true;
    }
    return false;
}
