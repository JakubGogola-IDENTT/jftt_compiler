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
    //Data object (symbols table and memory)
    std::shared_ptr<data> d;

    //Flag - true if sth is read from stdin
    bool is_read;

    //Flag - true if sth is wrtite to stdout
    bool is_write;

    //Genearted code
    std::vector<std::string> code;

    //Cost of some instructions
    std::map<enum instr, int> costs;

    //Generate costs map
    void gen_costs();

public:
    code_generator(std::shared_ptr<data> d);

    // Returns generated code
    std::vector<std::string> get_code();

    // Generate constat value.
    void gen_const(long long c); 

    // Program is reading sth from stdin
    void read_interact();

    // Program is writing sth to stdout
    void write_interact();

    // Writes HALT to the end of generated code
    void end_prog();
};