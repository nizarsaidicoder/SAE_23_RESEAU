#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

uint16_t read_lines(char *filename, char **lines);
void write_lines(char *filename, char **lines);
uint8_t split(char *string, char *delimiter, char **output);
uint8_t hex_to_int(char *hex);
char *int_to_hex(uint8_t num);
