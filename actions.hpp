#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "defs.hpp"

// Write compiled program to file.
void write_to_file(std::string filename);
// Check if symbol exists.
bool check_context(symbol sym);
// Generate constat value.
void gen_const(unsigned long long constant);
