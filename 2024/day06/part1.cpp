
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Pair = std::pair<T,T>;

template <typename T>
using PairVector = Vector<Pair<T>>;

using StringVector = Vector<String>;
using IntVector = Vector<int>;



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
    size_t          i = 0, j = 0;

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

void    findGuard(StringVector map, size_t &r, size_t &c) {
    for (r = 0; r < map.size(); r++) {
        c = 0;
        while (c < map[r].size() and map[r][c] != '^')
            c++;
        if (c != map[r].size())
            break;
    }
}

template <typename T>
T   next(Vector<T> &directions) {
    directions.push_back(directions.front());
    directions.erase(directions.begin());
    return directions.back();
}

// 4696
int main(void) {
    String          text = readFile("part1.txt");
    StringVector    map = split(text, "\n");
    PairVector<int> directions({
        {-1, 0}, // up
        {0, 1},  // right
        {1, 0},  // down
        {0, -1}  // left
    });
    Pair<int>       dir;
    size_t          r, c, count;

    findGuard(map, r, c);

    count = 0;
    dir = next(directions);
    while (true) {
        count += (map[r][c] != ' ');
        map[r][c] = ' ';
        if (r + dir.first < 0 or r + dir.first >= map.size()
            or c + dir.second < 0 or c + dir.second >= map[0].size())
            break;
        if (map[r + dir.first][c + dir.second] == '#') { // change direction
            dir = next(directions);
            continue;
        }
        r += dir.first;
        c += dir.second;
    }

    std::cout << "The number of distinct positions visited is " << count << std::endl;
    return (0);
}
