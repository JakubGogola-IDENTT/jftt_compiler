#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <cstdio>
#include <vector>

#include "io_handler.hpp"

io_handler::io_handler(int argc, char** argv) {
    if(argc != 3) {
        std::cerr << this->error_alert << "invalid number of arguments" << std::endl;
        exit(1);
    }

    this->input_file = argv[1];
    this->output_file = argv[2];
}

FILE *io_handler::read_code() {
    FILE *fin = fopen(this->input_file.c_str(), "r");

    if(!fin) {
        std::cerr << this->error_alert << "can't open file: " << this->input_file << std::endl;
        return nullptr;
    }

    return fin;
}

void io_handler::print_code(std::vector<std::string> code) {
    std::ofstream fout(this->output_file);
    
    std::vector<std::string>::iterator it;
    for(it = code.begin(); it != code.end(); it++) {
        fout << *it << std::endl;
    }

    fout.close();
}