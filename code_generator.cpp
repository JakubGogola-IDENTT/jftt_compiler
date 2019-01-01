#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <limits>

#include "code_generator.hpp"
#include "data.hpp"
#include "defs.hpp"

code_generator::code_generator(std::shared_ptr<data> d) {
    this->d = d;
    this->is_read = false;
    this->is_write = false;

    this->gen_costs();
    this->gen_reg_sym();
}

/**
 * Generates map with instructions costs
 */
void code_generator::gen_costs() {
    this->costs.insert(std::pair<enum instr, int>(I_INC, 1));
    this->costs.insert(std::pair<enum instr, int>(I_HALF, 1));
    this->costs.insert(std::pair<enum instr, int>(I_ADD, 5));
    this->costs.insert(std::pair<enum instr, int>(I_SUB, 5));
    this->costs.insert(std::pair<enum instr, int>(I_COPY, 5));
}

/**
 * Generates map with register std::string symbols
 */
void code_generator::gen_reg_sym() {
    //Stupid but works
    this->reg_sym.insert(std::pair<enum reg, std::string>(A, "A"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(B, "B"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(C, "C"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(D, "D"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(E, "E"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(F, "F"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(G, "G"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(H, "H"));
}

//TODO: ADD INCREMENTING
/**
 * Increases offset when cmds added
 */ 
void code_generator::incr_offset(long long incr) {
    this->code_offset += incr;
}

/**
 * Returns generated cmds
 */
std::vector<std::string> code_generator::get_code() {
    return this->code;
}

/**
 * Writes HALT to the end of generated cmds
 */
void code_generator::end_prog() {
    this->code.push_back("HALT");
    this->incr_offset(1);
}

/**
 * Sets A register to get address in memory
 */ 
void code_generator::set_mem_reg(long long addr) {
    std::vector<std::string> cmds = this->gen_const(addr, A);
    cmds.insert(cmds.begin(), "SUB A A");
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}

/**
 * Used in case: pidentifier(pididentifier)
 */ 
void code_generator::set_mem_reg_nested_addr(long long addr) {
    this->set_mem_reg(addr);
    this->code.push_back("STORE A");
    this->incr_offset(1);
}

/**
 * Value from memory to register
 */ 
void code_generator::mem_to_reg(long long addr, enum reg r) {

}

/**
 * Value from register to memory
 */
void code_generator::reg_to_mem(enum reg r, long long addr) {

}

/**
 * Generates constant value
 */
std::vector<std::string> code_generator::gen_const(long long c, enum reg r) {
    std::vector<std::string> cmds;
    std::string sub_cmd = "SUB " + this->reg_sym[r] + " " + this->reg_sym[r];
    std::string inc_cmd = "INC " + this->reg_sym[r];
    std::string add_cmd = "ADD " + this->reg_sym[r] + " " + this->reg_sym[C];
    
    bool is_large = false;
    int inc_cost = 0;
    int add_cost = 0;

    //Set 0 to given register r
    cmds.push_back(sub_cmd);
    add_cost += this->costs[I_SUB];

    //Constant is 0 or 1
    if(c == 0) {
        return cmds;
    } else if(c == 1) {
        cmds.push_back(inc_cmd);
        return cmds;
    }

    if(c > 200) {
        is_large = true;
    }

    //Init given register with 2
    cmds.push_back(inc_cmd);
    cmds.push_back(inc_cmd);
    add_cost += 2 * this->costs[I_INC];

    //Value in given register
    long long tmp = 2;

    while(tmp + tmp < c) {
        tmp += tmp;
        cmds.push_back(add_cmd);
        add_cost += this->costs[I_ADD];
    }

    cmds.push_back("SUB " + this->reg_sym[C] + " " + this->reg_sym[C]);
    add_cost += this->costs[I_SUB];

    cmds.push_back("COPY " + this->reg_sym[C] + " " + this->reg_sym[r]);
    add_cost += this->costs[I_COPY];

    cmds.push_back("HALF " + this->reg_sym[C]);
    add_cost += this->costs[I_HALF];

    long long half = tmp / 2;

    while(half != 2) {
        if(tmp + half <= c) {
            tmp += half;
            cmds.push_back("ADD " + this->reg_sym[r] + " " + this->reg_sym[C]);
            add_cost += this->costs[I_ADD];
        }

        if(tmp == c) {
            break;
        }

        cmds.push_back("HALF " + this->reg_sym[C]);
        add_cost += this->costs[I_HALF];
        half /= 2;
    }

    while(tmp != c) {
        cmds.push_back(inc_cmd);
        add_cost += this->costs[I_INC];
        tmp++;
    }

    if(!is_large) {
        inc_cost = c * this->costs[I_INC] + this->costs[I_SUB];

        if(inc_cost < add_cost) {
            cmds.clear();
            cmds.push_back(sub_cmd);

            for(int i = 0; i < c; i++) {
                cmds.push_back(inc_cmd);
            }
        }
    }

    return cmds;
}

/**
 * Puts code to handle READ
 */
void code_generator::read() {

}

void code_generator::read_interact() {
    this->is_read = true;
}

void code_generator::write_interact() {
    this->is_write = true;
}