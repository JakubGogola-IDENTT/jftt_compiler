#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>

void gen_const(long long constant) {
    std::map<std::string, int> instr;
    std::vector<std::string> output_code;
    bool is_large = false;

    instr.insert(std::pair<std::string, int>("INC", 1));
    instr.insert(std::pair<std::string, int>("ADD", 5));
    instr.insert(std::pair<std::string, int>("HALF", 1));
    instr.insert(std::pair<std::string, int>("SUB", 5));   
    instr.insert(std::pair<std::string, int>("COPY", 5));
    instr.insert(std::pair<std::string, int>("PUT", 100));
    instr.insert(std::pair<std::string, int>("GET", 100));

    if(constant < 0) {
        std::cerr << "You're an idiot!" << std::endl;
        exit(0);
    }

    if(constant == 0) {
        output_code.push_back("SUB B B");
        output_code.push_back("PUT B");
        output_code.push_back("HALT");
    } else if (constant == 1) {
        output_code.push_back("SUB B B");
        output_code.push_back("INC B");
        output_code.push_back("PUT B");
        output_code.push_back("HALT");
    }

    if(constant > 2000) {
        is_large = true;
    }

    std::vector<std::string> code_inc;
    int cost_inc = 0;
    code_inc.push_back("SUB B B");
    cost_inc += instr["SUB"];

    if(!is_large) {
        for(long long i = 0; i < constant; i++) {
            code_inc.push_back("INC B");
            cost_inc += instr["INC"];
        }
    }

    code_inc.push_back("PUT B");
    code_inc.push_back("HALT");
    cost_inc += instr["PUT"];

    std::cout << "Inc finished" << std::endl;

    std::vector<std::string> code_mixed;
    int cost_mixed = 0;

    code_mixed.push_back("SUB B B");
    cost_mixed += instr["SUB"];

    code_mixed.push_back("INC B");
    code_mixed.push_back("INC B");
    cost_mixed += 2* instr["INC"];

    long long tmp = 2;

    while(tmp + tmp <= constant) {
        tmp += tmp;
        code_mixed.push_back("ADD B B");
        cost_mixed += instr["ADD"];
    }

    code_mixed.push_back("SUB C C");
    cost_mixed += instr["SUB"];
    code_mixed.push_back("COPY C B");
    cost_mixed += instr["COPY"];
    code_mixed.push_back("HALF C");
    cost_mixed += instr["HALF"];

    long long half = tmp / 2;

    while(half != 2) {
        if(tmp + half <= constant) {
            tmp += half;
            code_mixed.push_back("ADD B C");
            cost_mixed += instr["ADD"];
        }

        if(tmp == constant) {
            break;
        }

        code_mixed.push_back("HALF C");
        cost_mixed += instr["HALF"];
        half /= 2;
    }

    while(tmp != constant) {
        code_mixed.push_back("INC B");
        cost_mixed += instr["INC"];
        tmp++;
    }

    code_mixed.push_back("PUT B");
    cost_mixed += instr["PUT"];
    code_mixed.push_back("HALT");

    if(cost_inc <= cost_mixed) {
        output_code = code_inc;
        std::cout << "Incrementing is faster" << std::endl;
    } else {
        output_code = code_mixed;
        std::cout << "Adding and incrementing is faster" << std::endl;
    }

    if(is_large) {
        output_code = code_mixed;
    }

    std::cout << "******* COSTS *******" << std::endl;
    std::cout << "Incrementing: " << cost_inc << std::endl;
    std::cout << "Adding and incrementing: " << cost_mixed << std::endl;
    std::cout << "*********************" << std::endl;
    std::ofstream fout("cpp_const.mr", std::ofstream::out);

    for(long long i = 0; i < output_code.size(); i++) {
        fout << output_code[i] << std::endl;
    }

    fout.close();
}

int main(int argc, char const *argv[])
{
    long long constant;
    std::cin >> constant;
    gen_const(constant);
    return 0;
}
