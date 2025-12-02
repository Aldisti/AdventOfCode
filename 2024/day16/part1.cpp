
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

template <typename T>
using PairVector = Vector<Pair<T>>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;



struct Point {
    u_int   x;
    u_int   y;
    u_int   d;

    Point(): x(0), y(0), d(0) { }
    Point(u_int point): x(point >> 16), y(point & 0xFF), d(0) { }
    Point(u_long point): x(point >> 16 & 0xFF), y(point & 0xFF), d(point >> 32) { }
    Point(const u_int &x, const u_int &y): x(x), y(y), d(0) { }
    Point(const u_int &x, const u_int &y, const u_int &d): x(x), y(y), d(d) { }
    Point(const Point &p): x(p.x), y(p.y), d(p.d) { }

    Point &operator =(const Point &p) {
        x = p.x;
        y = p.y;
        d = p.d;
        return *this;
    }

    Point &operator +=(const Point &p) {
        x += p.x;
        y += p.y;
        d += p.d;
        return *this;
    }

    u_int to_int() const { return (x << 16) + y; }
    u_long to_long() const { return (((u_long) d) << 32) + to_int(); }

    Point operator +(const Point &p) { return Point(x + p.x, y + p.y, d + p.d); }
    Point operator -(const Point &p) { return Point(x - p.x, y - p.y, d - p.d); }

    bool operator ==(const Point &p) const { return x == p.x and y == p.y; }
    bool operator !=(const Point &p) const { return x != p.x or y != p.y; }

    bool operator <(const Point &p) const { return to_long() < p.to_long(); }
    bool operator >(const Point &p) const { return to_long() > p.to_long(); }
};

std::ostream &operator <<(std::ostream &out, Point const &p) {
    return out << "(" << p.x << ", " << p.y << ", " << p.d << ")";
}



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

const char wall = '#';
const char player = '@';
const char path1 = '1';
const char path2 = '2';
const char common = 'O';

void printColoredMap(const StringVector &map) {
    for (const String &row : map) {
        for (const char &c : row) {
            if (c == wall)
                std::cout << "\033[1;31m";
            else if (c == common)
                std::cout << "\033[1;32m";
            else if (c == path1)
                std::cout << "\033[1;34m";
            else if (c == path2)
                std::cout << "\033[1;36m";
            else if (c == player)
                std::cout << "\033[1;34m";
            std::cout << c << "\033[0m";
        }
        std::cout << std::endl;
    }
}

void debug(StringVector map, const Point &p=Point()) {
    map[p.y][p.x] = '@';
    printColoredMap(map);
    String __;
    std::getline(std::cin, __);
}

void printPath(StringVector map, const Vector<Point> &path, bool stop=false) {
    for (auto p : path) {
        map[p.y][p.x] = '@';
        if (stop)
            printColoredMap(map);
        map[p.y][p.x] = 'O';
        if (stop) {
            String __;
            std::getline(std::cin, __);
        }
    }
    printColoredMap(map);
    if (stop) {
        String __; std::getline(std::cin, __);
    }
}

Vector<Vector<bool>> map_walls(const StringVector &map, char wall) {
    Vector<Vector<bool>> walls(map.size(), Vector<bool>(map.front().size(), false));

    for (u_int y = 0; y < map.size(); y++)
        for (u_int x = 0; x < map[y].size(); x++)
            if (map[y][x] == wall)
                walls[y][x] = true;
    return walls;
}

Map<char, Point> dirs({
    {'^', Point(0, -1, 1)}, // up
    {'v', Point(0, +1, 1)}, // down
    {'<', Point(-1, 0, 1)}, // left
    {'>', Point(+1, 0, 1)}  // right
});

Point   find_point(const StringVector &map, const char &c) {
    for (u_int y = 0; y < map.size(); y++)
        for (u_int x = 0; x < map[y].size(); x++)
            if (map[y][x] == c)
                return Point(x, y);
    return Point();
}

struct compare_v {
    bool operator() (const Pair<Point> &lhs, const Pair<Point> &rhs) const {
        Point p1 = lhs.first, p2 = rhs.first;
        Point d1 = lhs.second, d2 = rhs.second;

        if (p1 == p2 and d1 == d2)
            return false;
        return p1 < p2;
    }
};

Vector<Point> find_path(const StringVector &map, const Point &start, const Point &end, const Point &s_dir) {
    Map<u_int, Point>   parents;
    Set<Pair<Point>> queue;
    std::set<Pair<Point>, compare_v>    visited;

    queue.insert({start, s_dir});
    while (not queue.empty()) {
        auto [p, prev_dir] = *queue.begin();
        queue.erase(queue.begin());
        visited.insert({p, prev_dir});

        if (p == end) {
            Vector<Point> path({p});
            Point tmp = p;
            u_int steps = p.d % 100;
            while (tmp != start and steps < 100) {
                tmp = parents[tmp.to_long()];
                path.insert(path.begin(), tmp);
                steps--;
            }
            return path;
        }

        for (auto [d, dir] : dirs) {
            Point next = p + dir;
            if (next.y >= map.size() or next.x >= map[next.y].size()
                    or map[next.y][next.x] == wall)
                continue;
            if (dir + prev_dir == Point()) {
                next.d += 1000;
                visited.insert({next, dir});
                continue;
            }
            if (visited.find({next, dir}) != visited.end())
                continue;
            if (dir != prev_dir)
                next.d += 1000;
            queue.insert({next, dir});
            parents[next.to_long()] = p;
        }
    }
    return Vector<Point>();
}

// 89460
int main(int ac, char *av[]) {
    String                  text = readFile((ac != 2) ? "input.txt" : av[1]);
    StringVector            map = split(text, "\n");
    Vector<Vector<bool>>    walls = map_walls(map, '#');

    Vector<Point> path = find_path(map, find_point(map, 'S'), find_point(map, 'E'), dirs['>']);

    std::cout << "The lowest score to possibly get is " << path.back().d << std::endl;
    return (0);
}
