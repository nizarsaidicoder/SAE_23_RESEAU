#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

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

// A switch has an IP address, a number of ports and a switching table and ports maybe ??
typedef struct Switch
{
    uint16_t priority;
    uint8_t num_ports;
    Port *ports;
    MACAddress *switching_table;
} Switch;

// A device can be a station or a switch
typedef enum DeviceType
{
    STATION,
    SWITCH
} DeviceType;

// A port has a number and a status (up or down)
// PROBLEM : A port doesn't have a unique identifier, so we can't use it to identify a port in a switch in the network
// IMAGINE WE HAVE A SWITCH 1 WITH 4 PORTS, AND A SWITCH 2 WITH 4 PORTS, PORT 1 IN SWITCH 1 IS NOT THE SAME AS PORT 1 IN SWITCH 2
// SO WE NEED TO ADD A SWITCH ID TO THE PORT STRUCTURE MAYBE ??
// OR WE CAN USE THE PORT NUMBER AND THE SWITCH MAC ADDRESS TO IDENTIFY A PORT
// IDK, I'M JUST THINKING OUT LOUD
typedef struct Port
{
    uint8_t number;
    char status;
} Port;

// A Device has a type (station or switch), a MAC address and a union of either a station or a switch
typedef struct Device
{
    DeviceType type;
    MACAddress mac_address;
    union
    {
        Station staion_info;
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
} Link;

// A network has an array of devices and an array of links
typedef struct Network
{
    Device *devices;
    Link *links;
    uint8_t num_devices;
    uint16_t num_links;
} Network;
