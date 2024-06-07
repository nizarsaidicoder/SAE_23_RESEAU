#include "headers/utils.h"
/**
 * @brief Reads the lines from a file
 * @param filename The name of the file
 * @param lines The array of lines
 * @return The number of lines of the file
 */
uint16_t read_lines(char *filename, char **lines)
{
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
/**
 * @brief Writes the lines to a file
 * @param filename The name of the file
 * @param lines The array of lines
 */
void write_lines(char *filename, char **lines)
{
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
/**
 * @brief Splits a string into tokens
 * @param string The string to split
 * @param delimiter The delimiter
 * @param output The output tokens
 * @return The number of tokens
 */
uint8_t split(char *string, char *delimiter, char **output)
{

    uint8_t num_tokens = 0;
    char *token = string;
    char *end = string;
    while (*end != '\0')
    {
        if (*end == *delimiter)
        {
            *end = '\0';
            output[num_tokens] = token;
            num_tokens++;
            token = end + 1;
        }
        end++;
    }
    output[num_tokens] = token;
    num_tokens++;
    return num_tokens;
}
/**
 * @brief Converts a hexadecimal string to an integer
 * @param hex The hexadecimal string
 * @return The integer
 */
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
