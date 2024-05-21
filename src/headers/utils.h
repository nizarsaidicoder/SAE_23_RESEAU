#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

uint16_t read_lines(char *filename, char **lines);
void write_line(char *filename, char *line);
void write_lines(char *filename, char **lines);
char **split(char *string, char *delimiter);
uint8_t hex_to_int(char *hex);