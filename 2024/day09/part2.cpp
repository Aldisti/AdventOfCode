
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

using StringVector = Vector<String>;
using IntVector = Vector<int>;
using CharVector = Vector<char>;



template <typename T>
std::ostream &operator <<(std::ostream &out, const Vector<T> &v) {
    switch (v.size()) {
    case 0:
        return out << "[]";
    case 1:
        return out << "[" << v.front() << "]";
    default:
        out << "[" << v.front();
        for (u_int i = 1; i < v.size(); i++)
            std::cout << ", " << v[i];
        return out << "]";
    }
}

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

IntVector intoRepresentation(String str) {
    IntVector   repr;
    int         digit, tmp;

    for (size_t i = 0, id = 0; i < str.size(); i++) {
        digit = (int) (str[i] - 48);
        tmp = (i % 2 == 0) ? id++ : -1;
        while (digit-- > 0)
            repr.push_back(tmp);
    }
    return repr;
}

u_int find_empty_of(const IntVector &repr, const u_int &size, u_int max) {
    u_int i = 0, j;
    if (max > repr.size())
        max = repr.size();
    while (i <= max) {
        while (i <= max and repr[i] != -1)
            i++;
        j = i;
        while (j <= max and repr[j] == -1)
            j++;
        if (i <= max and j - i >= size) {
            return i;
        }
        i++;
    }
    return -1;
}

// 6351801932670
int main(void) {
    String          text = readFile("part1.txt");
    IntVector       repr = intoRepresentation(text);

    u_int i = 0;
    u_int j = repr.size() - 1;
    while (j < repr.size()) {
        while (j < repr.size() and j > 0 and repr[j] == -1) j--;
        u_int h = j;
        do j--;
        while (j < repr.size() and j > 0 and (repr[j] == repr[j + 1]));

        if (j >= repr.size())
            break;

        i = find_empty_of(repr, h - j, j + 1);
        if (i > j)
            continue;

        while (h > j) {
            repr[i++] = repr[h];
            repr[h--] = -1;
        }
    }

    u_long count = 0; // calculate the checksum
    for (i = 0; i < repr.size(); i++)
        if (repr[i] != -1)
            count += i * repr[i];

    std::cout << "The filesystem checksum is " << count << std::endl;
    return (0);
}
