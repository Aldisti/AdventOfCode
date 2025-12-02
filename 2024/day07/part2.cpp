
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

template <typename T>
using Queue = std::queue<T>;

using StringVector = Vector<String>;
using LongVector = Vector<long>;
using LongQueue = Queue<long>;



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

// stolq -> String TO Long Queue
LongQueue stolq(String str, String separator=" ") {
    LongQueue   nums;

    for (const String &num : split(str, separator))
        nums.push(std::stol(num));
    return nums;
}

template <typename T>
T   pop(Queue<T> & q) {
    T ret = q.front();
    q.pop();
    return ret;
}

void    combinationsHelper(Queue<String> &combinations, String currentComb, size_t n) {
    if (currentComb.size() == n) {
        combinations.push(currentComb);
        return;
    }
    combinationsHelper(combinations, currentComb + "+", n);
    combinationsHelper(combinations, currentComb + "*", n);
    combinationsHelper(combinations, currentComb + "|", n);
}

Queue<String>  generateCombinations(size_t n) {
    Queue<String> combinations;
    combinationsHelper(combinations, "", n);
    return combinations;
}

long    concatenate(long left, long right) {
    long    tmp = right;

    do {
        left *= 10;
        tmp /= 10;
    } while (tmp > 0);
    return left + right;
}

bool    checkEquation(LongQueue numbers, const String &operators, const long &sum) {
    long total, num;

    total = pop(numbers);
    for (size_t i = 0; i < operators.size() and numbers.size() > 0; i++) {
        num = pop(numbers);
        switch (operators[i]) {
            case '+': total += num;
                break;
            case '*': total *= num;
                break;
            case '|': total = concatenate(total, num);
                break;
        }
    }
    return total == sum;
}

// 4122618559853
int main(void) {
    String          text = readFile("part1.txt");
    StringVector    lines = split(text, "\n");
    Queue<String>   combs;
    LongQueue        equation;
    size_t          result = 0;

    for (String &line : lines) {
        line.replace(line.find(":"), 1, " ");
        equation = stolq(line);

        long sum = pop(equation);
        combs = generateCombinations(equation.size() - 1);
        while (combs.size() > 0) {
            if (checkEquation(equation, pop(combs), sum)) {
                result += sum;
                break;
            }
        }
    }

    std::cout << "The total calibration result is " << result << std::endl;
    return (0);
}
