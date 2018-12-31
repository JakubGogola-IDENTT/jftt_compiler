#include <iostream>
#include <fstream>
#include <string>

#include "io_handler.hpp"

io_handler::io_handler(int argc, char** argv) {
    if(argc != 3) {
        std::cerr << this->error_msg << "invalid number of arguments" << std::endl;
        exit(1);
    }

    this->input_file = argv[0];
    this->output_file = argv[1];

    
}

void io_handler::print_code() {

}