
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::set<T>;

template <typename T>
using Pair = std::pair<T,T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;




template <typename T>
void    printVector(Vector<T> vect) {
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

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

LongVector   mapToLong(StringVector arr) {
    LongVector   nums;

    for (const String & num : arr) {
         nums.push_back(std::stoi(num));
    }
    return nums;
}

void    blink(LongVector & stones) {
    String  num;
    size_t  i;

    for (i = 0; i < stones.size(); i++) {
        if (stones[i] == 0) {
            stones[i] = 1;
            continue;
        }
        num = String(std::to_string(stones[i]));
        if (num.size() % 2 != 0) {
            stones[i] *= 2024;
            continue;
        }

        stones[i] = std::stoi(num.substr(num.size() / 2));
        stones.insert(stones.begin() + i, std::stoi(num.substr(0, num.size() / 2)));
        i++;
    }
}

// 224529
int main(void) {
    String      text = readFile("part1.txt");
    LongVector   stones = mapToLong(split(text, " "));

    for (size_t iterations = 25; iterations > 0; iterations--)
        blink(stones);

    std::cout << "The total number of stones is " << stones.size() << std::endl;
    return (0);
}
