
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <forward_list>
#include <cmath>
#include <set>
#include <map>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Set = std::set<T>;

template <typename T>
using List = std::forward_list<T>;

template <typename T, typename U>
using Map = std::map<T, U>;

template <typename T>
using Pair = std::pair<T,T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using LongList = List<long>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;



struct Point {
    u_int   x;
    u_int   y;

    Point() {
        x = 0;
        y = 0;
    }

    Point(const u_int &x, const u_int &y): x(x), y(y) { }
};


template <typename T>
void    print(Vector<T> vect) {
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

void    print(StringVector strs) {
    for (const String &line : strs)
        std::cout << line << std::endl;
}

template <typename T>
void    print(List<T> mylist) {
    std::cout << "[";
    for (auto it = mylist.begin(); it != mylist.end(); ++it)
        std::cout << ((it != mylist.begin()) ? " " : "") << *it;
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

String  parseMoves(String &input) {
    size_t  i = input.size() - 1;

    while (i < input.size() and (input[i] != '\n' or input[i - 1] != '\n')) {
        i--;
    }
    String moves = input.substr(i + 1);
    input = input.substr(0, i);
    return moves;
}

Point   findBot(const StringVector &map) {
    for (u_int  y = 0; y < map.size(); y++)
        for (u_int x = 0; x < map[y].size(); x++)
            if (map[y][x] == '@')
                return Point(x, y);
    return Point();
}

Map<char, Point>    directions({
    {'^', Point(0, -1)}, // up
    {'v', Point(0, +1)}, // down
    {'<', Point(-1, 0)}, // left
    {'>', Point(+1, 0)}  // right
});

void    move(StringVector &map, Point &bot, const Point &dir) {
    u_int   x = bot.x + dir.x, y = bot.y + dir.y;

    if (map[y][x] == '#')
        return;

    if (map[y][x] == '.') {
        bot.x += dir.x;
        bot.y += dir.y;
        return;
    }

    while (map[y][x] != '.') {
        x += dir.x;
        y += dir.y;
        if (y >= map.size() or x >= map[y].size() or map[y][x] == '#')
            return;
    }

    bot.x += dir.x;
    bot.y += dir.y;
    map[y][x] = map[bot.y][bot.x];
    map[bot.y][bot.x] = '.';
}

void    orderBoxes(StringVector &map, Point &bot, String &moves) {
    for (const char &m : moves) {
        if (directions.find(m) == directions.end())
            continue;
        move(map, bot, directions[m]);
    }
}

// 1457740
int main(void) {
    String          text = readFile("input.txt");
    String          moves = parseMoves(text);
    StringVector    map = split(text, "\n");
    Point           bot = findBot(map);
    size_t          sum = 0;

    map[bot.y][bot.x] = '.';
    orderBoxes(map, bot, moves);

    for (u_int y = 0; y < map.size(); y++) {
        for (u_int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 'O')
                continue;
            sum += y * 100 + x;
        }
    }

    std::cout << "The sum of all boxes' GPS coordinates is " << sum << std::endl;
    return (0);
}
