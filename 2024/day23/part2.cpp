
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



std::ostream &operator <<(std::ostream &out, const Set<String> &set) {
    switch (set.size()) {
    case 0: return out;
    case 1: return out << *set.begin();
    default:
        auto it = set.begin();
        out << *it;
        ++it;
        for (;it != set.end(); ++it)
            out << "," << *it;
        return out;
    }
}

template <typename T>
void    print(const Vector<T> vect) {
    if (vect.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ",";
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
        std::cout << "," << *it;
    }
    std::cout << "]" << std::endl;
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

template <typename T>
Set<Set<T>> get_combinations(Set<T> s, u_int comb_size) {
    Set<Set<T>> combs;
    T tmp;

    if (s.size() == comb_size)
        combs.insert(s);
    if (s.size() <= comb_size)
        return combs;

    for (auto it = s.begin(); it != s.end(); it++) {
        tmp = *it;
        it = s.erase(it);
        Set<Set<T>> other_combs = get_combinations(s, comb_size);
        combs.insert(other_combs.begin(), other_combs.end());
        it = s.insert(it, tmp);
    }
    return combs;
}

bool is_clique(const Map<String, Set<String>> &nodes, const Set<String> &comb) {
    for (auto it = comb.begin(); it != comb.end(); ++it) {
        auto ot = it;
        ot++;
        for (; ot != comb.end(); ++ot)
            if (nodes.at(*it).find(*ot) == nodes.at(*it).end())
                return false;
    }
    return true;
}

// as,bu,cp,dj,ez,fd,hu,it,kj,nx,pp,xh,yu
int main(void) {
    String                      text = readFile("input.txt");
    Map<String, Set<String>>    nodes = parseConnections(text);
    Set<String>                 clique;
    u_int size = 15;

    while (size > 5) {
        for (const auto &[node, edges] : nodes) {
            for (auto comb : get_combinations(Set<String>(edges), size)) {
                if (not is_clique(nodes, comb))
                    continue;
                comb.insert(node);
                if (comb.size() > clique.size()){
                    clique = comb;
                    break;
                }
            }
            if (not clique.empty())
                break;
        }
        if (not clique.empty())
            break;
        size--;
    }

    std::cout << "The password for the LAN party is " << clique << std::endl;
    return (0);
}
