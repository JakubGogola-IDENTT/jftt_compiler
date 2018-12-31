#include <string>
#include <vector>
#include <map>

#include "Data.hpp"
#include "defs.hpp"

Data::Data() {
    this->mem_offset = 0;
    this->code_offset = 0;
    this->next_label = 0;
}
