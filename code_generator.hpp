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

    //Std::string symbols of registers
    std::map<enum reg, std::string> reg_sym;

    /*FUNCTIONS*/

    //Generate costs map
    void gen_costs();
    void gen_reg_sym();

    long long get_label();
    void incr_offset(long long incr);

    void set_mem_reg(variable *var);
    void single_var(variable *var, enum reg r);

public:
    code_generator(std::shared_ptr<data> d);

    std::vector<std::string> get_code();
    void array_offset(long long addr, long long offset);
    void end_prog();

    long long get_code_offset();

    void mem_to_reg(variable *var, enum reg r);
    void reg_to_mem(enum reg r, variable *var);

    std::vector<std::string> gen_const(long long c, enum reg r); 

    //operations
    void assign(variable *var);
    void constant(variable *var);

    void add(variable *v_1, variable *v_2);
    void sub(variable *v_1, variable *v_2);
    void mul(variable *v_1, variable *v_2);
    void div(variable *v_1, variable *v_2);
    void rem(variable *v_1, variable *v_2);
    
    //conditions
    void eq();
    void neq();
    void gt();
    void lt();
    void geq();
    void leq();

    //IO
    void write(variable *var);
    void read(variable *var);



    void read_interact();
    void write_interact();
};