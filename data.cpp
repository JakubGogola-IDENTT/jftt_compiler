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
    //this->code_offset = 0;
    //this->next_label = 0;
    this->errors = false;
    this->errors_num = 0;
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
        this->sym_map[name]->is_init = true;
    } else {
        std::cerr << this->error_alert << name << " - variable is not defined" << std::endl;
        this->error_found();
    }
    //TODO: finish!!!!
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
    return this->mem_offset - (size - 1);
}

/**
 * Puts symbol into symbol table
 */
long long data::put_symbol(std::string name) {
    if(this->check_context(name)) {
        std::cerr << this->error_alert << name << " - is already defined" << std::endl;
        this->error_found();
        return -1;
    }

    long long offset = this->alloc_mem();
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset);
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
}

/**
 * Puts array symbol into table
 */
long long data::put_symbol_array(std::string name, long long start, long long end) {
    if(this->check_context(name)) {
        std::cerr << this->error_alert << name << " - is already defined" << std::endl;
        this->error_found();
        return -1;
    }

    if(end < start) {
        std::cerr << this->error_alert << "ending index can't be smaller than starting index" << std::endl;
        this->error_found();
    }

    long long offset = this->alloc_mem_array(start, end);
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset, start, end);
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
            std::cerr << this->error_alert << name << " - can't be local operator in for loop" << std::endl;
            return -1;
        } else {
            return this->get_symbol(name)->offset;
        }
    }

    long long offset = this->alloc_mem();
    std::shared_ptr<symbol> sym = std::make_shared<symbol>(name, offset);
    sym->is_iterator = true;
    this->sym_map.insert(std::pair<std::string, std::shared_ptr<symbol>>(name, sym));
    return offset;
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
        std::cerr << this->error_alert << name << " - variable is not declared" << std::endl;
        this->error_found();
        return nullptr;
    }
    //TODO: CHECK IF IT WORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (if not - make pointer)
    sym = this->sym_map[name].get();

    //TODO: UNECESSARRY CODE!!!!! NOT HERE
    /*if(!sym->is_array && !sym->is_init) {
        std::cerr << this->error_alert << name << " - variable is not initialized" << std::endl;
        this->error_found();
        return nullptr;
    }*/

    //TODO: CHECK!!!!!!!!!!!!!!!
    //return this->sym_map[name].get();
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
    }

    var = std::make_shared<variable>(sym->offset);
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
    }

    if(!var_sym->is_init) {
        std::cerr << this->error_alert << name << " - variable is not initialized" << std::endl;
        this->error_found();
        return nullptr;
    }

    var = std::make_shared<variable>(array_sym->offset, var_sym->offset);
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

    long long real_addr = num - array_sym->offset + 1;
    var = std::make_shared<variable>(real_addr);
    return var.get();
}


/**
 * Returns constant variable
 */ 
variable *data::get_value_num(long long value) {
    std::shared_ptr<variable> val;
    long long offset;

    offset = this->put_value(value);
    val = std::make_shared<variable>(offset);

    return val.get();
}

/**
 * Syntactic sugar 
 */ 
variable *data::get_value(variable *var) {
    return var;
}