#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <limits>

#include "code_generator.hpp"
#include "data.hpp"
#include "defs.hpp"

code_generator::code_generator(std::shared_ptr<data> d) {
    this->d = d;
    this->is_read = false;
    this->is_write = false;
    this->code_offset = -1;

    this->gen_costs();
    this->gen_reg_sym();
}

/**
 * Generates map with instructions costs
 */
void code_generator::gen_costs() {
    this->costs.insert(std::pair<enum instr, int>(I_INC, 1));
    this->costs.insert(std::pair<enum instr, int>(I_HALF, 1));
    this->costs.insert(std::pair<enum instr, int>(I_ADD, 5));
    this->costs.insert(std::pair<enum instr, int>(I_SUB, 5));
    this->costs.insert(std::pair<enum instr, int>(I_COPY, 5));
}

/**
 * Generates map with register std::string symbols
 */
void code_generator::gen_reg_sym() {
    //Stupid but works
    this->reg_sym.insert(std::pair<enum reg, std::string>(A, "A"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(B, "B"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(C, "C"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(D, "D"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(E, "E"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(F, "F"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(G, "G"));
    this->reg_sym.insert(std::pair<enum reg, std::string>(H, "H"));
}

//TODO: ADD INCREMENTING
/**
 * Increases offset when cmds added
 */ 
void code_generator::incr_offset(long long incr) {
    this->code_offset += incr;
}

/**
 * Returns generated cmds
 */
std::vector<std::string> code_generator::get_code() {
    return this->code;
}

/**
 * Writes HALT to the end of generated cmds
 */
void code_generator::end_prog() {
    this->code.push_back("HALT");
    this->incr_offset(1);
}

/**
 * Changes code on given index
 */
void code_generator::change_operation(long long index, std::string operation) {
    this->code[index] = operation;
}

/**
 * Returns operation on given index
 */ 
std::string code_generator::get_operation(long long index) {
    return this->code.at(index);
}

/**
 * Adds single operation to code
 */
void code_generator::add_operation(std::string operation) {
    this->code.push_back(operation);
    this->incr_offset(1);
}

/**
 * Puts array offset to memory in first array cell
 */
void code_generator::array_offset(long long addr, long long offset) {
    std::vector<std::string> cmds;
    cmds = this->gen_const(addr, A);
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
    cmds.clear();
    cmds = this->gen_const(offset, B);
    cmds.push_back("STORE B");
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}


/**
 * Copies value of one variable to another
 */
variable *code_generator::copy_variable(variable *var) {
    std::string name;
    variable *new_var;

    name = d->put_border_symbol();
    new_var = d->get_variable(name);

    //TODO: check this shit
    this->single_var(var, G);
    //this->code.push_back("PUT G");
    //this->incr_offset(1);
    this->reg_to_mem(G, new_var);

    return new_var;
}

/**
 * Sets A register to get address in memory
 */ 
void code_generator::set_mem_reg(variable *var) {
    std::vector<std::string> cmds = this->gen_const(var->addr, A);
    //Adress is nested: pidentifier(pidentifier)
    if(var->array_addr != -1) { 
        cmds.push_back("LOAD D");
        this->code.insert(this->code.end(), cmds.begin(), cmds.end());
        this->incr_offset(cmds.size());

        cmds.clear();
        cmds = this->gen_const(var->array_addr, A);
        cmds.push_back("LOAD E");
        cmds.push_back("SUB D E");
        cmds.push_back("ADD D A");
        cmds.push_back("INC D");
        cmds.push_back("COPY A D");
    }
    
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}

long long code_generator::get_code_offset() {
    return this->code_offset;
}

/**
 * Value from memory to register
 */ 
void code_generator::mem_to_reg(variable *var, enum reg r) {
    this->set_mem_reg(var);
    this->code.push_back("LOAD " + this->reg_sym[r]);
    this->incr_offset(1);
}

/**
 * Value from register to memory
 */
void code_generator::reg_to_mem(enum reg r, variable *var) {
    this->set_mem_reg(var);
    this->code.push_back("STORE "+ this->reg_sym[r]);
    this->incr_offset(1);
}


//TODO: This could generate problems (CONSTANTS!!!!!)
void code_generator::single_var(variable *var, enum reg r) {
    if(var->value != -1) {
        std::vector<std::string> cmds;
        cmds = this->gen_const(var->value, r);
        this->code.insert(this->code.end(), cmds.begin(), cmds.end());
        this->incr_offset(cmds.size());
    } else {
        this->mem_to_reg(var, r);
    }
}

/***** OPERATIONS *****/

void code_generator::constant(variable *var) {
    this->single_var(var, B);
}

/**
 * ADD two variables. Result in register B
 */
void code_generator::add(variable *v_1, variable *v_2) {
    this->single_var(v_1, B);
    this->single_var(v_2, C);
    this->code.push_back("ADD B C");
    this->incr_offset(1);
}

/**
 * SUB two variables. Result in register B
 */
void code_generator::sub(variable *v_1, variable *v_2) {
    this->single_var(v_1, B);
    this->single_var(v_2, C);
    this->code.push_back("SUB B C");
    this->incr_offset(1);
}

/**
 * MUL two variables. Result in register B
 */
void code_generator::mul(variable *v_1, variable *v_2) {
    std::vector<std::string> cmds;
    std::stringstream ss;

    long long shift = 0;

    this->single_var(v_1, B);
    this->single_var(v_2, C);

    cmds.push_back("SUB D D # mull begin"); //virtual: code_offset + 1

    shift = this->code_offset + 9 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO C " + ss.str());
    ss.str("");

    shift = this->code_offset + 4 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JODD C " + ss.str());
    ss.str("");

    cmds.push_back("ADD B B");
    cmds.push_back("HALF C");

    shift = this->code_offset  + 2; 
    ss << shift;
    cmds.push_back("JUMP " + ss.str());
    cmds.push_back("ADD D B");
    cmds.push_back("ADD B B");
    cmds.push_back("HALF C");
    cmds.push_back("JUMP " + ss.str());
    cmds.push_back("COPY B D");
    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}

/**
 * DIV two variables. Result in register B
 */
void code_generator::div(variable *v_1, variable *v_2) {
    std::vector<std::string> cmds;
    std::stringstream ss;

    long long shift = 0;

    this->single_var(v_1, B);
    this->single_var(v_2, C);

    cmds.push_back("SUB D D");
    cmds.push_back("SUB E E");
    cmds.push_back("INC E");
    cmds.push_back("COPY F B");

    shift = this->code_offset + 27 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO C " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("INC H");
    cmds.push_back("SUB H B");
 
    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 4 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("ADD C C");
    cmds.push_back("ADD E E");

    shift = this->code_offset + 6;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("SUB H F");

    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 3 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");


    cmds.push_back("SUB F C");
    cmds.push_back("ADD D E");
    cmds.push_back("HALF C");
    cmds.push_back("HALF E");

    shift = this->code_offset + 10 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO E " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("SUB H F");

    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 3 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("SUB F C");
    cmds.push_back("ADD D E");
    cmds.push_back("HALF C");
    cmds.push_back("HALF E");

    shift = this->code_offset + 22;
    ss << shift;
    cmds.push_back("JUMP " + ss.str());
    ss.str("");
    
    cmds.push_back("COPY B D");

    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}

/**
 * REM of dividing two variables. Result in register B
 */
void code_generator::mod(variable *v_1, variable *v_2) {
    std::vector<std::string> cmds;
    std::stringstream ss;

    long long shift = 0;

    this->single_var(v_1, B);
    this->single_var(v_2, C);

    cmds.push_back("SUB D D");
    cmds.push_back("SUB E E");
    cmds.push_back("INC E");
    cmds.push_back("COPY F B");

    shift = this->code_offset + 28 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO C " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("INC H");
    cmds.push_back("SUB H B");
 
    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 4 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("ADD C C");
    cmds.push_back("ADD E E");

    shift = this->code_offset + 6;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("SUB H F");

    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 3 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");


    cmds.push_back("SUB F C");
    cmds.push_back("ADD D E");
    cmds.push_back("HALF C");
    cmds.push_back("HALF E");

    shift = this->code_offset + 10 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO E " + ss.str()); 
    ss.str("");

    cmds.push_back("COPY H C");
    cmds.push_back("SUB H F");

    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JZERO H " + ss.str()); 
    ss.str("");

    shift = this->code_offset + 3 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str()); 
    ss.str("");

    cmds.push_back("SUB F C");
    cmds.push_back("ADD D E");
    cmds.push_back("HALF C");
    cmds.push_back("HALF E");

    shift = this->code_offset + 22;
    ss << shift;
    cmds.push_back("JUMP " + ss.str());
    ss.str("");
    
    shift = this->code_offset + 2 + cmds.size() + 1;
    ss << shift;
    cmds.push_back("JUMP " + ss.str());
    ss.str("");

    cmds.push_back("SUB F F");
    cmds.push_back("COPY B F");


    this->code.insert(this->code.end(), cmds.begin(), cmds.end());
    this->incr_offset(cmds.size());
}

/**
 * Adds 1 to given variable
 */
void code_generator::inc(variable *var) {
    this->single_var(var, B);
    this->code.push_back("INC " + this->reg_sym[B]);
}


/**
 * Subs 1 from given variable
 */
void code_generator::dec(variable *var) {
    this->single_var(var, B);
    this->code.push_back("DEC " + this->reg_sym[B]);
}

/**
 * Assigns value to variable
 */
void code_generator::assign(variable *var) {
    this->reg_to_mem(B, var);
}


/**
 * Puts code to handle READ
 */
void code_generator::read(variable *var) {
    this->code.push_back("GET B");
    this->reg_to_mem(B, var);
    this->incr_offset(1);
}

/**
 * Puts code to handle WRITE
 */
void code_generator::write(variable *var) {
    this->single_var(var, B);
    this->code.push_back("PUT " + this->reg_sym[B]);
    this->incr_offset(1);
}


//CONDITIONS

/**
 * Checks if two variables are EQUAL
 */
cond_label *code_generator::eq(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    cond_start = this->code_offset + 1;
    this->single_var(v_1, G);
    this->single_var(v_2, H);
    this->code.push_back("INC H");
    this->code.push_back("SUB H G");

    shift = this->code_offset + 6;
    ss << shift;
    this->code.push_back("JZERO H "+ ss.str());
    ss.str("");

    this->code.push_back("DEC H");
    this->incr_offset(4);

    //jump if true
    shift++;
    ss << shift;
    this->code.push_back("JZERO H " + ss.str());

    //jump if false
    this->code.push_back("JUMP addr");
    this->incr_offset(2);
    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}

/**
 * Checks if two variables are NOT EQUAL
 */
cond_label *code_generator::neq(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    cond_start = this->code_offset + 1;
    this->single_var(v_1, G);
    this->single_var(v_2, H);
    this->code.push_back("INC H");
    this->code.push_back("SUB H G");

    //jump if true
    shift = this->code_offset + 6;
    ss << shift;
    this->code.push_back("JZERO H "+ ss.str());
    ss.str("");

    this->code.push_back("DEC H");
    this->incr_offset(4);

    //jump if false
    this->code.push_back("JZERO H addr");

    this->incr_offset(1);
    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}

/**
 * Checks if v_1 is GREATER than v_2
 */
cond_label *code_generator::gt(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    cond_start = this->code_offset + 1;
    this->single_var(v_1, G);
    this->single_var(v_2, H);

    this->code.push_back("INC H");
    this->code.push_back("SUB H G");

    //Jump if true
    shift = this->code_offset + 5;
    ss << shift;
    this->code.push_back("JZERO H " + ss.str());
    this->incr_offset(3);

    //Jump if false
    this->code.push_back("JUMP addr");
    this->incr_offset(1);

    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}


/**
 * Checks if v_1 is LESS than v_2
 */
cond_label *code_generator::lt(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    //Shift ->GREATER
    cond_start = this->code_offset + 1;
    this->single_var(v_2, G);
    this->single_var(v_1, H);

    this->code.push_back("INC H");
    this->code.push_back("SUB H G");

    //Jump if true
    shift = this->code_offset + 5 ;
    ss << shift;
    this->code.push_back("JZERO H " + ss.str());
    this->incr_offset(3);

    //Jump if false
    this->code.push_back("JUMP addr");
    this->incr_offset(1);

    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}

/**
 * Checks if v_1 is GREATER OR EQUAL than v_2
 */
cond_label *code_generator::geq(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    //Shift ->GREATER
    cond_start = this->code_offset + 1;
    this->single_var(v_1, G);
    this->single_var(v_2, H);

    this->code.push_back("SUB H G");

    //Jump if true
    shift = this->code_offset + 4;
    ss << shift;
    this->code.push_back("JZERO H "  + ss.str());
    this->incr_offset(2);

    //Jump if false
    this->code.push_back("JUMP addr");
    this->incr_offset(1);

    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}

/**
 * Checks if v_1 is LESS OR EQUAL than v_2
 */
cond_label *code_generator::leq(variable *v_1, variable *v_2) {
    std::shared_ptr<cond_label> cond;
    long long addr;
    long long cond_start;
    long long shift;
    std::stringstream ss;

    //Shift ->GREATER
    cond_start = this->code_offset + 1;
    this->single_var(v_2, G);
    this->single_var(v_1, H);

    this->code.push_back("SUB H G");

    //Jump if true
    shift = this->code_offset + 4;
    ss << shift;
    this->code.push_back("JZERO H "  + ss.str());
    this->incr_offset(2);

    //Jump if false
    this->code.push_back("JUMP addr");
    this->incr_offset(1);

    addr = this->code_offset;

    cond = std::make_shared<cond_label>(cond_start, addr);
    this->conditions.push_back(cond);
    return cond.get();
}

/**
 * Generates constant value
 */
std::vector<std::string> code_generator::gen_const(long long c, enum reg r) {
    std::vector<std::string> cmds;
    std::string sub_cmd = "SUB " + this->reg_sym[r] + " " + this->reg_sym[r];
    std::string inc_cmd = "INC " + this->reg_sym[r];
    std::string add_cmd = "ADD " + this->reg_sym[r] + " " + this->reg_sym[r];
    
    bool is_large = false;
    long long inc_cost = std::numeric_limits<long long>::max();
    long long add_cost = 0;

    //Set 0 to given register r
    cmds.push_back(sub_cmd);
    add_cost += this->costs[I_SUB];

    //Constant is 0 or 1
    if(c == 0) {
        return cmds;
    } else if(c == 1) {
        cmds.push_back(inc_cmd);
        return cmds;
    }

    if(c > 200) {
        is_large = true;
    }

    //Init given register with 2
    cmds.push_back(inc_cmd);
    cmds.push_back(inc_cmd);
    add_cost += 2 * this->costs[I_INC];

    //Value in given register
    long long tmp = 2;

    while(tmp + tmp <= c) {
        tmp += tmp;
        cmds.push_back(add_cmd);
        add_cost += this->costs[I_ADD];
    }

    cmds.push_back("SUB " + this->reg_sym[F] + " " + this->reg_sym[F]);
    add_cost += this->costs[I_SUB];

    cmds.push_back("COPY " + this->reg_sym[F] + " " + this->reg_sym[r]);
    add_cost += this->costs[I_COPY];

    cmds.push_back("HALF " + this->reg_sym[F]);
    add_cost += this->costs[I_HALF];

    long long half = tmp / 2;

    while(half != 2) {
        if(tmp + half <= c) {
            tmp += half;
            cmds.push_back("ADD " + this->reg_sym[r] + " " + this->reg_sym[F]);
            add_cost += this->costs[I_ADD];
        }

        if(tmp == c) {
            break;
        }

        cmds.push_back("HALF " + this->reg_sym[F]);
        add_cost += this->costs[I_HALF];
        half /= 2;
    }

    while(tmp != c) {
        cmds.push_back(inc_cmd);
        add_cost += this->costs[I_INC];
        tmp++;
    }

    if(!is_large) {
        inc_cost = c * this->costs[I_INC] + this->costs[I_SUB];
        if(inc_cost < add_cost) {
            cmds.clear();
            cmds.push_back(sub_cmd);

            for(int i = 0; i < c; i++) {
                cmds.push_back(inc_cmd);
            }
        }
    }

    return cmds;
}

//IF, IF_ELSE blocks

/**
 * Geneartes IF block
 */
void code_generator::if_block(long long go_to) {
    std::stringstream ss;
    std::string op;

    op = this->get_operation(go_to);
    ss << (this->code_offset + 1);

    //Replace 'addr' phrase
    op.replace(op.end() - 4, op.end(), ss.str());
    this->change_operation(go_to, op);
}

void code_generator::if_else_block_first(label *lab, long long go_to) {
    this->code.push_back("JUMP addr");
    this->incr_offset(1);
    lab->go_to = this->code_offset;

    this->if_block(go_to);
}

void code_generator::if_else_block_second(long long go_to) {
    this->if_block(go_to);
}

//WHILE, DO_WHILE blocks

/**
 * Jump true address for DO_WHILE
 */
void code_generator::do_while_block_first(label *lab) {
    lab->go_to = this->code_offset + 1;
}

void code_generator::do_while_block_second(label *lab, long long go_to) {
    std::stringstream ss;

    ss << lab->go_to;
    this->code.push_back("JUMP " + ss.str());
    ss.str("");
    this->incr_offset(1);

    this->if_block(go_to);
}

void code_generator::while_block(cond_label *cond) {
    std::stringstream ss;

    ss << cond->start;
    this->code.push_back("JUMP " + ss.str());
    this->incr_offset(1);

    this->if_block(cond->go_to);
}

/****FOR_TO***/

void code_generator::for_to_block_first(for_label *label) {
    label->start = this->copy_variable(label->start);
    label->end = this->copy_variable(label->end);

    this->constant(label->start);
    this->assign(label->iterator);
    label->cond = this->leq(label->iterator, label->end);
}

void code_generator::for_to_block_second(for_label *label) {
    std::stringstream ss;

    this->inc(label->iterator);
    this->reg_to_mem(B, label->iterator);

    ss << label->cond->start;
    this->code.push_back("JUMP " + ss.str());
    this->incr_offset(1);

    this->if_block(label->cond->go_to);
}

void code_generator::for_downto_block_first(for_label *label) {
    label->start = this->copy_variable(label->start);
    label->end = this->copy_variable(label->end);

    this->constant(label->start);
    this->assign(label->iterator);
    label->cond = this->geq(label->iterator, label->end);
}


void code_generator::for_downto_block_second(for_label *label) {
    std::stringstream ss;
    long long shift;
    long long go_to; 

    this->single_var(label->iterator, G);
    this->code.push_back("JZERO G addr");
    this->incr_offset(1);
    go_to = this->code_offset;

    this->dec(label->iterator);
    this->reg_to_mem(B, label->iterator);

    shift = this->code_offset + 3;
    ss << shift;
    
    ss.str("");

    ss << label->cond->start;
    this->code.push_back("JUMP " + ss.str());
    this->incr_offset(1);

    this->if_block(go_to);

    this->if_block(label->cond->go_to);
}


