#pragma once
#include "network.h"
#include "device.h"
#include "frame.h"
#include "utils.h"
#include "types.h"

bool update_bpdu(Network *network, Device *device, Device *previous_device, BPDU *new_bpdu);
void send_bpdu(Network *network, Device *device);
void print_bpdu(BPDU *bpdu);
void bpdu_to_frame(BPDU *bpdu, Frame *frame);
void frame_to_bpdu(Frame *frame, BPDU *bpdu);
void bpdu_to_string(BPDU *bpdu, char *output);
int compare_bpdu(BPDU *bpdu1, BPDU *bpdu2);
void assign_bpdu(Device *device);
