#pragma once

#include <string>
#include <vector>
#include <map>

// Struct for symbol in symbols array
struct symbol {
    // std::string name;
    unsigned long long offset;
    bool is_array;
    unsigned long long array_start;
    unsigned long long array_end;
    // TODO: I don't know if pointer to next struct is unecessary xD (use std::vector or std::map)

    symbol(unsigned long long offset) {
        this->offset = offset;
        this->is_array = false;
        this->array_start = 0;
        this->array_end = 0;
    }

    symbol(unsigned long long offset, unsigned long long array_start, unsigned long long array_end) {
        this->offset = offset;
        this->is_array = true;
        this->array_start = array_start;
        this->array_end = array_end;
    }
};

// Struct for temporary labels
struct label {
    unsigned long long jump_true;
    unsigned long long jump_false;
};

// Memory offset
unsigned long long mem_offset = 0;
// Code offset
unsigned long long code_offset = 0;
// Last label
unsigned long long label_offset = 0;

// TODO: Check if using std::map is faster
// std::vector<symbol> sym_arr;
std::map<std::string, symbol> sym_map;

