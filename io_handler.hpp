#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <streambuf>
#include <cstdio>
#include <vector>

class io_handler {
private:
    //Input file name
    std::string input_file;

    //Output file name
    std::string output_file;

    //Error color message
    const std::string error_alert = "\x1b[31merror\x1b[0m: ";

public:
    io_handler(int argc, char** argv);
    FILE *read_code();
    void print_code(std::vector<std::string> code);
};