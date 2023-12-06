#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <regex>
#include <execution>

struct Race {
    int64_t time;
    int64_t dist;

    void print() {
        std::cout << "TIME: " << time << '\n';
        std::cout << "DISTANCE: " << dist << '\n';
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Race> out;

    std::regex ex("\\D*([\\d]+)\\D*");
    std::smatch m;

    //Times line
    std::getline(file, linetxt);
    while (std::regex_search(linetxt, m, ex)) {
        Race r;
        r.time = std::stoi(m[1]);
        linetxt.erase(0, linetxt.find(m[0]) + m[0].length());
        out.push_back(r);
    }

    //Distance line
    std::getline(file, linetxt);
    int i = 0;
    while (std::regex_search(linetxt, m, ex)) {
        out[i++].dist = std::stoi(m[1]);
        linetxt.erase(0, linetxt.find(m[0]) + m[0].length());
    }

    return out;
}

int64_t calcDist(const int64_t heldFor, const int64_t maxT) {
    return heldFor * (maxT - heldFor);
}

template<typename T>
int run1(T input) {
    int prod = 1;
    for(auto & r : input) {
        int sum = 0;
        for (int i = 0; i < r.time; i++) {
            int d = calcDist(i, r.time);
            if (d > r.dist) { sum++; }
        }
        prod *= (sum) ? sum : 1;
    }

    return prod;
}



template<typename T>
int run2(T input) {
    Race r{ 0, 0 };
    std::string t{""};
    std::string d{""};
    for (auto& rac : input) {
        t += std::to_string(rac.time);
        d += std::to_string(rac.dist);
    }
    r.time = std::stoll(t);
    r.dist = std::stoll(d);

    int64_t sum = 0;
    for (int64_t i = 0; i <= r.time; i++) {
        int64_t d = calcDist(i, r.time);
        if (d > r.dist) { sum++; }
    }

    return sum;
}


int main() {
    auto input = getInput();
    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

//Part 1: 625968
//Part 2: 43663323
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 23
//Ticks : 233864
//TotalDays : 2.70675925925926E-07
//TotalHours : 6.49622222222222E-06
//TotalMinutes : 0.000389773333333333
//TotalSeconds : 0.0233864
//TotalMilliseconds : 23.3864