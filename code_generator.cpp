#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "code_generator.hpp"
#include "data.hpp"
#include "defs.hpp"

code_generator::code_generator(std::shared_ptr<data> d) {
    this->d = d;
    this->is_read = false;
    this->is_write = false;
}

void code_generator::print_code(std::string filename) {
    std::cout << "Printing code..." << std::endl;

    //Compilation finished
    exit(1);
}

void code_generator::gen_const(long long constant) {

}

void code_generator::read_interact() {
    this->is_read = true;
}

void code_generator::write_interact() {
    this->is_write = true;
}