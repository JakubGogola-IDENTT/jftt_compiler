#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "defs.hpp"

// Write compiled program to file.
void write_to_file(std::string filename);
bool check_context(symbol sym);
void stupid_func();