#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);
}
int compare_bpdu(BPDU *bpdu1, BPDU *bpdu2)
{
    if (bpdu2->root_bridge_priority < bpdu1->root_bridge_priority)
        return -1;
    if (bpdu2->root_bridge_priority > bpdu1->root_bridge_priority)
        return 1;
    else
    {
        if (compare_mac_address(&bpdu2->root_bridge_mac_address, &bpdu1->root_bridge_mac_address) == -1)
            return -1;
        if (compare_mac_address(&bpdu2->root_bridge_mac_address, &bpdu1->root_bridge_mac_address) == 1)
            return 1;
        else
        {
            if (bpdu2->root_path_cost < bpdu1->root_path_cost)
                return -1;
            if (bpdu2->root_path_cost > bpdu1->root_path_cost)
                return 1;
            else
            {
                if (compare_mac_address(&bpdu2->sender_mac_address, &bpdu1->sender_mac_address) == -1)
                    return -1;
                if (compare_mac_address(&bpdu2->sender_mac_address, &bpdu1->sender_mac_address) == 1)
                    return 1;
                else
                {
                    if (bpdu2->sender_priority < bpdu1->sender_priority)
                        return -1;
                    if (bpdu2->sender_priority > bpdu1->sender_priority)
                        return 1;
                    else
                        return 0;
                }
            }
        }
    }
}
// void send_frame(Network *network, Device *new_source, Device *previous_source, Frame *frame)
//  {
//      if (new_source->type == STATION)
//      {
//          if (previous_source == NULL)
//          {
//              Device *connected_devices[256];
//              uint16_t nb = find_connected_devices(network, new_source->index, connected_devices);
//              if (nb != 0)
//                  send_frame(network, connected_devices[0], new_source, frame);
//          }
//          else
//              receive_frame(new_source, frame);
//      }
//      // Check if the device is a switch
//      if (new_source->type == SWITCH)
//      {
//          if (previous_source != NULL)
//          {
//              uint8_t port = get_port_number(network, new_source, previous_source);
//              add_entry_to_switching_table(new_source, &frame->src, port);
//          }
//          if (!receive_frame(new_source, frame))
//          {
//              int port_number = check_switching_table_entries(new_source, &frame->dest);
//              if (port_number != -1)
//              {
//                  // printf("Redirecting\n");
//                  Device *connected_devices[256];
//                  find_connected_devices(network, new_source->index, connected_devices);
//                  Device *reciever = connected_devices[port_number];
//                  send_frame(network, reciever, new_source, frame);
//              }
//              else
//                  flood_frame(network, new_source, previous_source, frame);
//          }
//      }
//  }

void bpdu_to_frame(BPDU *bpdu, Frame *frame)
{
    MACAddress broadcast;
    for (int i = 0; i < 6; i++)
    {
        broadcast.address[i] = 0xFF;
    }
    char bpdu_string[BPDU_BUFFER_SIZE];
    bpdu_to_string(bpdu, bpdu_string);
    frame_init(frame, bpdu->sender_mac_address, broadcast, BPDU_FRAME_TYPE, bpdu_string);
}
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
void bpdu_to_string(BPDU *bpdu, char *output)
{
    char root_bridge_mac_address[MAC_BUFFER_SIZE];
    mac_address_to_string(&bpdu->root_bridge_mac_address, root_bridge_mac_address);
    char sender_mac_address[MAC_BUFFER_SIZE];
    mac_address_to_string(&bpdu->sender_mac_address, sender_mac_address);
    sprintf(output, "%d;%s;%d;%s;%d", bpdu->root_bridge_priority, root_bridge_mac_address, bpdu->root_path_cost, sender_mac_address, bpdu->sender_priority);
}

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
void send_bpdu(Network *network, Device *device)
{
    // do not use frame
}
void elect_root_bridge(Network *network)
{
}
void visite_composante_connexe(Network *network, Device *device, bool *visite)
{
    visite[device->index] = true;

    Device *connectedDevices;
    size_t n = find_connected_devices(network, device->index, &connectedDevices);

    for (size_t i = 0; i < n; i++)
    {
        uint16_t index = connectedDevices[i].index;

        if (!visite[index])
        {
            visite_composante_connexe(network, &connectedDevices[i], visite);
        }
    }
}

uint32_t nb_composantes_connexes(Network *network)
{
    uint32_t nbComposantes = 0;
    uint8_t ordre = network_num_devices(network);
    bool *visite = (bool *)malloc(sizeof(bool) * ordre);

    for (size_t i = 0; i < ordre; i++)
    {
        visite[i] = false;
    }

    for (size_t i = 0; i < ordre; i++)
    {
        if (!visite[i])
        {
            visite_composante_connexe(network, &network->devices[i], visite);
            nbComposantes++;
        }
    }

    visite = NULL;
    free(visite);

    return nbComposantes;
}

void dijkstra(Network *network, Device device, uint16_t *distanceSommets)
{
    if (nb_composantes_connexes(network) != 1)
    {
        return;
    }

    size_t ordre = network_num_devices(network);
    bool visite[ordre];

    // initialisation du tableau des "sommets" visités + distance
    for (size_t i = 0; i < ordre; i++)
    {
        visite[i] = false;
        distanceSommets[i] = __INT16_MAX__;
    }

    // on fixe le "sommet" de départ
    uint16_t sommetFixe = device.index;
    distanceSommets[sommetFixe] = 0;

    while (!visite[sommetFixe])
    {
        visite[sommetFixe] = true;
        Device *connectedDevices;
        size_t n = find_connected_devices(network, sommetFixe, &connectedDevices);

        for (size_t i = 0; i < n; i++)
        {
            // pour chaque "arête"
            Link link;
            link.device1_index = sommetFixe;
            link.device2_index = connectedDevices[i].index;

            //.. on récupère l'index dans le tableau d'"aretes"
            uint8_t index = network_link_index(network, &link);

            //..si la distance trouvée est plus petite que la distance enregistrée
            uint16_t distanceTrouvee = distanceSommets[sommetFixe] + network->links[index].weight;

            if (distanceTrouvee < distanceSommets[connectedDevices[i].index])
            {
                distanceSommets[connectedDevices[i].index] = distanceTrouvee;
            }
        }

        double min = __INT16_MAX__;

        // mise à jour du sommet fixé
        for (size_t i = 0; i < ordre; i++)
        {
            // si on ne l'a pas visité et que sa distance est la plus petite
            if (!visite[i] && distanceSommets[i] < min)
            {
                min = distanceSommets[i];
                sommetFixe = i;
            }
        }
    }
}

Device *find_shortest_path(Network *network, Device *device, Device *destination, Device *path[])
{
    return 0;
}
