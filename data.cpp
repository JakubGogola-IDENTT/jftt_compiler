#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "data.hpp"
#include "defs.hpp"

data::data() {
    this->mem_offset = 0;
    this->code_offset = 0;
    this->next_label = 0;
    this->errors = true;
}

/**
 * Checks if symbol exists.
 */
bool data::check_context(std::string name) {
    std::map<std::string, std::shared_ptr<symbol>>::iterator it = this->sym_map.find(name);

    if(it != sym_map.end()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Sets flag of initialization in variable
 */
void data::init_variable(std::string name) {
    if(this->check_context(name)) {
        this->sym_map[name]->is_init = true;
    } else {
        std::cerr << this->error_msg << "variable is not defined" << std::endl;
    }
    //TODO: finish!!!!
}

/**
 * Set errors flag true
 */
void data::error_found() {
    this->errors = true;
}

/**
 * Returns errors flag
 */
bool data::get_errors() {
    return this->errors;
}

/**
 * Allocates memory for variable
 */
long long data::alloc_mem() {
    this->mem_offset++;
    return this->mem_offset;
}

/**
 * Allocates memory for array, but returns begining addres of array in memory
 */
long long data::alloc_mem_array(long long start, long long end) {
    long long size = end - start + 2;
    this->mem_offset += size;
    return this->mem_offset - size - 1;
}

/**
 * Puts symbol into symbol table
 */
long long data::put_symbol(std::string name) {
    if(this->check_context(name)) {
        std::cerr << this->error_msg << name << " is already defined" << std::endl;
        return -1;
    }

    long long offset = this->alloc_mem();
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset);
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}

/**
 * Puts array symbol into table
 */
long long data::put_symbol_array(std::string name, long long start, long long end) {
    if(this->check_context(name)) {
        std::cerr << this->error_msg << name << " is already defined" << std::endl;
        return -1;
    }

    if(end < start) {
        std::cerr << this->error_msg << "ending index can't be smaller than starting index" << std::endl;
    }

    long long offset = this->alloc_mem_array(start, end);
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset, start, end);
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}