#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define MAC_BUFFER_SIZE 18
#define IP_BUFFER_SIZE 16
#define BPDU_BUFFER_SIZE 256
#define BPDU_FRAME_TYPE 0x4242

// An IP address is an array of 4 bytes (uint8_t) each representing an octet of the address
typedef struct IPAddress
{
    uint8_t address[4];
} IPAddress;

// A MAC address is an array of 6 bytes (uint8_t) each representing an octet of the address
typedef struct MACAddress
{
    uint8_t address[6];
} MACAddress;

// A station has an IP address
typedef struct Station
{
    IPAddress ip_address;
} Station;

// A port has a state and a role
// ROLES
// R: ROOT
// D: DESIGNATED
// B: BLOCKING
// STATES
// F: FORWARDING
// B: BLOCKING
// D: DISABLED
// L: LISTENING
typedef struct Port
{
    char role;
    char state;
} Port;

typedef struct SwitchingTableEntry
{
    MACAddress mac_address;
    uint8_t port_number;
} SwitchingTableEntry;

typedef struct BPDU
{
    uint16_t root_bridge_priority;
    MACAddress root_bridge_mac_address;
    uint16_t root_path_cost;
    MACAddress sender_mac_address;
    uint16_t sender_priority;
} BPDU;

// A switch has an IP address, a number of ports and a switching table and ports maybe ??
typedef struct Switch
{
    uint16_t priority;
    uint8_t num_ports;
    Port *ports;
    SwitchingTableEntry switching_table[256];
    uint16_t switching_table_entries;
    BPDU bpdu;
} Switch;

// A device can be a station or a switch
typedef enum DeviceType
{
    STATION,
    SWITCH
} DeviceType;

// A Device has a type (station or switch), a MAC address and a union of either a station or a switch
typedef struct Device
{
    DeviceType type;
    MACAddress mac_address;
    uint16_t index;
    union
    {
        Station station_info;
        // IT SEEMS THAT SWITCH IS A RESERVED WORD IN C SO I CHANGED IT TO switch_info
        Switch switch_info;
    };
} Device;

// A link is a connection between two devices "SIMILAR TO AN EDGE "ARETE" IN A GRAPH"
typedef struct Link
{
    // device 1 and device 2 are the devices that are connected by the link
    // SHOULD WE USE POINTERS TO THE DEVICES OR JUST THE DEVICES INDEXES IN THE NETWORK ??
    uint8_t device1_index;
    uint8_t device2_index;
    uint16_t weight;
} Link;

// A network has an array of devices and an array of links
typedef struct Network
{
    Device *devices;
    Link *links;
    uint16_t link_capacity;
    uint16_t device_capacity;
    uint8_t num_devices;
    uint16_t num_links;
    uint8_t num_stations;
    uint8_t num_switches;
} Network;
