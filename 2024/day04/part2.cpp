
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using String = std::string;

using StringVector = std::vector<String>;

String readFile(String filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

StringVector split(String str, String separators, bool removeSeparator=true) {
    std::vector<String> strs;
    size_t              i = 0, j = 0;

    while (i < str.size()) {
        if (separators.find(str[i]) == String::npos) {
            i++;
            continue;
        }
        strs.push_back(str.substr(j, i - j + !removeSeparator));
        j = ++i;
    }
    if (j < i)
        strs.push_back(str.substr(j, i - j + !removeSeparator));
    return strs;
}

String reverse(String str) {
    return str.substr(0, 1).append(String(str.rbegin(), str.rend() - 1));
}

size_t  count(StringVector strs) {
    String  d1("  "), d2("  ");
    size_t  row, col, count;

    count = 0;
    for (row = 1; row < strs.size() - 1; row++) {
        for (col = 1; col < strs[row].size() - 1; col++) {
            if (strs[row][col] != 'A')
                continue;

            d1[0] = strs[row - 1][col - 1]; // top left
            d1[1] = strs[row + 1][col + 1]; // bottom right
            d2[0] = strs[row - 1][col + 1]; // top right
            d2[1] = strs[row + 1][col - 1]; // bottom left

            if ((d1 == "MS" or d1 == "SM") and (d2 == "MS" or d2 == "SM"))
                count++;
        }
    }
    return count;
}

// 2029
int main(void) {
    String text = readFile("part1.txt");

    StringVector strs = split(text, " \n\t");

    size_t ret = count(strs);

    std::cout << "The number of times X-MAS appears is " << ret << std::endl;
    return (0);
}
