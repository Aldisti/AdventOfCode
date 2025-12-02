
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
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
using StrPair = Pair<String>;





template <typename T>
void    print(const Vector<T> vect) {
    if (vect.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

template<typename T>
void    print(const Set<T> s) {
    if (s.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[" << *(s.begin());
    for (typename Set<T>::iterator it = s.begin(); it != s.end();) {
        ++it;
        if (it == s.end())
            break;
        std::cout << ", " << *it;
    }
    std::cout << "]" << std::endl;
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

Map<String, Set<String>> parseConnections(const String &str) {
    StringVector                cons = split(str, "\n-");
    Map<String, Set<String>>    connections;

    for (u_int i = 0; i < cons.size(); i += 2) {
        if (connections.find(cons[i]) == connections.end())
            connections[cons[i]] = Set<String>();
        if (connections.find(cons[i + 1]) == connections.end())
            connections[cons[i + 1]] = Set<String>();
        connections[cons[i]].insert(cons[i + 1]);
        connections[cons[i + 1]].insert(cons[i]);
    }
    return connections;
}

// 1083
int main(void) {
    String                      text = readFile("input.txt");
    Map<String, Set<String>>    connections = parseConnections(text);
    Set<Set<String>>            lans;

    for (const auto &pair : connections) {
        if (pair.first[0] != 't')
            continue;
        for (const auto &second : pair.second) {
            if (second == pair.first)
                continue;
            for (const auto &third : connections[second]) {
                if (pair.first == third)
                    continue;
                if (connections[third].find(pair.first) == connections[third].end())
                    continue;
                lans.insert(Set<String>({pair.first, second, third}));
            }
        }
    }

    std::cout << "The LAN parties containing at least a t are " << lans.size() << std::endl;
    return (0);
}
