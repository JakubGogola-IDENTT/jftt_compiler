#pragma once

#include <iostream>
#include <fstream>
#include <string>

class io_handler {
private:
    //Input file name
    std::string input_file;

    //Output file name
    std::string output_file;

    //Error color message
    const std::string error_msg = "\x1b[31m error \x1b[0m: ";

public:
    io_handler(int argc, char** argv);
    void print_code();
};