
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
using IntVector = Vector<int>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;



String readFile(String filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

StringVector split(String str, String separators, bool removeSeparator=true) {
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

int findNine(StringVector map, size_t row, size_t col, Set<IntPair> & alreadyVisited) {
    int ret = 0;

    alreadyVisited.insert(IntPair(row, col));
    if (map[row][col] == '9') {
        return (1);
    }

    // look up
    if (row > 0 and alreadyVisited.find(IntPair(row - 1, col)) == alreadyVisited.end() and map[row - 1][col] == map[row][col] + 1)
        ret += findNine(map, row - 1, col, alreadyVisited);
    // look down
    if (row < map.size() - 1 and alreadyVisited.find(IntPair(row + 1, col)) == alreadyVisited.end() and map[row + 1][col] == map[row][col] + 1)
        ret += findNine(map, row + 1, col, alreadyVisited);
    // look left
    if (col > 0 and alreadyVisited.find(IntPair(row, col - 1)) == alreadyVisited.end() and map[row][col - 1] == map[row][col] + 1)
        ret += findNine(map, row, col -1, alreadyVisited);
    // look right
    if (col < map[row].size() - 1 and alreadyVisited.find(IntPair(row, col + 1)) == alreadyVisited.end() and map[row][col + 1] == map[row][col] + 1)
        ret += findNine(map, row, col + 1, alreadyVisited);

    return (ret);
}

// 659
int main(void) {
    String          text = readFile("part1.txt");
    StringVector    map = split(text, "\n");
    Set<IntPair>    alreadyVisited;
    size_t          count = 0;

    for (size_t row = 0; row < map.size(); row++) {
        for (size_t col = 0; col < map[row].size(); col ++) {
            if (map[row][col] != '0')
                continue;
            alreadyVisited.clear();
            count += findNine(map, row, col, alreadyVisited);
        }
    }

    std::cout << "The sum of the scores of the trailheads is " << count << std::endl;
    return (0);
}
