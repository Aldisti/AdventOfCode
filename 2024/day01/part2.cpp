
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

int main(void) {
    std::ifstream       file("part1.txt");
    std::string         line;
    std::vector<int>    left;
    std::map<int,int>   right;
    int                 tmp;
    long                sum;

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
        left.push_back(std::stoi(line));
        // skip the first number
        line = line.substr(line.find_first_of(' '));
        // add number to map (if a key is not in a map, then its value will be 0)
        right[std::stoi(line)]++;
    }

    sum = 0;
    for (const int &i : left) {
        sum += i * right[i];
    }

    std::cout << "The similarity score between the two lists is " << sum << std::endl;

    return (0);
}
