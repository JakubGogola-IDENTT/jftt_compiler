#pragma once

#include <string>
#include <vector>
#include <map>

#include "defs.hpp"

class Data {
private:
    //Memory offset
    long long mem_offset;

    //Code offset
    long long code_offset;

    //Next free label
    long long next_label;

    //Symbols table
    std::map<std::string, symbol> sym_map;

    //Memory
    std::vector<long long> mem;

public:
    Data();
};