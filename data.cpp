#include <string>
#include <vector>
#include <map>

#include "data.hpp"
#include "defs.hpp"

data::data() {
    this->mem_offset = 0;
    this->code_offset = 0;
    this->next_label = 0;
}

/**
 * Checks if symbol exists.
 */
bool data::check_context(std::string name) {
    std::map<std::string, symbol>::iterator it = this->sym_map.find(name);

    if(it != sym_map.end()) {
        return true;
    } else {
        return false;
    }
}