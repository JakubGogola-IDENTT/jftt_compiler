#pragma once

#include <string>

// Memory offset
unsigned long long mem_offset = 0;

// Code offset
unsigned long long code_offset = 0;

// Last label
unsigned long long label_offset = 0;

// Struct for symbol in symbols array
struct symbol {
    std::string name;
    unsigned long long offset;
    bool is_array;
    unsigned long long array_start;
    unsigned long long array_end;
    // TODO: I don't know if pointer to next struct is unecessary xD (use std::vector)
};

// Struct for temporary labels
struct label {
    unsigned long long jump_true;
    unsigned long long jump_false;
};
