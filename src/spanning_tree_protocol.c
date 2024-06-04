#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);

    //     Etats d'un port
    // • Vis-à-vis des messages 802.1d
    // • Port racine
    // • port via lequel on reçoit les messages provenant d e la racine par le plus court
    // chemin
    // • un port racine ne transmet pas de messages 802.1d  mais les lit
    // • un seul port racine par pont
    // • Port désigné
    // • port via le(s)quel(s) le pont retransmet les messa ges 802.1d reçus de la
    // racine
    // • message 802.1d transmis par un pont
    // • racine actuelle, identification du pont, coût jusq ue la racine
    // • 0, ou un port désigné sur un pont
    // • un port est désigné si le message qu'il transmet e st meilleur que le meilleur
    // message qu'il reçoit
    // • Port bloqué (reçoit uniquement les message. 802.1d )
    // • Vis-à-vis des trames de données
    // • port actif (port racine et port désigné)
    // • le pont écoute les trames via ce port et les retra nsmet (sélectivement) vers
    // les autres ports actifs du pont si nécessaire
    // • port inactif (port bloqué)
    // • le pont n'écoute pas les trames de données via ce port et donc ne retransmet
    // aucune trame de données reçue via ce port

    // Marking the ports of the switches that lead to the root bridge as root ports

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

    // BLOCK EVERY PORT THAT IS NOT A ROOT PORT
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

    // if a switch uses a port to reach the root bridge and that port is blocked for the other switch
    // activate the port for the other switch

    for (int i = 0; i < network->num_switches; i++)
    {
        if (network->devices[i].switch_info.bpdu.root_bridge_priority == network->devices[i].switch_info.priority && compare_mac_address(&network->devices[i].switch_info.bpdu.root_bridge_mac_address, &network->devices[i].mac_address) == 0)
        {
            continue;
        }

        int root_port = 0;
        for (int j = 0; j < network->devices[i].switch_info.num_ports; j++)
        {
            if (network->devices[i].switch_info.ports[j].role == 'R')
                root_port = j;
        }

        Device *connected_devices[256];
        uint16_t nb = find_connected_devices(network, network->devices[i].index, connected_devices);
        print_device(&network->devices[i]);
        Device *connected_devices_other_switch[256];
        uint16_t nb_other_switch = find_connected_devices(network, connected_devices[root_port]->index, connected_devices_other_switch);
        for (int j = 0; j < nb_other_switch; j++)
        {
            if (connected_devices_other_switch[j]->type == SWITCH)
            {
                if (compare_mac_address(&connected_devices_other_switch[j]->mac_address, &network->devices[i].mac_address) == 0)
                {
                    print_device(connected_devices[root_port]);
                    print_device(connected_devices_other_switch[j]);
                    connected_devices[root_port]->switch_info.ports[j].state = 'F';
                    connected_devices[root_port]->switch_info.ports[j].role = 'D';
                }
            }
        }

        // ACTIVATE ALL THE PORTS THAT LEAD TO A STATION
        for (int j = 0; j < nb; j++)
        {

            if (connected_devices[j]->type == STATION)
            {
                network->devices[i].switch_info.ports[j].state = 'F';
                network->devices[i].switch_info.ports[j].role = 'D';
            }
        }
    }

    for (int i = 0; i < network->num_switches; i++)
    {
        if (network->devices[i].switch_info.bpdu.root_bridge_priority == network->devices[i].switch_info.priority && compare_mac_address(&network->devices[i].switch_info.bpdu.root_bridge_mac_address, &network->devices[i].mac_address) == 0)
        {
            for (int j = 0; j < network->devices[i].switch_info.num_ports; j++)
            {
                network->devices[i].switch_info.ports[j].state = 'F';
                network->devices[i].switch_info.ports[j].role = 'D';
            }
        }
    }
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
    Frame frame;
    Device *connected_devices[256];
    uint16_t nb = find_connected_devices(network, device->index, connected_devices);
    for (int i = 0; i < nb; i++)
    {
        bpdu_to_frame(&device->switch_info.bpdu, &frame);
        printf("SWITCH %d :  TO SWITCH %d \n", device->index, connected_devices[i]->index);
        send_frame(network, connected_devices[i], device, &frame);
    }
}

void elect_root_bridge(Network *network)
{
    for (int i = 0; i < network->num_switches; i++)
    {
        send_bpdu(network, &network->devices[i]);
    }
}
// void visite_composante_connexe(Network *network, Device *device, bool *visite)
// {
//     visite[device->index] = true;

//     Device *connectedDevices;
//     size_t n = find_connected_devices(network, device->index, &connectedDevices);

//     for (size_t i = 0; i < n; i++)
//     {
//         uint16_t index = connectedDevices[i].index;

//         if (!visite[index])
//         {
//             visite_composante_connexe(network, &connectedDevices[i], visite);
//         }
//     }
// }

// uint32_t nb_composantes_connexes(Network *network)
// {
//     uint32_t nbComposantes = 0;
//     uint8_t ordre = network_num_devices(network);
//     bool *visite = (bool *)malloc(sizeof(bool) * ordre);

//     for (size_t i = 0; i < ordre; i++)
//     {
//         visite[i] = false;
//     }

//     for (size_t i = 0; i < ordre; i++)
//     {
//         if (!visite[i])
//         {
//             visite_composante_connexe(network, &network->devices[i], visite);
//             nbComposantes++;
//         }
//     }

//     visite = NULL;
//     free(visite);

//     return nbComposantes;
// }

void dijkstra(Network *network, Device device, uint16_t *distanceSommets)
{

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
        for (size_t i = 0; i < n; i++)
        {
            // si on ne l'a pas visité et que sa distance est la plus petite
            if (!visite[i] && distanceSommets[i] < min)
            {
                min = distanceSommets[i];
                sommetFixe = i;
            }
        }
        printf("test\n");
    }
}

Device *find_shortest_path(Network *network, Device *device, Device *destination, Device *path[])
{
    return 0;
}
