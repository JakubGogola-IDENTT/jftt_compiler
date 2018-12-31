#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "defs.hpp"
#include "data.hpp"

class code_generator {
private:
    std::shared_ptr<data> d;

public:
    code_generator(std::shared_ptr<data> d);

    // Write compiled program to file.
    void print_code(std::string filename);

    // Check if symbol exists.
    bool check_context(symbol sym);

    // Generate constat value.
    void gen_const(long long constant); 

};