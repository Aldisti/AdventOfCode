
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

void printColoredMap(const StringVector &map, const char wall='#', const char player='@', const char visited='O') {
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

StringVector    genMap(const u_int width, const u_int height, const char filler='+') {
    return StringVector(height, String(width, filler));
}

Vector<Point> parsePoints(const String &input) {
    Vector<Point>   points;
    StringVector    nums;

    for (auto line : split(input, "\n")) {
        nums = split(line, ",");
        points.push_back(Point(std::stoi(nums.front()), std::stoi(nums.back())));
    }
    return points;
}

Vector<Point> dirs({
    Point(0, -1, 1),
    Point(0, +1, 1),
    Point(-1, 0, 1),
    Point(+1, 0, 1),
});

void debug(StringVector map, const Point &p=Point()) {
    map[p.y][p.x] = '@';
    print(map);
    String __;
    std::getline(std::cin, __);
}

void printPath(StringVector map, const Vector<Point> &path, bool stop=true) {
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
    if (not stop) {
        String __; std::getline(std::cin, __);
    }
}

Vector<Point> findPath(const StringVector &map, Vector<Vector<bool>> visited) {
    Point end(map.size() - 1, map[0].size() - 1), start;
    Map<u_int, Point> parents;

    Queue<Point> queue;
    queue.push(start);
    visited[start.y][start.x] = true;

    while (not queue.empty()) {
        Point p = queue.front();
        queue.pop();

        if (p == end) {
            Vector<Point> path({p});
            while (p != start) {
                p = parents[p.to_int()];
                path.insert(path.begin(), p);
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

// 20,44
int main(void) {
    u_int                   size = 71;
    String                  text = readFile((size == 71) ? "input.txt" : "example.txt");
    StringVector            map = genMap(size, size, ' ');
    Vector<Point>           walls = parsePoints(text);
    Vector<Vector<bool>>    visited(size, Vector<bool>(size, false));

    u_int initial_walls = (size == 7) ? 12 : 1024;
    for (u_int i = 0; i < initial_walls; i++) {
        const Point &p = walls[i];
        if (p.y >= map.size() or p.x >= map[p.y].size())
            continue;
        visited[p.y][p.x] = true;
        map[p.y][p.x] = '#';
    }

    Vector<Point> path = findPath(map, visited);

    walls = Vector<Point>(walls.begin() + initial_walls, walls.end());

    for (const Point &wall : walls) {
        visited[wall.y][wall.x] = true;
        if (std::find(path.begin(), path.end(), wall) == path.end())
            continue;
        path = findPath(map, visited);
        if (path.empty()) {
            std::cout << "The first byte to cut off the exit is " << wall.x << "," << wall.y << std::endl;
            break;
        }
    }
    return (0);
}
