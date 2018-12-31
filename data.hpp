#pragma once

#include <string>
#include <vector>
#include <map>

#include "defs.hpp"

class data {
private:
    //Memory offset
    long long mem_offset;

    //Code offset
    long long code_offset;

    //Next free label
    long long next_label;

    //Symbols table
    std::map<std::string, std::shared_ptr<symbol>> sym_map;

    //Memory
    std::vector<long long> mem;

    //Error color message
    const std::string error_msg = "\x1b[31m error \x1b[0m: ";

    //Flag - true if errors occured
    bool errors;

public:
    data();

    //Checks if variable exists
    bool check_context(std::string name);
    void init_variable(std::string);

    void error_found();
    bool get_errors();

    long long put_symbol(std::string name);
    long long put_symbol_array(std::string name, long long start, long long end);

    long long alloc_mem();
    long long alloc_mem_array(long long start, long long end);


};