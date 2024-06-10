#include "headers/bpdu.h"

/**
 * @brief Converts a BPDU to a string
 * @param bpdu The BPDU to convert
 * @param output The string to output the BPDU to
 */
void bpdu_to_string(BPDU *bpdu, char *output)
{
    char root_bridge_mac_address[MAC_BUFFER_SIZE];
    mac_address_to_string(&bpdu->root_bridge_mac_address, root_bridge_mac_address);
    char sender_mac_address[MAC_BUFFER_SIZE];
    mac_address_to_string(&bpdu->sender_mac_address, sender_mac_address);
    sprintf(output, "%d;%s;%d;%s;%d", bpdu->root_bridge_priority, root_bridge_mac_address, bpdu->root_path_cost, sender_mac_address, bpdu->sender_priority);
}
/**
 * @brief Converts a BPDU to a frame
 * @param bpdu The BPDU to convert
 * @param frame The frame to output the BPDU to
 */
void bpdu_to_frame(BPDU *bpdu, Frame *frame)
{
    MACAddress broadcast;
    for (int i = 0; i < 6; i++)
    {
        broadcast.address[i] = 255;
    }
    char bpdu_string[BPDU_BUFFER_SIZE];
    bpdu_to_string(bpdu, bpdu_string);
    frame_init(frame, bpdu->sender_mac_address, broadcast, BPDU_FRAME_TYPE, bpdu_string);
}
/**
 * @brief Converts a frame to a BPDU
 * @param frame The frame to convert
 * @param bpdu The BPDU to output the frame to
 */
void frame_to_bpdu(Frame *frame, BPDU *bpdu)
{
    char *output[5];
    split(frame->data, ";", output);
    bpdu->root_bridge_priority = atoi(output[0]);
    MACAddress root_bridge_mac_address = mac_address_from_string(output[1]);
    bpdu->root_bridge_mac_address = root_bridge_mac_address;
    bpdu->root_path_cost = atoi(output[2]);
    MACAddress sender_mac_address = mac_address_from_string(output[3]);
    bpdu->sender_priority = atoi(output[4]);
    bpdu->sender_mac_address = sender_mac_address;
}
/**
 * @brief Prints a BPDU
 * @param bpdu The BPDU to print
 */
void print_bpdu(BPDU *bpdu)
{
    printf("BPDU : \n");
    printf("Root Bridge Priority : %d\n", bpdu->root_bridge_priority);
    printf("Root Bridge MAC Address : ");
    print_mac_address(&bpdu->root_bridge_mac_address);
    printf("Root Path Cost : %d\n", bpdu->root_path_cost);
    printf("Sender MAC Address : ");
    print_mac_address(&bpdu->sender_mac_address);
    printf("Sender Priority : %d\n", bpdu->sender_priority);
}
/**
 * @brief Assigns a BPDU to a device
 * @param device The device to assign the BPDU to
 */
void assign_bpdu(Device *device)
{
    if (device->type != SWITCH)
        return;
    BPDU bpdu;
    bpdu.root_bridge_priority = device->switch_info.priority;
    bpdu.root_bridge_mac_address = device->mac_address;
    bpdu.root_path_cost = 0;
    bpdu.sender_mac_address = device->mac_address;
    bpdu.sender_priority = device->switch_info.priority;
    device->switch_info.bpdu = bpdu;
}
/**
 * @brief Compares two BPDUs
 * @param bpdu1 The first BPDU
 * @param bpdu2 The second BPDU
 * @return 1 if bpdu1 is greater, -1 if bpdu2 is greater, 0 if they are equal
 */
int compare_bpdu(BPDU *bpdu1, BPDU *bpdu2)
{
    if (bpdu1->root_bridge_priority < bpdu2->root_bridge_priority)
        return 1;
    if (bpdu1->root_bridge_priority > bpdu2->root_bridge_priority)
        return -1;
    else
    {
        if (compare_mac_address(&bpdu1->root_bridge_mac_address, &bpdu2->root_bridge_mac_address) == -1)
            return 1;
        if (compare_mac_address(&bpdu1->root_bridge_mac_address, &bpdu2->root_bridge_mac_address) == 1)
            return -1;
        else
        {
            if (bpdu1->root_path_cost < bpdu2->root_path_cost)
                return 1;
            if (bpdu1->root_path_cost > bpdu2->root_path_cost)
                return -1;
            else
            {
                if (compare_mac_address(&bpdu1->sender_mac_address, &bpdu2->sender_mac_address) == -1)
                    return 1;
                if (compare_mac_address(&bpdu1->sender_mac_address, &bpdu2->sender_mac_address) == 1)
                    return -1;
                else
                {
                    if (bpdu1->sender_priority < bpdu2->sender_priority)
                        return 1;
                    if (bpdu1->sender_priority > bpdu2->sender_priority)
                        return -1;
                }
            }
        }
    }
    return 0;
}
/**
 * @brief Updates a BPDU
 * @param network The network
 * @param device The device to update the BPDU of
 * @param previous_device The previous device that sent the BPDU
 * @param new_bpdu The new BPDU
 * @return true if the BPDU was updated, false otherwise
 */
bool update_bpdu(Network *network, Device *device, Device *previous_device, BPDU *new_bpdu)
{
    BPDU bpdu;
    bpdu.root_bridge_priority = new_bpdu->root_bridge_priority;
    bpdu.root_bridge_mac_address = new_bpdu->root_bridge_mac_address;
    bpdu.root_path_cost = new_bpdu->root_path_cost + network_link_weight(network, device->index, previous_device->index);
    bpdu.sender_mac_address = new_bpdu->sender_mac_address;
    bpdu.sender_priority = new_bpdu->sender_priority;
    if (compare_bpdu(&device->switch_info.bpdu, &bpdu) == -1)
    {
        // MODIFY ONLY THE ROOT BRIDGE INFO
        device->switch_info.bpdu.root_bridge_priority = bpdu.root_bridge_priority;
        device->switch_info.bpdu.root_bridge_mac_address = bpdu.root_bridge_mac_address;
        device->switch_info.bpdu.root_path_cost = bpdu.root_path_cost;
        return true;
    }
    return false;
}
/**
 * @brief Sends a BPDU
 * @param network The network
 * @param device The device to send the BPDU from
 */
void send_bpdu(Network *network, Device *device)
{
    // COLOR BLUE
    printf("\033[0;34m");
    printf("Sending BPDU from ");
    printf("\033[0m");
    print_device(device);
    printf("Current ");
    print_bpdu(&device->switch_info.bpdu);
    Device *connected_devices[256];
    uint16_t nb = find_connected_devices(network, device->index, connected_devices);
    for (int i = 0; i < nb; i++)
    {
        if (connected_devices[i]->type != STATION && device->switch_info.ports[i].state == 'F')
        {
            Frame frame;
            bpdu_to_frame(&device->switch_info.bpdu, &frame);
            send_frame(network, connected_devices[i], device, &frame);
        }
    }
}
