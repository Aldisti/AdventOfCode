
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// 598
int main(void) {
    std::ifstream       file("part1.txt");
    std::string         line;
    std::vector<int>    report;
    int                 diff;
    long                count;

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

        while (!line.empty()) {
            line = line.substr(line.find_first_not_of(' '));
            report.push_back(std::stoi(line));
            if (line.find_first_of(' ') == std::string::npos)
                break;
            line = line.substr(line.find_first_of(' '));
        }

        for (size_t i = 0; i < report.size() - 1; i++) {
            diff = std::abs(report[i] - report[i + 1]);
            if (diff > 3 or diff < 1) {
                count--;
                break;
            }
            if (i == 0)
                continue;
            if (report[i - 1] < report[i] and report[i] > report[i + 1]) {
                count--;
                break;
            }
            if (report[i - 1] > report[i] and report[i] < report[i + 1]) {
                count--;
                break;
            }
        }
        count++;
    }

    std::cout << "The safe reports are " << count << std::endl;
    return (0);
}
