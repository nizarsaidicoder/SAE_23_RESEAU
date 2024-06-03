#include "headers/utils.h"
uint16_t read_lines(char *filename, char **lines)
{
    // Read all lines from the file
    FILE *file;
    char line[256];
    uint16_t num_lines = 0;
    file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }
    while (fgets(line, sizeof(line), file) != NULL)
    {
        size_t line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n')
            line[line_len - 1] = '\0'; // Remove the newline character
        lines[num_lines] = strdup(line);
        num_lines++;
    }
    fclose(file);

    return num_lines;
}

void write_lines(char *filename, char **lines)
{
    // Write the lines to the file
    FILE *file;
    file = fopen(filename, "w"); // Open the file in write mode "w"
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    for (int i = 0; lines[i] != NULL; i++)
        fprintf(file, "%s\n", lines[i]);
}

uint8_t split(char *string, char *delimiter, char **output)
{
    char *copie = strdup(string);
    char *token = strtok(copie, delimiter);
    uint8_t num_tokens = 0;
    while (token != NULL)
    {
        output[num_tokens] = strdup(token);
        num_tokens++;
        token = strtok(NULL, delimiter);
    }
    free(copie);
    return num_tokens;
}

uint8_t hex_to_int(char *hex)
{
    // Convert a hexadecimal string to an integer
    uint8_t num = 0;
    for (int i = 0; hex[i] != '\0'; i++)
    {
        num = num << 4; // Shift the number <=> multiply by 16
        if (hex[i] >= '0' && hex[i] <= '9')
            num += hex[i] - '0';
        else
            num += hex[i] - 'a' + 10;
    }
    return num;
}

char *int_to_hex(uint8_t num)
{
    // Convert an integer to a hexadecimal string
    char *hex = malloc(3);
    if (hex == NULL)
    {
        printf("Error allocating memory\n");
        return NULL;
    }
    sprintf(hex, "%02x", num);
    return hex;
}