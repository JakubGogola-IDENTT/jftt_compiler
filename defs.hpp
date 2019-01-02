#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * Some instructions
 */
enum instr {
    I_INC,
    I_HALF,
    I_ADD,
    I_SUB,
    I_COPY
};

enum reg {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H
};

/**
 * Struct for symbol in symbols array
 */ 
struct symbol {
    std::string name;
    long long offset;
    long long value;
    bool is_init;
    bool is_const;
    bool is_array;
    bool is_iterator;
    long long array_start;
    long long array_end;
    long long size;

    // Not array variable
    symbol(std::string name, long long offset) {
        this->name = name;
        this->offset = offset;
        this->is_init = false;
        this->is_iterator = false;
        this->is_const = false;
        this->is_array = false;
        this->array_start = 0;
        this->array_end = 0;
        this->size = 1;
    }

    // Array variable
    symbol(std::string name, long long offset, long long array_start, long long array_end) {
        this->name = name;
        this->offset = offset;
        this->is_init = true; //TODO: CHECK THIS SHIT!!!!!!
        this->is_iterator = false;
        this->is_const = false;
        this->is_array = true;
        this->array_start = array_start;
        this->array_end = array_end;
        this->size = array_end - array_start + 2;
    }

    //Only value, not variable
    symbol(std::string name, long long offset, bool is_const) {
        this->name = name;
        this->offset = offset;
        this->is_init = true;
        this->is_iterator = false;
        this->is_const = is_const;
        this->is_array = false;
        this->array_start = 0;
        this->array_end = 0;
        this->size = 1;
    }
};

/**
 * Struct for temporary labels (IF, WHILE)
 */
struct label {
    long long go_to;
    long long jump_false;

    label(long long go_to, long long jump_false) {
        this->go_to = go_to;
        this->jump_false = jump_false;
    }
};

/**
 * 
 */
struct for_label {

};

struct cond_label {
    long long start;
    long long go_to;

    cond_label(long long start, long long go_to) {
        this->start = start;
        this->go_to = go_to;
    }
};

/**
 * Struct to pass variable addres
 */
struct variable {
    //Array cell
    long long array_addr;

    //Number of array cell
    long long addr;

    //Value for constant variable
    long long value;

    //Single variable
    variable(long long addr) {
        this->array_addr = -1;
        this->addr = addr;
        this->value = -1;
    }

    //Array cell
    variable(long long array_addr, long long addr) {
        this->array_addr = array_addr;
        this->addr = addr;
        this->value = -1;
    }

    variable(long long array_addr, long long addr, long long value) {
        this->array_addr = array_addr;
        this->addr = addr;
        this->value = value;
    }  
};

