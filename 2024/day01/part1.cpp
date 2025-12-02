
#include <iostream>
#include <fstream>
#include <string>

#include "BIVector.hpp"

int main(void) {
    std::ifstream   file("part1.txt");
    std::string     line;
    BIVector<int>   left, right;
    long            sum;

    if (!file.is_open()) {
        std::cout << "File not open." << std::endl;
        return (1);
    }

    while (file and !file.eof()) {
        line.clear();
        std::getline(file, line);
        if (line.empty())
            break;
        // skip all the starting spaces
        line = line.substr(line.find_first_not_of(' '));
        left.insert(std::stoi(line));
        // skip the first number
        line = line.substr(line.find_first_of(' '));
        right.insert(std::stoi(line));
    }

    sum = 0;
    while (left.size() > 0 and right.size() > 0) {
        sum += std::abs(left.pop(0) - right.pop(0));
    }

    std::cout << "The total distance between the two lists is " << sum << std::endl;
    return (0);
}
