#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "utilities.h"

noreturn void panic(const char *message)
{
    fprintf(stderr, "[PANIC] %s\n", message);
    exit(EXIT_FAILURE);
}


void usage(const char *executable_name) {
    fprintf(stderr, "usage: %s -i <input_file> -o <output_file> [-v] [-h]\n", executable_name);
}

void version(const char *program_name) {
    fprintf(stderr, "%s %s\n", program_name, VERSION);
}

void parse_arguments(int argc, char *argv[], const char *program_name, FILE **input_file, FILE **output_file) {
    const char *input_filename = NULL, *output_filename = NULL;
    int option;
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "i:o:vh", long_options, NULL)) != -1) {
        switch (option) {
            case 'i':
                input_filename = optarg;
                break;
            case 'o':
                output_filename = optarg;
                break;
            case 'v':
                version(DECODE_PROGRAM_NAME);
                exit(EXIT_FAILURE);
            case 'h':
                usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (input_filename == NULL || output_filename == NULL) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(input_filename, "-") == 0) {
        *input_file = stdin;
    } else if ((*input_file = fopen(input_filename, "r")) == NULL) {
        perror(DECODE_PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }

    if (strcmp(output_filename, "-") == 0) {
        *output_file = stdout;
    } else if ((*output_file = fopen(output_filename, "w")) == NULL) {
        perror(DECODE_PROGRAM_NAME);
        exit(EXIT_FAILURE);
    }
}

void close_files(FILE *input_file, FILE *output_file) {
    if (input_file != stdin) {
        fclose(input_file);
    }
    if (output_file != stdout) {
        fclose(output_file);
    }
}

bool is_little_endian(void) {
    union {
        unsigned long long _long;
        char bytes[8];
    } long_bytes;

    long_bytes._long = 1;

    return long_bytes.bytes[0] == 1;
}

void long_to_little_endian(unsigned long long input, char output[8]) {
    union {
        unsigned long long _long;
        char bytes[8];
    } long_bytes;
    bool _is_little_endian;
    size_t left_index;

    long_bytes._long = input;

    _is_little_endian = is_little_endian();
    for (size_t index = 0; index < 8; index++) {
        left_index = _is_little_endian ? index : 7 - index;
        output[left_index] = long_bytes.bytes[index];
    }
}

unsigned long long little_endian_to_long(char input[8]) {
    union {
        unsigned long long _long;
        char bytes[8];
    } long_bytes;
    bool _is_little_endian;
    size_t left_index;

    _is_little_endian = is_little_endian();
    for (size_t index = 0; index < 8; index++) {
        left_index = _is_little_endian ? index : 7 - index;
        long_bytes.bytes[left_index] = input[index];
    }

    return long_bytes._long;
}
