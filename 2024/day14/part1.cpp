
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



struct Robot {
    int   x;
    int   y;
    int   a_x;
    int   a_y;
};


template <typename T>
void    print(Vector<T> vect) {
    std::cout << "[";
    for (size_t i = 0; i < vect.size() - 1; i++)
        std::cout << vect[i] << ", ";
    std::cout << vect.back() << "]" << std::endl;
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

int mod(int n, const int m) {
    while (n < 0)
        n += m;
    while (n >= m)
        n -= m;
    return (n);
}

Vector<Robot>   parseRobots(StringVector lines) {
    Vector<Robot>   robots;

    for (String line : lines) {
        Robot robot;

        robot.x = std::stoi(line.substr(line.find('=') + 1));
        robot.y = std::stoi(line.substr(line.find(',') + 1));
        line = line.substr(line.find(' '));
        robot.a_x = std::stoi(line.substr(line.find('=') + 1));
        robot.a_y = std::stoi(line.substr(line.find(',') + 1));

        robots.push_back(robot);
    }
    return robots;
}


// 226548000
int main(void) {
    int             X = 101, Y =103;
    String          text = readFile("input.txt");
    StringVector    lines = split(text, "\n");

    Vector<Robot>   robots = parseRobots(lines);
    Vector<int>     quadrants({0, 0, 0, 0});

    for (Robot &robot: robots) {
        robot.x  = mod((robot.x + robot.a_x * 100), X);
        robot.y  = mod((robot.y + robot.a_y * 100), Y);

        if (robot.x < X / 2 and robot.y < Y / 2)
            quadrants[0]++;
        else if (robot.x > X / 2 and robot.y < Y / 2)
            quadrants[1]++;
        else if (robot.x > X / 2 and robot.y > Y / 2)
            quadrants[2]++;
        else if (robot.x < X / 2 and robot.y > Y / 2)
            quadrants[3]++;
    }

    std::cout << "The safety factor is "
    << quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3]
    << std::endl;
    return (0);
}
