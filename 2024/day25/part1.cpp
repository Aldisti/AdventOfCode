
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <cmath>
#include <set>
#include <map>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::set<T>;

template <typename T, typename U>
using Map = std::map<T, U>;

template <typename T>
using Pair = std::pair<T,T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;
using IntVector = Vector<int>;
using UIntVector = Vector<u_int>;




template <typename T>
void    print(Vector<T> vect) {
    if (vect.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

void    print(StringVector strs) {
    for (const String &line : strs)
        std::cout << line << std::endl;
}

template <typename T, typename U>
void    print(const Map<T, U> &map) {
    for (auto const &pair : map) {
        std::cout << pair.first << ": " << pair.second << " | ";
    }
    std::cout << std::endl;
}

template<typename T>
void    print(const Set<T> s) {
    if (s.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[" << *(s.begin());
    for (typename Set<T>::iterator it = s.begin(); it != s.end();)
        std::cout << ", " << *it++;
    std::cout << "]" << std::endl;
}

String  readFile(String filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

StringVector    split(String str, String separators, bool check_in=true) {
    StringVector    strs;
    String          tmp;
    size_t          i = 0, j = 0;

    while (i < str.size()) {
        if ((check_in and separators.find(str[i]) == String::npos)
                or (not check_in and str.compare(i, separators.size(), separators) != 0)) {
            i++;
            continue;
        }
        tmp = str.substr(j, i - j);
        if (not tmp.empty())
            strs.push_back(tmp);
        i += (check_in) ? 1 : separators.size();
        j = i;
    }
    if (j < i and not tmp.empty())
        strs.push_back(str.substr(j, i - j));
    return strs;
}

u_int parseMatrix(const String &str) {
    const u_int width = 5;
    u_int       col = 0, pins = 0;

    for (u_int i = width; i < str.size() - width; i++) {
        if (str[i] == '\n') {
            col = 0;
            continue;
        }
        pins += (str[i] == '#') << ((4 - col) * 4);
        col++;
    }
    return pins;
}

void parseInput(const String &input, Set<u_int> &locks, Set<u_int> &keys) {
    StringVector splitted = split(input, "\n\n", false);

    for (const String &matrix : splitted) {
        u_int parsed = parseMatrix(matrix);
        if (matrix.compare(0, 5, "#####") == 0)
            locks.insert(parsed);
        else
            keys.insert(parsed);
    }
}

bool doesFit(const u_int &lock, const u_int &key) {
    u_int sum = lock + key;

    while (sum > 0) {
        if ((sum & 0x0F) > 5)
            return false;
        sum >>= 4;
    }
    return true;
}

u_int countFittingKeys(const Set<u_int> &locks, const Set<u_int> &keys) {
    u_int   fitting = 0;

    for (const u_int &key : keys)
        for (const u_int &lock : locks)
            fitting += doesFit(lock, key);
    return fitting;
}

// 3136
int main(void) {
    String      text = readFile("input.txt");
    Set<u_int>  locks, keys;

    parseInput(text, locks, keys);

    u_int fitting = countFittingKeys(locks, keys);

    std::cout << fitting << " unique pairs fit without overlapping." << std::endl;
    return (0);
}
