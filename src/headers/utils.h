#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

char **read_lines(FILE *file);
void write_line(FILE *file, char *line);
void write_lines(FILE *file, char **lines);
char **split(char *string, char *delimiter);
uint8_t hex_to_int(char *hex);