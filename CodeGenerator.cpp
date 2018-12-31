#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <vector>
#include <string>

#include "CodeGenerator.hpp"
#include "Data.hpp"
#include "defs.hpp"

CodeGenerator::CodeGenerator(std::shared_ptr<Data> data) {
    this->data = data;
}

void CodeGenerator::print_code(std::string filename) {

}

bool CodeGenerator::check_context(symbol sym) {
    
}

void CodeGenerator::gen_const(long long constant) {

}