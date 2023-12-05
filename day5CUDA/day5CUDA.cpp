#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <sstream>

#include "mapping.h"

#include "kernel.cuh"

#define PRINT_TOTAL_SEEDS true
#define PRINT_COUNT_100000 true

struct Almanac {
    std::vector<int64_t> seeds;

    std::vector<std::vector<Mapping>> maps = {{}};
};


auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    Almanac out;

    int currMap = 0;

    std::regex ex("([\\d]+) ([\\d]+) ([\\d]+)");
    std::smatch m;

    while (std::getline(file, linetxt)) {
        if (linetxt.substr(0, 6) == "seeds:") {
            std::stringstream ss;
            ss << linetxt;
            std::string temp;
            int64_t num;
            while (!ss.eof()) {
                ss >> temp;
                if (std::stringstream(temp) >> num) {
                    out.seeds.push_back(num);
                }
            }
        }
        else {
            if (std::isdigit(linetxt[0])) {
                std::regex_match(linetxt, m, ex);
                Mapping ma;
                ma.from = std::stoll(m[2]);
                ma.to = std::stoll(m[1]);
                ma.range = std::stoll(m[3]);
                out.maps[currMap].push_back(ma);
            }
            else if(linetxt == ""){
                currMap++;
                out.maps.push_back({});
            }
        }
    }
    return out;
}


int64_t traverseMap(const Almanac& almanac, int64_t seed) {
    auto mapVal = [&](const int str, int64_t s) {
        for (auto& a : almanac.maps[str]) {
            if (s >= a.from && s < a.from + a.range) { return a.to + (s - a.from); }
        }
        return s;
    };
    
    return mapVal(7, mapVal(6, mapVal(5, mapVal(4, mapVal(3, mapVal(2, mapVal(1, seed)))))));

}

template<typename T>
int run1(T input) {
    int64_t min = INT64_MAX;
    for (auto& s : input.seeds) {
        int64_t loc = traverseMap(input, s);
        min = (loc < min) ? loc : min;
    }
    return min;
}


int main(int argc, char **argv) {
    auto input = getInput();
    
    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << pt2(input.seeds, input.maps) << '\n';
}

//Part 1: 662197086
//Part 2: 52510809
//
//real	0m7.157s
//user	0m0.025s
//sys	0m0.085s