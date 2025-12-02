
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



struct Garden {
    StringVector    map;
    size_t          area;
    size_t          perimeter;
    IntPairSet      visited;
    IntPairSet      calculated;

    Garden() {
        map = StringVector();
        area = 0;
        perimeter = 0;
        visited = IntPairSet();
        calculated = IntPairSet();
    }

    Garden(StringVector garden) {
        map = garden;
        area = 0;
        perimeter = 0;
        visited = IntPairSet();
        calculated = IntPairSet();
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

bool alreadyVisited(const size_t &row, const size_t &col, const IntPairSet &visited) {
    return visited.find(IntPair(row, col)) != visited.end();
}

bool areValidCoords(const StringVector &map, const size_t &row, const size_t &col) {
    return (row < map.size() and col < map[row].size());
}

void    calculate(Garden & g, size_t row, size_t col) {
    static IntPairSet  directions({
        IntPair(-1, 0), // up
        IntPair(+1, 0), // down
        IntPair(0, -1), // left
        IntPair(0, +1)  // right
    });

    char    cell = g.map[row][col];

    g.visited.insert(IntPair(row, col));
    g.area++;

    for (const IntPair &dir : directions) {
        if (alreadyVisited(row + dir.first, col + dir.second, g.visited))
            continue;

        if (not areValidCoords(g.map, row + dir.first, col + dir.second)
                or g.map[row + dir.first][col + dir.second] != cell) {
            g.perimeter++;
            continue;
        }

        calculate(g, row + dir.first, col + dir.second);
    }
}

// example1 -> 140
// example2 -> 772
// example3 -> 1930
// part1    -> 1461752
int main(void) {
    String  text = readFile("part1.txt");
    Garden  garden = Garden(split(text, "\n"));
    size_t  price = 0;


    for (size_t row = 0; row < garden.map.size(); row++) {
        for (size_t col = 0; col < garden.map[row].size(); col++) {
            if (alreadyVisited(row, col, garden.calculated))
                continue;
            calculate(garden, row, col);
            price += garden.area * garden.perimeter;

            garden.area = 0;
            garden.perimeter = 0;
            garden.calculated.insert(garden.visited.begin(), garden.visited.end());
            garden.visited = IntPairSet();
        }
    }

    std::cout << "The total price is " << price << std::endl;
    return (0);
}
