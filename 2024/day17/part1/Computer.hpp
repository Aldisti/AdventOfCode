
#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>


using String = std::string;

template <typename T>
using Vector = std::vector<T>;

using StringVector = Vector<String>;



class Computer {
    using Instruction = void (Computer::*)(const char &);

    private:
        int registers[3] = {0, 0, 0};
        Vector<Instruction> instructions = Vector<Instruction>({
            &Computer::adv,
            &Computer::bxl,
            &Computer::bst,
            &Computer::jnz,
            &Computer::bxc,
            &Computer::out,
            &Computer::bdv,
            &Computer::cdv,
        });

        int &reg_a = registers[0];
        int &reg_b = registers[1];
        int &reg_c = registers[2];

        Vector<char> program;
        String stdout;
        u_int inst_ptr; // instruction pointer
        bool _jumped;

    public:
        // Constructors
        Computer();
        ~Computer();
        Computer(const Computer &c);

        // Operators
        Computer &operator =(const Computer &c);

        // Getters
        String get_stdout() const;
        int get_reg(const char c) const;
        int get_reg(const int i) const;
        int get_inst_ptr() const;
        char get_curr_inst() const;
        // Setters
        void set_reg(const char c, int value);
        void set_reg(const int i, int value);

        // Methods
        void reset();
        void load_program(const String &program);
        void execute();

    private:
        int get_combo_op(const char &c) const;
        // Instructions
        void    adv(const char &op);
        void    bxl(const char &op);
        void    bst(const char &op);
        void    jnz(const char &op);
        void    bxc(const char &op);
        void    out(const char &op);
        void    bdv(const char &op);
        void    cdv(const char &op);

};

std::ostream &operator <<(std::ostream &out, const Computer &c);

struct ComputerException: std::runtime_error {
    using std::runtime_error::runtime_error;
};
