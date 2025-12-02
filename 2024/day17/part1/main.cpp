
#include "Computer.hpp"
#include <sstream>
#include <fstream>



String  readFile(String filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

StringVector    split(String str, String separators, bool removeSeparator=true) {
    StringVector    strs;
    String          tmp;
    size_t          i = 0, j = 0;

    while (i < str.size()) {
        if (separators.find(str[i]) == String::npos) {
            i++;
            continue;
        }
        tmp = str.substr(j, i - j + !removeSeparator);
        if (!removeSeparator or not tmp.empty())
            strs.push_back(tmp);
        j = ++i;
    }
    if (j < i) {
        if (!removeSeparator or not tmp.empty())
            strs.push_back(str.substr(j, i - j + !removeSeparator));
    }
    return strs;
}

void test1() {
    String test = "1";
    Computer computer;
    try {
        computer.set_reg('c', 9);
        computer.load_program("2,6");
        computer.execute();
        if (computer.get_reg('b') == 1) {
            std::cout << "[OK] TEST " << test << std::endl;
            return;
        }
    } catch (const ComputerException &ce) {
        std::cout << std::endl << ce.what() << std::endl;
    }
    std::cout << "[KO] TEST " << test << std::endl;
}

void test2() {
    String test = "2";
    Computer computer;
    try {
        computer.set_reg('a', 10);
        computer.load_program("5,0,5,1,5,4");
        computer.execute();
        if (computer.get_stdout() == "0,1,2") {
            std::cout << "[OK] TEST " << test << std::endl;
            return;
        }
    } catch (const ComputerException &ce) {
        std::cout << std::endl << ce.what() << std::endl;
    }
    std::cout << "[KO] TEST " << test << std::endl;
}

void test3() {
    String test = "3";
    Computer computer;
    try {
        computer.set_reg('a', 2024);
        computer.load_program("0,1,5,4,3,0");
        computer.execute();
        if (computer.get_stdout() == "4,2,5,6,7,7,7,7,3,1,0" and computer.get_reg('a') == 0) {
            std::cout << "[OK] TEST " << test << std::endl;
            return;
        }
    } catch (const ComputerException &ce) {
        std::cout << std::endl << ce.what() << std::endl;
    }
    std::cout << "[KO] TEST " << test << std::endl;
}

// 2,7,6,5,6,0,2,3,1
int main(void) {
    String text = readFile("input.txt");
    StringVector lines = split(text, "\n");
    Computer computer;

    for (auto line : lines) {
        if (line.find("Register A") != String::npos)
            computer.set_reg('a', std::stoi(line.substr(line.find(":") + 1)));
        else if (line.find("Register B") != String::npos)
            computer.set_reg('b', std::stoi(line.substr(line.find(":") + 1)));
        else if (line.find("Register C") != String::npos)
            computer.set_reg('c', std::stoi(line.substr(line.find(":") + 1)));
        else if (line.find("Program") != String::npos)
            computer.load_program(line.substr(line.find(":") + 2));
    }
    computer.execute();

    std::cout << "The output is " << computer.get_stdout() << std::endl;
    return (0);
}
