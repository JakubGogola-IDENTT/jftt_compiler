#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>

#include "data.hpp"
#include "defs.hpp"

data::data() {
    this->mem_offset = 0;
    this->errors = false;
    this->errors_num = 0;
    this->border_symbol = 0;
    this->line = 0;
}

/**
 * Checks if symbol exists.
 */
bool data::check_context(std::string name) {
    std::map<std::string, std::shared_ptr<symbol>>::iterator it = this->sym_map.find(name);

    if(it != sym_map.end()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Sets flag of initialization in variable
 */
void data::init_variable(std::string name) {
    if(this->check_context(name)) {
        if(this->sym_map[name]->is_iterator) {
            std::cerr << this->error_alert << name << " - iterator can't be modified" << this->put_line() << std::endl;
            this->error_found();
        } else {
            this->sym_map[name]->is_init = true;
        }
    } else {
        std::cerr << this->error_alert << name << " - variable is not defined" << this->put_line() << std::endl;
        this->error_found();
    }
}

/**
 * Set errors flag true
 */
void data::error_found() {
    if(!this->errors) {
        this->errors = true;
    }

    this->errors_num++;
}

/**
 * Returns errors flag
 */
bool data::get_errors() {
    return this->errors;
}

/**
 * Sets number of current line;
 */
void data::set_line(int line) {
    this->line = line;
}

/**
 * Puts info about line number
 */
std::string data::put_line() {
    std::stringstream ss;
    ss << " (line " << this->line << ")";
    return ss.str();
}

/**
 * Returns number of found errors
 */
int data::get_errors_num() {
    return this->errors_num;
}

/**
 * Allocates memory for variable
 */
long long data::alloc_mem() {
    this->mem_offset++;
    return this->mem_offset;
}

/**
 * Allocates memory for array, but returns begining addres of array in memory
 */
long long data::alloc_mem_array(long long start, long long end) {
    long long size = end - start + 2;
    this->mem_offset += size;
    return this->mem_offset - size + 1;
}

/**
 * Frees memory allocated for iterator
 */
void data::free_mem() {
    this->mem_offset--;
}

/**
 * Puts symbol into symbol table
 */
long long data::put_symbol(std::string name) {
    if(this->check_context(name)) {
        std::cerr << this->error_alert << name << " - is already defined" << this->put_line() << std::endl;
        this->error_found();
        return -1;
    }

    long long offset = this->alloc_mem();
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset);
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}


/**
 * Puts border symbol for FOR loop
 */ 
std::string data::put_border_symbol() {
    std::stringstream ss;

    ss << "!" << this->border_symbol;
    border_symbol++;

    this->put_symbol(ss.str());

    return ss.str();
}

/**
 * Puts array symbol into table
 */
long long data::put_symbol_array(std::string name, long long start, long long end) {
    if(this->check_context(name)) {
        std::cerr << this->error_alert << name << " - is already defined" << this->put_line() << std::endl;
        this->error_found();
        return -1;
    }

    if(end < start) {
        std::cerr << this->error_alert << "ending index can't be smaller than starting index" << this->put_line() << std::endl;
        this->error_found();
        return -1;
    }

    long long offset = this->alloc_mem_array(start, end);
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset, start, end);
    sym->is_init = true;
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}


/**
 * Puts valus which is iterator
 */
long long data::put_symbol_iterator(std::string name) {
    if(this->check_context(name)) {
        //Variable exists but is not iterator
        if(!this->get_symbol(name)->is_iterator) {
            std::cerr << this->error_alert << name << " - variable already defined and can't be local operator in for loop" << std::endl;
            return -1;
        } else {
            return this->get_symbol(name)->offset;
        }
    }

    long long offset = this->alloc_mem();
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset);
    sym->is_iterator = true;
    sym->is_init = true;
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}

void data::remove_iterator_symbol(std::string name) {
    this->sym_map.erase(name);
    this->free_mem();
}

/**
 * Puts variable which is only value
 */ 
long long data::put_value(long long value) {
    std::shared_ptr<symbol> sym;
    std::stringstream ss;
    std::string name;
    long long offset;

    ss << value;
    name = ss.str();

    if(this->check_context(name)) {
        return this->get_symbol(name)->offset;
    } 

    offset = this->alloc_mem();
    sym = std::make_shared<symbol>(name, offset);
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));

    return offset;
}

/**
 * Returns symbol (pointer)
 */
symbol *data::get_symbol(std::string name) {
    symbol *sym;

    if(!this->check_context(name)) {
        std::cerr << this->error_alert << name << " - variable is not declared" << this->put_line() << std::endl;
        this->error_found();
        return nullptr;
    }
    sym = this->sym_map[name].get();

    return sym;
}

/**
 * Returns variable's addres (single variable)
 */
variable *data::get_variable(std::string name) {
    std::shared_ptr<variable> var;
    symbol *sym = this->get_symbol(name);

    if(sym == nullptr) {
        return nullptr;
    } else if (sym->is_array) {
        std::cerr << this->error_alert << name << " - invalid use of array variable" << this->put_line() << std::endl;
        this->error_found();
        return nullptr;
    }

    var = std::make_shared<variable>(sym->offset);
    this->variables.push_back(var);
    return var.get();
}

/**
 * Returns array's cell's addres - version with variable
 */
variable *data::get_variable_array_var(std::string name, std::string var_name) {
    std::shared_ptr<variable> var;
    symbol *array_sym = this->get_symbol(name);
    symbol *var_sym = this->get_symbol(var_name);
    if(array_sym == nullptr || var_sym == nullptr) {
        return nullptr;
    } else if(var_sym->is_array) {
        std::cerr << this->error_alert << var_name << " - invalid use of array variable" << this->put_line() << std::endl;
        this->error_found();
        return nullptr;
    }

    /*if(!var_sym->is_init) {
        std::cerr << this->error_alert << name << " - variable is not initialized" << this->put_line() << std::endl;
        this->error_found();
        return nullptr;
    }*/
    
    var = std::make_shared<variable>(array_sym->offset, var_sym->offset);
    this->variables.push_back(var);
    return var.get();
}
    
/**
 * Returns array's cell's addres - version with value
 */
variable *data::get_variable_array_num(std::string name, long long num) {
    std::shared_ptr<variable> var;
    symbol *array_sym = this->get_symbol(name);

    if(array_sym == nullptr) {
        return nullptr;
    }

    long long real_addr = num + array_sym->offset - array_sym->array_start + 1;
    var = std::make_shared<variable>(real_addr);
    this->variables.push_back(var);
    return var.get();
}


/**
 * Returns constant variable
 */ 
variable *data::get_value_num(long long value) {
    std::shared_ptr<variable> val;
    long long offset;

    offset = this->put_value(value);
    val = std::make_shared<variable>(-1, offset, value);
    this->variables.push_back(val);
    return val.get();
}

/**
 * Syntactic sugar 
 */ 
variable *data::get_value(variable *var, std::string name) {
    std::shared_ptr<variable> val;

    if(var == nullptr) {
        //return nullptr; TODO: could make problem
        val = std::make_shared<variable>();
        this->variables.push_back(val);
        return val.get();
    }

    if(!this->sym_map[name]->is_init) {
        std::cerr << this->error_alert << name << " - variable is not initialized" << this->put_line() << std::endl;
        this->error_found();
    }
    
    val = std::make_shared<variable>(var->array_addr, var->addr);
    this->variables.push_back(val);
    return val.get();
}


/**
 * Returns new label struct
 */
label *data::get_label(long long go_to, long long jump_false) {
    return new label(go_to, jump_false);
}

for_label *data::get_for_label(std::string iterator_name, variable *start, variable *end) {
    variable *var = this->get_variable(iterator_name);
    variable *skip = this->get_value_num(1);
    return new for_label(var, start, end, skip);
}

std::shared_ptr<variable> data::nop() {
    std::shared_ptr<variable> var;
    var = std::make_shared<variable>();
    this->variables.push_back(var);
    return var;
}