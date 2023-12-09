#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <numeric>
#include <regex>
#include <execution>
#include <functional>
#include <chrono>

#include <ctre.hpp>

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<std::vector<int>> out;
    
    std::regex ex("(-?\\d+)");
    std::smatch m;

    while (std::getline(file, linetxt)) {
        std::vector<int> temp;
        auto begin = std::sregex_iterator(linetxt.begin(), linetxt.end(), ex);
        auto end = std::sregex_iterator();

        for (std::sregex_iterator i = begin; i != end; i++) {
            m = *i;
            temp.push_back(std::stoll(m.str()));
        }
        out.push_back(temp);
    }

    return out;
}

auto getInputCTRE(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<std::vector<int>> out;

    while (std::getline(file, linetxt)) {
        std::vector<int> temp;
        for (const auto& m : ctre::search_all<"(-?\\d+)">(linetxt)) {
            temp.push_back(m.to_number<int>());
        }
        out.push_back(temp);
    }

    return out;
}

int calcDiff(std::vector<int>& v) {
    if (std::all_of(v.begin(), v.end(), [](const int& i) { return i == 0; })) { return 0; }

    std::vector<int> diffs;
    for (int i = 0; i < v.size() - 1; i++) {
        diffs.push_back(v[i + 1] - v[i]);
    }
    int end = calcDiff(diffs);

    return diffs[diffs.size() - 1] + end;

}

template<typename T>
int run1(T input) {
    int sum = 0;

    for (auto& v : input) {
        auto num = calcDiff(v);
        sum += v[v.size() - 1] + num;
    }
    return sum;
}



template<typename T>
int run2(T input) {
    int sum = 0;
    std::for_each(std::execution::par, input.begin(), input.end(), [](auto& v) { 
        std::transform(std::execution::par, v.cbegin(), v.cend(), v.begin(), std::negate<int>());
        std::reverse(v.begin(), v.end()); 
        });

    for (auto& v : input) {
        auto num = calcDiff(v);
        sum += -1 * (v[v.size() - 1] + num);
    }
    return sum;
}

auto runWithInputFetching(std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<std::vector<int>> out;

    std::regex ex("(-?\\d+)");
    std::smatch m;

    int p1 = 0;
    int p2 = 0;

    while (std::getline(file, linetxt)) {
        std::vector<int> input;
        auto begin = std::sregex_iterator(linetxt.begin(), linetxt.end(), ex);
        auto end = std::sregex_iterator();

        for (std::sregex_iterator i = begin; i != end; i++) {
            m = *i;
            input.push_back(std::stoll(m.str()));
        }
        
        auto num = calcDiff(input);
        p1 += input[input.size() - 1] + num;

        std::transform(std::execution::par, input.cbegin(), input.cend(), input.begin(), std::negate<int>());
        std::reverse(input.begin(), input.end());

        num = calcDiff(input);
        p2 += -1 * (input[input.size() - 1] + num);

    }

    std::cout << "PART 1: " << p1 << "\nPART 2: " << p2 << '\n';
}

auto runWithInputFetchingCTRE(std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<std::vector<int>> out;

    std::regex ex("(-?\\d+)");
    std::smatch m;

    int p1 = 0;
    int p2 = 0;

    while (std::getline(file, linetxt)) {
        std::vector<int> input;
        for (const auto& m : ctre::search_all<"(-?\\d+)">(linetxt)) {
            input.push_back(m.to_number<int>());
        }

        auto num = calcDiff(input);
        p1 += input[input.size() - 1] + num;

        std::transform(std::execution::par, input.cbegin(), input.cend(), input.begin(), std::negate<int>());
        std::reverse(input.begin(), input.end());

        num = calcDiff(input);
        p2 += -1 * (input[input.size() - 1] + num);

    }

    std::cout << "PART 1: " << p1 << "\nPART 2: " << p2 << '\n';
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    input = getInputCTRE();
    end = std::chrono::high_resolution_clock::now();
    auto inputCTRET = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    /*for (auto& h : input) {
        for (auto& i : h) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }*/
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "INPUT GOT IN: " << inputT << "\nCTRE INPUT GOT IN: " << inputCTRET << "\nPART 1 DONE IN: " << part1T << "\nPART 2 DONE IN: " << part2T << "\n";


    std::cout << "DOING ALL AT ONCE\n";
    start = std::chrono::high_resolution_clock::now();
    runWithInputFetching();
    end = std::chrono::high_resolution_clock::now();
    auto all = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "RAN IN: " << all << '\n';

    std::cout << "DOING ALL AT ONCE CTRE\n";
    start = std::chrono::high_resolution_clock::now();
    runWithInputFetchingCTRE();
    end = std::chrono::high_resolution_clock::now();
    auto allCTRE = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "RAN IN: " << allCTRE << '\n';
}

//Part 1: 1798691765
//Part 2: 1104
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 16
//Ticks : 163453
//TotalDays : 1.89181712962963E-07
//TotalHours : 4.54036111111111E-06
//TotalMinutes : 0.000272421666666667
//TotalSeconds : 0.0163453
//TotalMilliseconds : 16.3453


//Part 1: 1798691765
//Part 2: 1104
//INPUT GOT IN : 8197us
//CTRE INPUT GOT IN : 734us
//PART 1 DONE IN : 579us
//PART 2 DONE IN : 792us
//DOING ALL AT ONCE
//PART 1 : 1798691765
//PART 2 : 1104
//RAN IN : 8448us
//DOING ALL AT ONCE CTRE
//PART 1 : 1798691765
//PART 2 : 1104
//RAN IN : 2480us
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 34
//Ticks : 348199
//TotalDays : 4.03008101851852E-07
//TotalHours : 9.67219444444444E-06
//TotalMinutes : 0.000580331666666667
//TotalSeconds : 0.0348199
//TotalMilliseconds : 34.8199