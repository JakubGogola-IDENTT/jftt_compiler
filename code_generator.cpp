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
}

void code_generator::gen_costs() {
    this->costs.insert(std::pair<enum instr, int>(I_INC, 1));
    this->costs.insert(std::pair<enum instr, int>(I_HALF, 1));
    this->costs.insert(std::pair<enum instr, int>(I_ADD, 5));
    this->costs.insert(std::pair<enum instr, int>(I_SUB, 5));
    this->costs.insert(std::pair<enum instr, int>(I_COPY, 5));
}

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
}

/**
 * Sets A register to get address in memory
 */ 
void code_generator::set_mem_reg(long long addr) {
    std::vector<std::string> cmds = this->gen_const(addr, "A");
    cmds.insert(cmds.begin(), "SUB A A");
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
}

/**
 * Used in case: pidentifier(pididentifier)
 */ 
void code_generator::set_mem_reg_nested_addr(long long addr) {
    this->set_mem_reg(addr);
    this->code.push_back("STORE A");
}

/**
 * Generates constant value
 */
std::vector<std::string> code_generator::gen_const(long long c, std::string reg) {
    //TODO: write to memory
    std::vector<std::string> cmds;
    bool is_large = false;
    long long inc_cost = std::numeric_limits<long long>::max();
    long long mix_cost = 0;

    std::string sub_cmd = "SUB " + reg + " " + reg;
    std::string inc_cmd = "INC " + reg;

    if(c == 0) {
        cmds.push_back(sub_cmd);
        //TODO: STORE
    } else if(c == 1) {
        cmds.push_back(sub_cmd);
        cmds.push_back(inc_cmd);
    }

    //TODO: return @up!!!!!!!!!!!!!!!!!!!!!!!!!

    if(c > 200) {
        is_large = true;
    }
    
    //Generating constant using INC and ADD instructions
    cmds.push_back(sub_cmd);
    mix_cost += this->costs[I_SUB];
    cmds.push_back(inc_cmd);
    cmds.push_back(inc_cmd);
    mix_cost += 2 * this->costs[I_SUB];

    //Value in register B
    long long tmp = 2;

    while(tmp + tmp < c) {
        tmp += tmp;
        //ADD B B!!!
        cmds.push_back("ADD " + reg + " " + reg);
        mix_cost += this->costs[I_ADD];
    }

    //Half of value in register B
    cmds.push_back("SUB C C");
    mix_cost += this->costs[I_SUB];

    cmds.push_back("COPY C " + reg);
    mix_cost += this->costs[I_COPY];

    cmds.push_back("HALF C");
    mix_cost += this->costs[I_HALF];

    long long half = tmp / 2;

    while(half != 2) {
        if(tmp + half <= c) {
            tmp += half;
            cmds.push_back("ADD " + reg + " C");
            mix_cost += this->costs[I_ADD];
        }

        if(tmp == c) {
            break;
        }

        cmds.push_back("HALF C");
        mix_cost += this->costs[I_HALF];
        half /= 2;
    }

    while(tmp != c) {
        cmds.push_back(inc_cmd);
        mix_cost += this->costs[I_INC];
        tmp++;
    }

    if(!is_large) {
        inc_cost = c;
    }

    if(inc_cost < mix_cost) {
        cmds.clear();
        cmds.push_back(sub_cmd);

        for(int i = 0; i < c; i++) {
            cmds.push_back(inc_cmd);
        }
    }

    return cmds;
}

void code_generator::read_interact() {
    this->is_read = true;
}

void code_generator::write_interact() {
    this->is_write = true;
}