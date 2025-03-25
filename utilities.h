#ifndef KLOBT_HUFFMAN_CODING__UTILITIES_
#define KLOBT_HUFFMAN_CODING__UTILITIES_

#include <stdio.h>
#include <stdnoreturn.h>

noreturn void panic(const char *message);

void parse_arguments(int argc, char *argv[], const char *program_name, FILE **input_file, FILE **output_file);
void close_files(FILE *input_file, FILE *output_file);

#endif
