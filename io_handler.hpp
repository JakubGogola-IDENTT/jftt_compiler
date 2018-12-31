#pragma once

#include <iostream>
#include <fstream>
#include <string>

class io_handler {
private:
    std::string input_file;
    std::string output_file;

public:
    io_handler(int argc, char** argv);
};