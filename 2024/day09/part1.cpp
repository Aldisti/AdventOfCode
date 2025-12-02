
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

// 6323641412437
int main(void) {
    String      text = readFile("part1.txt");
    IntVector   repr = intoRepresentation(text);

    size_t  i, j, count;

    i = 0;
    j = repr.size() - 1;
    while (j > i) {
        while (i < j and repr[i] != -1) i++; // find the first empty space
        while (j > i and repr[j] == -1) j--; // skip all the empty spaces

        repr[i++] = repr[j];
        repr[j--] = -1;

    }

    count = 0; // calculate the checksum
    for (i = 0; i < repr.size() and repr[i] != -1; i++)
        count += i * repr[i];

    std::cout << "The filesystem checksum is " << count << std::endl;
    return (0);
}
