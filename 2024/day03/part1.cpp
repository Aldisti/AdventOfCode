
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

// 174336360
int main(void) {
    std::regex          regex(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::ifstream       file("part1.txt");
    std::stringstream   buffer;
    std::string         text;
    std::smatch         matches;
    long                sum = 0;

    if (!file.is_open()) {
        std::cout << "File not open." << std::endl;
        return (1);
    }

    buffer << file.rdbuf();
    text = buffer.str();

    while (std::regex_search(text, matches, regex)) {
        sum += std::stoi(matches[1]) * std::stoi(matches[2]);
        text = matches.suffix();
    }

    std::cout << "The safe reports are " << sum << std::endl;
    return (0);
}
