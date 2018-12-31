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

    //Code offset
    long long code_offset;

    //Next free label
    long long next_label;

    //Genearted code
    std::vector<std::string> code;

    //Cost of some instructions
    std::map<enum instr, int> costs;

    /*FUNCTIONS*/

    //Generate costs map
    void gen_costs();

    //Gets label
    long long get_label();

    //Increases offset when code added
    void incr_offset(long long incr);

public:
    code_generator(std::shared_ptr<data> d);

    void set_mem_reg(long long addr);

    // Returns generated code
    std::vector<std::string> get_code();

    // Generate constat value.
    std::vector<std::string> gen_const(long long c, std::string reg); 

    // Program is reading sth from stdin
    void read_interact();

    // Program is writing sth to stdout
    void write_interact();

    // Writes HALT to the end of generated code
    void end_prog();
};