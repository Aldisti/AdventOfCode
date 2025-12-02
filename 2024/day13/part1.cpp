
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <forward_list>
#include <cmath>
#include <set>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::set<T>;

template <typename T>
using List = std::forward_list<T>;

template <typename T>
using Pair = std::pair<T,T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using LongList = List<long>;
using CharVector = Vector<char>;
using IntPair = Pair<size_t>;
using IntPairSet = Set<IntPair>;


struct Button {
    int     x;
    int     y;

    Button() {
        this->x = 0;
        this->y = 0;
    }

    Button(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct Machine {
    Button  a;
    Button  b;
    Button  prize;

    Machine() { }

    Machine(Button a, Button b, Button prize) {
        this->a = a;
        this->b = b;
        this->prize = prize;
    }
};


template <typename T>
void    printVector(Vector<T> vect) {
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

template <typename T>
void    printList(List<T> mylist) {
    std::cout << "[";
    for (auto it = mylist.begin(); it != mylist.end(); ++it)
        std::cout << ((it != mylist.begin()) ? " " : "") << *it;
    std::cout << "]" << std::endl;
}

void    printMachine(const Machine &m) {
    std::cout << "A(" << m.a.x << "," << m.a.y
    << ") B(" << m.b.x << "," << m.b.y << ") Prize("
    << m.prize.x << "," << m.prize.y << ")" << std::endl;
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

Button parseButton(String line, bool parsePrize=false) {
    return Button(
        std::stoi(line.substr(line.find("X") + 1 + parsePrize)),
        std::stoi(line.substr(line.find("Y") + 1 + parsePrize))
    );
}

size_t  calculate(Machine m) {
    IntPair pair(0, 0);
    size_t  tmp;

    for (int i = 1; i < m.prize.x / m.a.x + 1; i++) {
        for (int j = 1; j < m.prize.x / m.b.x + 1; j++) {
            if (m.a.x * i + m.b.x * j != m.prize.x or m.a.y * i + m.b.y * j != m.prize.y)
                continue;
            tmp = i * 3 + j;
            if (pair == IntPair(0, 0) or (tmp < pair.first * 3 + pair.second))
                pair = IntPair(i, j);
        }
    }
    return (pair.first * 3 + pair.second);
}

// 32067
int main(void) {
    String          text = readFile("part1.txt");
    StringVector    lines = split(text, "\n");
    Machine         machine;
    size_t          count = 0;


    for (size_t i = 0; i < lines.size(); i += 3) {
        machine = Machine(
            parseButton(lines[i]),
            parseButton(lines[i + 1]),
            parseButton(lines[i + 2], true)
        );
        count += calculate(machine);
    }

    std::cout << "The fewest tokens you would have to spend is " << count << std::endl;
    return (0);
}
