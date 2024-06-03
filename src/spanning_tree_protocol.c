#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
    elect_root_bridge(network);
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
