
#include "Computer.hpp"

std::ostream &operator <<(std::ostream &out, const Computer &c) {
    out << "inst_ptr: " << c.get_inst_ptr()
    << " op: " << (char) c.get_curr_inst() << std::endl
    << "A: " << c.get_reg('a') << std::endl
    << "B: " << c.get_reg('b') << std::endl
    << "C: " << c.get_reg('c') << std::endl
    << "out: " << c.get_stdout() << std::endl;
	return (out);
}

Computer::Computer() {
    reset();
    return;
}

Computer::~Computer() {
    reset();
    return;
}

Computer::Computer(const Computer &c) {
    *this = c;
    return;
}

Computer &Computer::operator =(const Computer &c) {
    if (this == &c)
        return *this;
    reg_a = c.reg_a;
    reg_b = c.reg_b;
    reg_c = c.reg_c;
    inst_ptr = c.inst_ptr;
    _jumped = c._jumped;
    program = c.program;
    stdout = c.stdout;
    return *this;
}

String Computer::get_stdout() const {
    return stdout;
}

int Computer::get_reg(const char c) const {
    if (c > 'c' or c - 'a' < 0)
        throw new ComputerException("Invalid register");
    return registers[c - 'a'];
}

int Computer::get_reg(const int i) const {
    if (i < 0 or i > 2)
        throw new ComputerException("Invalid register index");
    return registers[i];
}

int Computer::get_inst_ptr() const {
    return inst_ptr;
}

char Computer::get_curr_inst() const {
    return program[inst_ptr] + 48;
}

void Computer::set_reg(const char c, int value) {
    if (c > 'c' or c - 'a' < 0)
        throw new ComputerException("Invalid register");
    registers[c - 'a'] = value;
}

void Computer::set_reg(const int i, int value) {
    if (i < 0 or i > 2)
        throw new ComputerException("Invalid register index");
    registers[i] = value;
}

void Computer::reset() {
    reg_a = 0;
    reg_b = 0;
    reg_c = 0;

    inst_ptr = 0;
    _jumped = false;

    program.clear();
    stdout.clear();
}

void Computer::load_program(const String &program) {
    Vector<char> prg;

    for (u_int i = 0; i < program.size(); i++) {
        char c = program[i];
        if (i % 2 == 1 and c != ',')
            throw new ComputerException("No comma at odd index");
        else if (i % 2 == 1)
            continue;
        else if (c < 48 or c - 48 > 7)
            throw new ComputerException("Invalid operand");
        prg.push_back(c - 48);
    }
    this->program = prg;
    return;
}

void Computer::execute() {
    if (program.size() == 0)
        throw new ComputerException("Cannot execute without program");

    while (inst_ptr < program.size()) {
        Instruction inst = instructions[program[inst_ptr]];
        (this->*inst)(program[inst_ptr + 1]);

        if (_jumped)
            _jumped = false;
        else
            inst_ptr += 2;
    }
}

int Computer::get_combo_op(const char &c) const {
    if (c < 0 or c >= 7)
        throw new ComputerException("Received invalid op");
    if (c < 4)
        return (int) c;
    return registers[c - 4];
}

void    Computer::adv(const char &op) {
    reg_a /= (1 << get_combo_op(op));
}

void    Computer::bxl(const char &op) {
    reg_b ^= (int) op;
}

void    Computer::bst(const char &op) {
    reg_b = get_combo_op(op) % 8;
}

void    Computer::jnz(const char &op) {
    if (reg_a == 0)
        return;
    _jumped = true;
    inst_ptr = (int) op;
}

void    Computer::bxc(const char &op) {
    (void) op;
    reg_b ^= reg_c;
}

void    Computer::out(const char &op) {
    if (stdout.size() > 0)
        stdout += ",";
    stdout += (char) ((get_combo_op(op) % 8) + 48);
}

void    Computer::bdv(const char &op) {
    reg_b = reg_a / (1 << get_combo_op(op));
}

void    Computer::cdv(const char &op) {
    reg_c = reg_a / (1 << get_combo_op(op));
}

