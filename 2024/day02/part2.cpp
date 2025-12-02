
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;



bool isSafe(Vector<int> report) {
    bool    is_inc;
    int     diff;
    size_t  i;

    if (report.size() <= 2)
        return true;
    is_inc = (report[0] < report[1]);
    for (i = 0; i < report.size() - 1; i++) {
        diff = std::abs(report[i] - report[i + 1]);
        if (diff < 1 or diff > 3)
            return false;
        if (is_inc and report[i] > report[i + 1])
            return false;
        if (not is_inc and report[i] < report[i + 1])
            return false;
    }
    return true;
}

// 634
int main(void) {
    std::ifstream   file("part2.txt");
    String          line, back;
    Vector<int>     report, tmp;
    long            count;

    if (!file.is_open()) {
        std::cout << "File not open." << std::endl;
        return (1);
    }

    count = 0;
    while (file and !file.eof()) {
        line.clear();
        std::getline(file, line);
        if (line.empty())
            break;
        report.clear();

        back = line;
        while (!line.empty()) {
            line = line.substr(line.find_first_not_of(' '));
            report.push_back(std::stoi(line));
            if (line.find_first_of(' ') == String::npos)
                break;
            line = line.substr(line.find_first_of(' '));
        }

        for (size_t i = 0; i < report.size(); i++) {
            tmp = Vector<int>(report);
            tmp.erase(tmp.begin() + i);
            if (not isSafe(tmp))
                continue;
            count++;
            break;
        }
    }

    std::cout << "The safe reports are " << count << std::endl;
    return (0);
}
