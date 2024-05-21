#include "headers/types.h"
#include "headers/address.h"
#include "headers/device.h"
#include "headers/network.h"
#include "headers/utils.h"
int main()
{
    char **lines = (char **)malloc(256 * sizeof(char *));
    uint16_t n = read_lines("../config/mylan.lan", lines);
    for (int i = 0; i < n; i++)
    {
        printf("%s\n", lines[i]);
    }
    return 0;
}