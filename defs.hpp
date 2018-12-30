#pragma once

#include <string>
#include <vector>
#include <map>

// Struct for symbol in symbols array
struct symbol {
    std::string name;
    unsigned long long offset;
    bool is_init;
    bool is_array;
    unsigned long long array_start;
    unsigned long long array_end;

    // Not array variable
    symbol(std::string name, unsigned long long offset) {
        this->name = name;
        this->offset = offset;
        this->is_init = false;
        this->is_array = false;
        this->array_start = 0;
        this->array_end = 0;
    }

    // Array variable
    symbol(std::string name, unsigned long long offset, unsigned long long array_start, unsigned long long array_end) {
        this->name = name;
        this->offset = offset;
        this->is_init = false;
        this->is_array = true;
        this->array_start = array_start;
        this->array_end = array_end;
    }
};

// Struct for temporary labels
struct label {
    unsigned long long jump_true;
    unsigned long long jump_false;

    label(unsigned long long jump_true, unsigned long long jump_false) {
        this->jump_true = jump_true;
        this->jump_false = jump_false;
    }
};

// Struct for code blocks: IF, IF-ELSE, WHILE, FOR
// TODO: I'don't know if it is unecessarry..
struct block {
    //TODO: implement
    unsigned long long offset;
    std::vector<std::string> cmds;
};

struct utils {
    // Memory offset
    unsigned long long mem_offset;
    // Code offset
    unsigned long long code_offset;
    // Last label
    unsigned long long next_label;

    // TODO: Check if using std::map is faster
    std::map<std::string, symbol> sym_map;

    utils() {
        this->mem_offset = 0;
        this->code_offset = 0;
        this->next_label = 0;
    }
};