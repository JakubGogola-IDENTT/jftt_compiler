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
    std::vector<std::string> cmds = this->gen_const(addr, A);
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
std::vector<std::string> code_generator::gen_const(long long c, enum reg r) {
    //TODO: make again
}

void code_generator::read_interact() {
    this->is_read = true;
}

void code_generator::write_interact() {
    this->is_write = true;
}