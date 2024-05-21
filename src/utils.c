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
void write_line(char *filename, char *line)
{
    // Write the line to the file
}
void write_lines(char *filename, char **lines)
{
    // Write the lines to the file
}
char **split(char *string, char *delimiter)
{
    // Split the string into tokens using the delimiter
    return NULL;
}

uint8_t hex_to_int(char *hex)
{
    // Convert a hexadecimal string to an integer
    return 0;
}