
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

size_t  count(const Map<long, size_t> &map) {
    size_t  count = 0;

    for (auto const &pair: map) {
        count += pair.second;
    }
    return count;
}

Map<long, size_t>   toMap(const StringVector &nums) {
    Map<long, size_t>   stones;

    for (const String &num : nums) {
        stones[std::stol(num)] += 1;
    }
    return stones;
}

int numOfDigits(long num) {
    int digits = 0;
    do {
        num /= 10;
        digits++;
    } while (num > 0);
    return digits;
}

void  blink(Map<long, size_t> &stones) {
    Map<long, size_t>   new_stones;
    long                key;
    size_t              value;
    int                 digits;

    for (auto it = stones.begin(), next = it; it != stones.end();  it = next) {
        ++next;
        key = it->first;
        value = it->second;

        if (value <= 0) {
            stones.erase(key);
            continue;
        }

        if (key == 0) {
            new_stones[1] += value;
            stones[key] -= value;
            continue;
        }

        digits = numOfDigits(it->first);
        if (digits % 2 != 0) {
            new_stones[key * 2024] += value;
            stones[key] -= value;
            continue;
        }

        digits = std::pow(10, digits / 2);
        new_stones[key / digits] += value;
        new_stones[key % digits] += value;
        stones[key] -= value;
    }

    for (const auto elem : new_stones) {
        stones[elem.first] += elem.second;
    }
}

// example1 1 [1 2024 1 0 9 9 2021976] 7
// example2 6 [125 17] 22 
// part1 25 [...] 224529
// part2 75 [...] 266820198587914
int main(void) {
    String          text = readFile("part1.txt");
    Map<long, size_t>  stones = toMap(split(text, " "));

    for (size_t iterations = 0; iterations < 75; iterations++)
        blink(stones);

    std::cout << "The total number of stones is " << count(stones) << std::endl;
    return (0);
}
