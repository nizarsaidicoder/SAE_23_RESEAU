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
void send_bpdu(Device *device, Device *destination, Frame *bpdu);
void receive_bpdu(Device *device, Frame *bpdu);
void elect_root_bridge(Network *network);
Device * find_shortest_path_djikstra(Network * network, Device *device, Device *destination, Device * path[]);
void update_ports_state(Device *device, Device *destination, Device * path[]);
// uint8_t compare_bpdu(BPDU *bpdu1, BPDU *bpdu2);



