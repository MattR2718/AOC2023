#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <windows.h>

#define MAX_PATH 2048

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<std::string> out;
    while (std::getline(file, linetxt)) {
        out.push_back(linetxt);
    }
    return out;
}

template<typename T>
int run1(T input) {
    std::vector<int> nums;
    for (auto s : input) {
        int n = 0;
        for (auto c : s) {
            if (c - '0' >= 0 && c - '0' <= 9) {
                n += c - '0';
                n *= 10;
                break;
            }
        }
        for (int i = s.length() - 1; i > -1; i--) {
            char c = s[i];
            if (c - '0' >= 0 && c - '0' <= 9) {
                n += c - '0';
                break;
            }
        }
        nums.push_back(n);
    }
    return std::accumulate(nums.begin(), nums.end(), 0);
}

int checkWordNumber(std::string s, int i) {
    auto checkNum = [&](std::string num) {
        for (int j = 0; j < num.length(); j++) {
            if (j + i >= s.length() || num[j] != s[j + i]) {
                return false;
            }
        }
        return true;
    };

    if (checkNum("zero")) { return 0; }
    if (checkNum("one")) { return 1; }
    if (checkNum("two")) { return 2; }
    if (checkNum("three")) { return 3; }
    if (checkNum("four")) { return 4; }
    if (checkNum("five")) { return 5; }
    if (checkNum("six")) { return 6; }
    if (checkNum("seven")) { return 7; }
    if (checkNum("eight")) { return 8; }
    if (checkNum("nine")) { return 9; }

    return -1;
}

template<typename T>
int run2(T input) {
    std::vector<int> nums;

    for (const std::string s : input) {
        int n = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s[i];
            if (c - '0' >= 0 && c - '0' <= 9) {
                n += c - '0';
                n *= 10;
                break;
            }
            else {
                int d = checkWordNumber(s, i);
                if (d > -1) { n += d; n *= 10; break; }
            }
        }
        for (int i = s.length() - 1; i > -1; i--) {
            char c = s[i];
            if (c - '0' >= 0 && c - '0' <= 9) {
                n += c - '0';
                break;
            }
            else {
                int d = checkWordNumber(s, i);
                if (d > -1) { n += d; break; }
            }
        }
        nums.push_back(n);
    }

    return std::accumulate(nums.begin(), nums.end(), 0);
}


int main() {
    auto input = getInput();
    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

// Part 1: 55386
// Part 2 : 54824