#pragma once
#include "types.h"
#include "device.h"
#include "utils.h"
#include "frame.h"
#include "network.h"

// spannig_tree_protocol(network)
//     elect_root_bridge(network)
//     for each switch in network
//         find_shortest_path(switch, root_bridge, path)
//         for each port in switch
//             set port state to blocking if port is not in path
//         end
//     end
// end
void spanning_tree_protocol(Network *network);
void assign_bpdu(Device *device);
int compare_bpdu(BPDU *bpdu1, BPDU *bpdu2);
void bpdu_to_frame(BPDU *bpdu, Frame *frame);
void frame_to_bpdu(Frame *frame, BPDU *bpdu);
void bpdu_to_string(BPDU *bpdu, char *output);
void print_bpdu(BPDU *bpdu);
void send_bpdu(Network *network, Device *device);
int find_shortest_distance_cost(Network *network, Device *device, Device *destination);
bool receive_bpdu(Network *network, Device *device, Frame *frame);
void elect_root_bridge(Network *network);
void visite_composante_connexe(Network *network, Device *device, bool *visite);
uint32_t nb_composantes_connexes(Network *network);
void dijkstra(Network *network, Device device, uint16_t *distance_sommet);
Device *find_shortest_path(Network *network, Device *device, Device *destination, Device *path[]);
void update_ports_state(Device *device, Device *destination, Device *path[]);
