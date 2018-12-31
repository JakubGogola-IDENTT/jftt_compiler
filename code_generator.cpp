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
 * Returns generated code
 */
std::vector<std::string> code_generator::get_code() {
    return this->code;
}

/**
 * Writes HALT to the end of generated code
 */
void code_generator::end_prog() {
    this->code.push_back("HALT");
}

/**
 * Generates constant value
 */
void code_generator::gen_const(long long c) {
    //TODO: write to memory
    std::vector<std::string> code;
    bool is_large = false;
    long long inc_cost = std::numeric_limits<long long>::max();
    long long mix_cost = 0;

    if(c == 0) {
        code.push_back("SUB B B");
        //TODO: STORE
    } else if(c == 1) {
        code.push_back("SUB B B");
        code.push_back("INC B");
    }

    //TODO: return @up!!!!!!!!!!!!!!!!!!!!!!!!!

    if(c > 200) {
        is_large = true;
    }
    
    //Generating constant using INC and ADD instructions
    code.push_back("SUB B B");
    mix_cost += this->costs[I_SUB];
    code.push_back("INC B");
    code.push_back("INC B");
    mix_cost += 2 * this->costs[I_SUB];

    //Value in register B
    long long tmp = 2;

    while(tmp + tmp < c) {
        tmp += tmp;
        code.push_back("ADD B B");
        mix_cost += this->costs[I_ADD];
    }

    //Half of value in register B
    code.push_back("SUB C C");
    mix_cost += this->costs[I_SUB];

    code.push_back("COPY C B");
    mix_cost += this->costs[I_COPY];

    code.push_back("HALF C");
    mix_cost += this->costs[I_HALF];

    long long half = tmp / 2;

    while(half != 2) {
        if(tmp + half <= c) {
            tmp += half;
            code.push_back("ADD B C");
            mix_cost += this->costs[I_ADD];
        }

        if(tmp == c) {
            break;
        }

        code.push_back("HALF C");
        mix_cost += this->costs[I_HALF];
        half /= 2;
    }

    while(tmp != c) {
        code.push_back("INC B");
        mix_cost += this->costs[I_INC];
        tmp++;
    }

    if(!is_large) {
        inc_cost = c;
    }

    if(inc_cost < mix_cost) {
        code.clear();
        code.push_back("SUB B B");

        for(int i = 0; i < c; i++) {
            code.push_back("INC B");
        }
    }

}

void code_generator::read_interact() {
    this->is_read = true;
}

void code_generator::write_interact() {
    this->is_write = true;
}