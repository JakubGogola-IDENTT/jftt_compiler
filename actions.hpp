#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>

#include "defs.hpp"

// Write compiled program to file.
void print_code(std::string filename);
// Check if symbol exists.
bool check_context(symbol sym);
// Generate constat value.
void gen_const(long long constant);
