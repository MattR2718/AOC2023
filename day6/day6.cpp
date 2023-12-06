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

int calcDist(const int heldFor, const int maxT) {
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

    std::vector<int64_t> times(r.time + 1);
    std::iota(times.begin(), times.end(), 0);
    std::transform(std::execution::par, times.cbegin(), times.cend(), times.begin(), [&](int64_t t) { return (t * (r.time - t)) > r.dist; });


    return std::accumulate(times.begin(), times.end(), 0);
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
//Milliseconds : 366
//Ticks : 3668030
//TotalDays : 4.24540509259259E-06
//TotalHours : 0.000101889722222222
//TotalMinutes : 0.00611338333333333
//TotalSeconds : 0.366803
//TotalMilliseconds : 366.803