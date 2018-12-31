#pragma once

#include <string>
#include <vector>
#include <map>

#include "defs.hpp"

class data {
private:
    //Memory offset
    long long mem_offset;

    //Symbols table
    std::map<std::string, std::shared_ptr<symbol>> sym_map;

    //Memory
    std::vector<long long> mem;

    //Error color message
    const std::string error_alert = "\x1b[31merror\x1b[0m: ";

    //Flag - true if errors occured
    bool errors;

    //Numbers of found errors
    int errors_num;

public:
    data();

    //Checks if variable exists
    bool check_context(std::string name);
    void init_variable(std::string);

    void error_found();
    bool get_errors();
    int get_errors_num();

    long long alloc_mem();
    long long alloc_mem_array(long long start, long long end);

    long long put_symbol(std::string name);
    long long put_symbol_array(std::string name, long long start, long long end);
    long long put_value(long long value);
    symbol *get_symbol(std::string name);

    variable *get_variable(std::string name);
    variable *get_variable_array_var(std::string name, std::string var_name);
    variable *get_variable_array_num(std::string name, long long num);

    variable_value *get_value(variable *var);
    variable_value *get_value_num();
    //TODO: long long get_value();
};