#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <regex>

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<unsigned char> out;
    int width = -1;

    while (std::getline(file, linetxt)) {
        width = 0;
        for (auto& c : linetxt) {
            out.push_back(c);
            width++;
        }
    }
    return std::make_pair(out, width);
}

template<typename T>
int64_t run1(T input) {
    auto [chars, width] = input;
    int height = chars.size() / width;
    int64_t sum = 0;
    int num = 0;
    bool nearSymbol = false;
    for (int i = 0; i < chars.size(); i++) {
        num = 0;
        nearSymbol = false;
        while(i < chars.size() && std::isdigit(chars[i])) {
            num *= 10;
            num += chars[i] - '0';

            int x = i % width;
            int y = i / width;

            for (int k = x - 1; k < x + 2; k++) {
                for (int j = y - 1; j < y + 2; j++){
                    if (k > -1 && k < width && j > -1 && j < height && !std::isdigit(chars[j * width + k]) && chars[j * width + k] != '.') { nearSymbol = true; }  // Dont think technically correct, might be able to read off the left or right of a line and up to a previous, works on input htough
                }
            }
            i++;
            if (i % width == 0) { break; }
        }
        if (nearSymbol) { sum += num; }
        if (num) { i--; }

    }
    return sum;
}

template<typename T>
int run2(T input) {
    auto [chars, width] = input;
    int height = chars.size() / width;
    int64_t sum = 0;
    int num = 0;
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] == '*') {

            std::vector<int> nums;
            std::map<std::pair<int, int>, bool> indices;

            int x = i % width;
            int y = i / width;

            for (int k = x - 1; k < x + 2; k++) {
                for (int j = y - 1; j < y + 2; j++) {
                    num = 0;
                    if (k > -1 && k < width && j > -1 && j < height && std::isdigit(chars[j * width + k]) && !indices[{k, j}]) {
                        indices[{k, j}] = true;
                        int h = k;
                        //Go back to start of number
                        while ((h + 1) % width != 0 && std::isdigit(chars[j * width + h])) { 
                            h-- ; 
                        }
                        h++;
                        //Read over number
                        while (std::isdigit(chars[j * width + h])) {
                            indices[{h, j}] = true; 
                            num *= 10; 
                            num += chars[j * width + h] - '0';
                            h++; 
                        }
                        nums.push_back(num);
                        num = 0;
                    }
                }
            }
            if (nums.size() == 2) { sum += nums[0] * nums[1]; }

        }

    }
    return sum;
}


int main() {
    auto input = getInput();

    /*for (auto g : input) {
        std::cout << g;
    }
    std::cout << '\n';*/
    /*std::cout << input.second << '\n';
    std::cout << input.first.size() << '\n';*/

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

//Part 1: 540025
//Part 2 : 84584891
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 8
//Ticks : 81374
//TotalDays : 9.41828703703704E-08
//TotalHours : 2.26038888888889E-06
//TotalMinutes : 0.000135623333333333
//TotalSeconds : 0.0081374
//TotalMilliseconds : 8.1374