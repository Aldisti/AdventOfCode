
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
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
using Queue = std::queue<T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using CharVector = Vector<char>;
using IntPair = Pair<int>;



struct Point {
    u_int           x;
    u_int           y;
    u_int           d;

    Point(): x(0), y(0), d(0) { }
    Point(const u_int &x, const u_int &y): x(x), y(y), d(0) { }
    Point(const u_int &x, const u_int &y, const u_int &d): x(x), y(y), d(d) { }
    Point(const Point &p): x(p.x), y(p.y), d(p.d) { }

    Point &operator =(const Point &p) {
        x = p.x;
        y = p.y;
        d = p.d;
        return *this;
    }

    Point operator +(const Point &p) const {
        return Point(x + p.x, y + p.y, d + p.d);
    }

    bool operator <(const Point &p) const { return x < p.x and y < p.y; }

    bool operator ==(const Point &p) const { return x == p.x and y == p.y; }

    bool operator !=(const Point &p) const { return x != p.x or y != p.y; }

    u_int to_int() const { return x * 1000 + y; }

    float distance(const Point &p) {
        return abs(x - p.x) + abs(y - p.y);
    }
};

std::ostream &operator <<(std::ostream &out, const Point &p) {
    return out << "(" << p.x << ", " << p.y << ", " << p.d << ")";
}




template <typename T>
void    print(const Vector<T> &vect) {
    if (vect.size() == 0) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
}

void    print(const StringVector &strs) {
    for (const String &line : strs)
        std::cout << line << std::endl;
}

template <typename T>
void    print(const Set<T> &set) {
    std::cout << "[" << *(set.begin());
    for (auto it = set.begin(); it != set.end();) {
        ++it;
        std::cout << ", " << *it;
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

void printColoredMap(const StringVector &map, const char wall='#',
                        const char player='@', const char visited='O') {
    for (const String &row : map) {
        for (const char &c : row) {
            if (c == wall)
                std::cout << "\033[1;31m";
            else if (c == visited)
                std::cout << "\033[1;32m";
            else if (c == player)
                std::cout << "\033[1;34m";
            std::cout << c << "\033[0m";
        }
        std::cout << std::endl;
    }
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

Point findPoint(const StringVector &map, char c) {
    for (u_int y = 0; y < map.size(); y++) {
        for (u_int x = 0; x < map.at(y).size(); x++)
            if (map.at(y).at(x) == c)
                return Point(x, y);
    }
    return Point();
}

Vector<Vector<bool>> mapWalls(const StringVector &map, const char wall='#') {
    Vector<Vector<bool>>   walls(map.size(), Vector<bool>(map.front().size(), false));

    for (u_int y = 0; y < map.size(); y++) {
        for (u_int x = 0; x < map.at(y).size(); x++)
            if (map.at(y).at(x) == wall)
                walls[y][x] = true;
    }
    return walls;
}

Vector<Point> dirs({
    Point(0, -1, 1),
    Point(0, +1, 1),
    Point(-1, 0, 1),
    Point(+1, 0, 1),
});

Vector<Point> findPath(const StringVector &map, const Point &start, const Point &end) {
    Vector<Vector<bool>> visited = mapWalls(map);
    Map<u_int, Point> parents;

    Queue<Point> queue;
    queue.push(start);
    visited[start.y][start.x] = true;

    while (not queue.empty()) {
        Point p = queue.front();
        queue.pop();
 
        if (p == end) {
            Vector<Point> path({p});
            Point tmp = p;
            while (tmp != start) {
                tmp = parents[tmp.to_int()];
                path.push_back(tmp);
            }
            return path;
        }
 
        std::sort(dirs.begin(), dirs.end(), [p, end](const Point &a, const Point &b){
            return (p + a).distance(end) < (p + b).distance(end);
        });

        for (const Point &dir : dirs) {
            Point next = p + dir;
            if (next.y >= map.size() or next.x >= map[next.y].size()
                    or visited[next.y][next.x])
                continue;
            queue.push(next);
            parents[next.to_int()] = p;
            visited[next.y][next.x] = true;
        }
    }
    return Vector<Point>();
}

bool is_removable(const StringVector &map, const Point &wall) {
    if (wall.y >= map.size() or wall.x >= map[wall.y].size())
        return false;
    if (map[wall.y][wall.x] != '#')
        return false;
    return true;
}

template <typename T>
u_int find(const Vector<T> &haystack, const T &needle) {
    for (u_int i = 0; i < haystack.size(); i++)
        if (haystack[i] == needle)
            return i;
    return -1;
}

// 1399
int main(void) {
    String              text = readFile("input.txt");
    StringVector        map = split(text, "\n");
    Point               start = findPoint(map, 'S'), end = findPoint(map, 'E');

    Vector<Point> path = findPath(map, start, end);

    Point next;
    u_int count = 0;
    for (u_int i = 0; i < path.size(); i++) {
        const Point &p = path.at(i);
        for (const Point &dir : dirs) {
            next = p + dir;
            if (not is_removable(map, next))
                continue;
            next = next + dir;
            u_int j = find(path, next);
            if (j > path.size() or j <= i)
                continue;
            if (j - i - 2 >= 100)
                count++;
        }
    }

    std::cout << count << " could save at least 100 seconds" << std::endl;
    return (0);
}
