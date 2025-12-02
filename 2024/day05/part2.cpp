
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;
template <typename T, typename D>
using Map = std::map<T,D>;
template <typename T>
using Set = std::unordered_set<T>;

using StringVector = Vector<String>;
using IntVector = Vector<int>;



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

Map<int, Set<int>> parseRules(StringVector lines, size_t &i) {
    Map<int, Set<int>>  rules;
    StringVector        splitted;
    String              line;
    size_t              j;

    for (i = 0; i < lines.size(); i++) {
        line = lines[i];
        if (line.empty())
            break;
        splitted = split(line, "|");
        j = std::stoi(splitted[0]);
        if (rules.find(j) == rules.end())
            rules[j] = Set<int>();
        rules[j].insert(std::stoi(splitted[1]));
    }
    return rules;
}

Vector<IntVector>   parseUpdates(StringVector lines, size_t &i) {
    Vector<IntVector>   updates;
    IntVector           update;
    StringVector        splitted;
    String              line;

    for (i++; i < lines.size(); i++) {
        line = lines[i];
        if (line.empty())
            break;
        splitted = split(line, ",");
        update = IntVector();
        for (String num : splitted)
            update.push_back(std::stoi(num));
        updates.push_back(update);
    }
    return updates;
}

// 5466
int main(void) {
    String              text = readFile("part1.txt");
    Map<int, Set<int>>  rules;
    Vector<IntVector>   updates;
    size_t              i, j;
    int                 sum;

    StringVector lines = split(text, "\n"), splitted;

    rules = parseRules(lines, i);
    updates = parseUpdates(lines, i);

    sum = 0;
    for (IntVector update : updates) {
        bool ordered = true;
        for (i = 1; i < update.size(); i++) {
            Set<int> ruleSet = rules[update[i]];
            for (j = 0; j < i; j++)
                if (ruleSet.find(update[j]) != ruleSet.end())
                    break;
            if (j == i)
                continue;
            ordered = false;
            int tmp = update[i];
            update.erase(update.begin() + i);
            update.insert(update.begin() + j, tmp);
            i = j;
        }
        if (ordered)
            continue;
        sum += update[update.size() / 2];
    }

    std::cout << "The sum of middle numbers in the unordered updates is " << sum << std::endl;
    return (0);
}