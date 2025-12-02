
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(std::string filename) {
    std::ifstream       file(filename);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error("Error: file not open!");

    buffer << file.rdbuf();
    return buffer.str();
}

// 88802350
int main(void) {
    std::string     text = readFile("part1.txt");
    size_t          i = 0, j = 0;
    // true evaluates the mul, false doesn't
    bool            status = true;
    int             n1;
    long            sum = 0;

    while (i < text.size()) {
        if (!status and text.compare(i, 4, "do()") != 0) {
            i++;
            continue;
        }
        status = true;

        if (text.compare(i, 7, "don't()") == 0){
            status = false;
            i += 7;
            continue;
        }
        if (text.compare(i, 4, "mul(") != 0) {
            i++;
            continue;
        }

        i += 4; // add +4 after successful comparision with "mul("
        j = 0;
        // cycle all the digits you find
        while (i + j < text.size() and std::isdigit(text[i + j]))
            j++;
        // if the number has more than 3 digits
        // or less then one or the next char is not a comma
        // skip this 'mul', it's invalid
        if (j > 3 or j < 1 or text[i + j] != ',') {
            i += j;
            continue;
        }
        n1 = std::stoi(text.substr(i, j)); // save the first number for later
        i += j + 1; // move i after the comma (skip the first number)
        j = 0; // reset j to get the second number
        // like before, cycle through all the digits there are
        while (i + j < text.size() and std::isdigit(text[i + j]))
            j++;
        // as before, if not valid skip this 'mul'
        if (j > 3 or j < 1 or text[i + j] != ')') {
            i += j;
            continue;
        }
        // add the 'mul' to the sum
        sum += n1 * std::stoi(text.substr(i, j));
        i += j;
    }

    std::cout << "The safe reports are " << sum << std::endl;
    return (0);
}
