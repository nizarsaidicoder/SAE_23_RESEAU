#include "headers/spanning_tree_protocol.h"
#include "headers/device.h"
#include "headers/utils.h"
#include "headers/frame.h"
#include "headers/network.h"

void spanning_tree_protocol(Network *network)
{
}

void elect_root_bridge(Network *network)
{
}

uint16_t find_shortest_path(Network *network, Device *device, Device *destination, int path[])
{
    return 0;
}

// void dijkstra(graphe const *g, sommet s, double const *poids_arete, double *distance_sommet)
//  {
//  Initialise all distances as infinite
//  Set all vertices as not visited
//  Set the distance of the source vertex as 0
//  Set the source vertex as the current vertex
//  for each connected vertex to the source vertex
//       if the distance of the vertex is greater than the distance of the source vertex + the weight of the edge
//           update the distance of the vertex
//       end if
//       mark the vertex as visited
//       set the current vertex as the vertex with the smallest distance
//  end for
//  repeat the previous steps until all vertices are visited
//     if(nb_composantes_connexes(g) != 1) return;
//     size_t ordre_graphe = ordre(g);
//     bool sommets_visite[ordre_graphe];
//     for(size_t i = 0; i < ordre_graphe; i++) sommets_visite[i] = false;
//     for(size_t i = 0; i < ordre_graphe; i++) distance_sommet[i] = DBL_MAX;
//     distance_sommet[s] = 0;
//     sommet current_sommet = s;
//     // We'll use the function index_arete to get the index of the edge
//     // between the current vertex and the vertex we're looking at
//     while(!sommets_visite[current_sommet])
//     {
//         sommets_visite[current_sommet] = true;
//         sommet sommets_adjac[4096];
//         size_t nb_sommets = sommets_adjacents(g, current_sommet, sommets_adjac);
//         for(size_t i = 0; i < nb_sommets; i++)
//         {
//             arete a;
//             a.s1 = current_sommet;
//             a.s2 = sommets_adjac[i];
//             size_t index = index_arete(g, a);
//             if(distance_sommet[sommets_adjac[i]] > distance_sommet[current_sommet] + poids_arete[index])
//             {
//                 distance_sommet[sommets_adjac[i]] = distance_sommet[current_sommet] + poids_arete[index];
//             }
//         }
//         double min = DBL_MAX;
//         for(size_t i = 0; i < ordre_graphe; i++)
//         {
//             if(!sommets_visite[i] && distance_sommet[i] < min)
//             {
//                 min = distance_sommet[i];
//                 current_sommet = i;
//             }
//         }
//     }
// }