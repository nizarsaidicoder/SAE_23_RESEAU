#pragma once
#include "types.h"
#include "device.h"
#include "utils.h"
#include "frame.h"
#include "network.h"

void spanning_tree_protocol(Network *network);
void elect_root_bridge(Network *network);
