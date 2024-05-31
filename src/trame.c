#include "headers/types.h"
#include "headers/trame.h"
#include "headers/address.h"

void trame_print(Trame * trame)
{
    printf("------------------Trame------------------\n");
    printf("Preambule: %d.%d.%d.%d.%d.%d.%d\n", trame->preambule[0], trame->preambule[1], trame->preambule[2], trame->preambule[3], trame->preambule[4], trame->preambule[5], trame->preambule[6]);
    printf("SFD: %d\n", trame->SFD);
    printf("Destinataire: ");
    print_mac_address(&trame->dest);
    printf("Source: ");
    print_ip_address(&trame->src);
    printf("Type: %d", trame->type);
}

void trame_init(Trame *trame, MACAddress dest, MACAddress src, uint16_t type, uint8_t *data)
{
    //Initialisation de preambule
    for(int i = 0; i < 7; i++)
    {
        trame->preambule[i] = 170;
    }

    trame->SFD = 171;
    trame->dest = dest;
    trame->src = src;
    trame->type = type;
    trame->data = data;
}