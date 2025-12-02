
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

String  readFile(String filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

StringVector    split(String str, String separators) {
    StringVector    strs;
    String          tmp;
    size_t          i = 0, j = 0;

    while (i < str.size()) {
        if (separators.find(str[i]) == String::npos) {
            i++;
            continue;
        }
        tmp = str.substr(j, i - j);
        if (not tmp.empty())
            strs.push_back(tmp);
        j = ++i;
    }
    if (j < i) {
        if (not tmp.empty())
            strs.push_back(str.substr(j, i - j));
    }
    return strs;
}

bool ft_and(const bool a, const bool b) {
    return a and b;
}

bool ft_or(const bool a, const bool b) {
    return a or b;
}

bool ft_xor(const bool a, const bool b) {
    return a ^ b;
}

Map<String, bool (*)(const bool, const bool)> ops({
    {"AND", &ft_and},
    {"OR", &ft_or},
    {"XOR", &ft_xor},
});

String get_key(int n) {
    String key = "z";

    if (n < 10)
        key += "0";
    key += (char) (n + 48);
    return key;
}

template <typename T, typename U>
Vector<T> get_keys(const Map<T,U> &map) {
    Vector<T> keys;
    for (auto &it : map)
        keys.push_back(it.first);
    return keys;
}

// 66055249060558
int main(void) {
    String              text = readFile("input.txt");
    StringVector        lines = split(text, "\n");
    Map<String, bool>   registers;

    for (auto it = lines.begin(); it != lines.end(); ++it) {
        String line = *it;
        if (line.empty()) {
            it = lines.erase(it);
            continue;
        }
        if (line.find(":") == String::npos)
            break;
        StringVector values = split(line, ":");
        registers[values[0]] = std::stoi(values[1]);
    }

    auto it = lines.begin();
    while (it != lines.end()) {
        String &line = *it ;
        StringVector values = split(line, " ");
        if (registers.find(values[0]) == registers.end() or registers.find(values[2]) == registers.end()) {
            ++it;
            continue;
        }
        registers[values[4]] = ops[values[1]](registers[values[0]], registers[values[2]]);
        lines.erase(it);
        it = lines.begin();
    }

    StringVector keys = get_keys(registers);
    keys.erase(std::remove_if(keys.begin(), keys.end(), [](String key){
        return key[0] != 'z';
    }), keys.end());

    size_t res = 0;
    for (u_int i = keys.size() - 1; i < keys.size(); i--)
        res = (res << 1) + registers[keys[i]];

    std::cout << "The decimal number value is " << res << std::endl;
    return (0);
}
