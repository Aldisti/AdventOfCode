
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
    return String(str.rbegin(), str.rend());
}

size_t countHorizontal(StringVector strs, String needle) {
    String  reverseNeedle = reverse(needle);
    size_t  count, i;

    count = 0;
    for (const String & row : strs) {
        for (i = 0; i < row.size() - needle.size() + 1; i++) {
            if (row.compare(i, needle.size(), needle) == 0 or row.compare(i, reverseNeedle.size(), reverseNeedle) == 0) {
                count++;
                i++;
            }
        }
    }
    return count;
}

size_t  countVertical(StringVector strs, String needle) {
    String  reverseNeedle = reverse(needle), tmp;
    size_t  count, row, col, i;

    count = 0;
    for (row = 0; row < strs.size(); row++) {
        for (col = 0; col < strs[row].size(); col++) {
            if (strs[row][col] != needle.front() and strs[row][col] != needle.back())
                continue;
            i = 0;
            tmp = (strs[row][col] == needle.front()) ? needle : reverseNeedle;
            while (i < tmp.size() and row + i < strs.size() and strs[row + i][col] == tmp[i])
                i++;
            count += (i == tmp.size());
        }
    }
    return count;
}

size_t  countDiagonal(StringVector strs, String needle) {
    String  reverseNeedle = reverse(needle), tmp;
    size_t  count, row, col, i;

    count = 0;
    for (row = 0; row < strs.size(); row++) {
        for (col = 0; col < strs[row].size(); col++) {
            if (strs[row][col] != needle.front() and strs[row][col] != needle.back())
                continue;
            tmp = (strs[row][col] == needle.front()) ? needle : reverseNeedle;
            // check bottom left
            i = 0;
            while (i < tmp.size()
                    and row + i < strs.size() and col >= i
                    and strs[row + i][col - i] == tmp[i]) {
                i++;
            }
            count += (i == tmp.size());
            // check bottom right
            i = 0;
            while (i < tmp.size() and row + i < strs.size()
                    and col + i < strs[row + i].size()
                    and strs[row + i][col + i] == tmp[i]) {
                i++;
            }
            count += i == tmp.size();
        }
    }
    return count;
}

// 2567
int main(void) {
    std::string     text = readFile("part1.txt");
    size_t          count = 0;

    StringVector strs = split(text, " \n\t");

    count = countHorizontal(strs, "XMAS");
    // std::cout << "Horizontal " << count << std::endl;
    count += countVertical(strs, "XMAS");
    // std::cout << "Vertical " << count << std::endl;
    count += countDiagonal(strs, "XMAS");
    // std::cout << "Diagonal " << count << std::endl;

    std::cout << "The number of times XMAS appears is " << count << std::endl;
    return (0);
}
