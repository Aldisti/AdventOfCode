
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



struct Sequence {
    const static u_int SIZE = 4;
    int     sequence[SIZE];
    u_int   last;

    Sequence() {
        for (u_int i = 0; i < SIZE; i++)
            sequence[i] = 0;
        last = 0;
    }

    Sequence &add(int n) {
        if (last == SIZE) {
            for (u_int i = 0; i < SIZE - 1; i++)
                sequence[i] = sequence[i + 1];
            last--;
        }
        sequence[last++] = n;
        return *this;
    }

    u_int get_last() const { return last; }

    bool is_full() const { return last == SIZE; }

    int operator [](const u_int &index) const {
        if (index >= SIZE)
            throw new std::out_of_range("Index out of bounds");
        return sequence[index];
    }

    bool operator <(const Sequence &seq) const {
        for (u_int i = 0; i < SIZE; i++) {
            if (sequence[i] < seq[i])
                return true;
            if (sequence[i] > seq[i])
                return false;
        }
        return false;
    }

    bool operator ==(const Sequence &seq) const {
        for (u_int i = 0; i < SIZE; i++)
            if (sequence[i] != seq[i])
                return false;
        return true;
    }
};

std::ostream &operator <<(std::ostream &out, const Sequence &seq) {
    out << "(" << seq[0];
    for (u_int i = 1; i < Sequence::SIZE; i++)
        out << "," << seq[i];
    out << ")";
    return out;
}


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
    std::cout << "{" << map.begin()->first << ": " << map.begin()->second;
    for (auto it = map.begin(); it != map.end();) {
        it++;
        std::cout << "," << it->first << ": " << it->second;
    }
    std::cout << "}" << std::endl;
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

StringVector    split(String str, String separators="\n", bool check_in=true) {
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

Vector<u_long> parseInput(const String &str) {
    Vector<u_long> secrets;
    for (const String &num : split(str))
        secrets.push_back(std::stoul(num));
    return secrets;
}

u_long next_secret(const u_long &secret) {
    u_long new_secret = secret;

    new_secret = (new_secret ^ new_secret << 6) % 16777216;
    new_secret = (new_secret ^ new_secret >> 5) % 16777216;
    new_secret = (new_secret ^ new_secret << 11) % 16777216;

    return new_secret;
}

// 2277
int main(void) {
    String                  text = readFile("input.txt");
    Vector<u_long>          secrets = parseInput(text);
    Map<Sequence, u_int>    sequences;

    u_long  prev_sec;
    int     diff = 0;
    for (u_long &secret : secrets) {
        Sequence sequence;
        Set<Sequence> found;
        for (u_int i = 0; i < 2000; i++) {
            prev_sec = secret;
            secret = next_secret(secret);
            diff = (secret % 10) - (prev_sec % 10);
            sequence.add(diff);
            if (not sequence.is_full() or found.find(sequence) != found.end())
                continue;
            sequences[sequence] += secret % 10;
            found.insert(sequence);
        }
    }

    u_int greatest = 0;
    for (auto pair : sequences)
        if (pair.second > greatest)
            greatest = pair.second;

    std::cout << "The most bananas you can get is " << greatest << std::endl;
    return (0);
}
