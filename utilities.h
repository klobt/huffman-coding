#ifndef KLOBT_HUFFMAN_CODING__UTILITIES_H_
#define KLOBT_HUFFMAN_CODING__UTILITIES_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdnoreturn.h>

noreturn void panic(const char *message);

void parse_arguments(int argc, char *argv[], const char *program_name, FILE **input_file, FILE **output_file);
void close_files(FILE *input_file, FILE *output_file);

bool is_little_endian(void);
void long_to_little_endian(unsigned long long input, char output[8]);
unsigned long long little_endian_to_long(char input[8]);

#endif
