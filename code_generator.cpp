#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "code_generator.hpp"
#include "data.hpp"
#include "defs.hpp"

code_generator::code_generator(std::shared_ptr<data> d) {
    this->d = d;
}

void code_generator::print_code(std::string filename) {

}

bool code_generator::check_context(symbol sym) {
    
}

void code_generator::gen_const(long long constant) {

}