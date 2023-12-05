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

#define PRINT_TOTAL_SEEDS false
#define PRINT_COUNT_100000 false

struct Almanac {
    std::vector<int64_t> seeds;

    std::map<std::string, std::vector<std::tuple<int64_t, int64_t, int64_t>>> maps = {
                                                                            {"seed-to-soil map:", {}},
                                                                            {"soil-to-fertilizer map:", {}},
                                                                            {"fertilizer-to-water map:", {}},
                                                                            {"water-to-light map:", {}},
                                                                            {"light-to-temperature map:", {}},
                                                                            {"temperature-to-humidity map:", {}},
                                                                            {"humidity-to-location map:", {}}
                                                                        };


    void print() {
        std::cout << "SEEDS: ";
        for (auto& s : seeds) { std::cout << s << ' '; }
        std::cout << '\n';
        for (auto& [key, val] : maps) {
            std::cout << "KEY: " << key << '\n';
            for (auto& [a, b, c] : val) {
                std::cout << a << ' ' << b << ' ' << c << '\n';
            }
            std::cout << '\n';
        }
    }
};


auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    Almanac out;

    std::string currMap;

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
                out.maps[currMap].push_back({ std::stoll(m[2]), std::stoll(m[1]) , std::stoll(m[3]) });
            }
            else {
                currMap = linetxt;
            }
        }
    
    }
    return out;
}


int64_t traverseMap(const Almanac& almanac, int64_t seed) {
    auto mapVal = [&](const std::string str, int64_t s) {
        for (auto& [from, to, range] : almanac.maps.at(str)) {
            if (s >= from && s < from + range) { return to + (s - from); }
        }
        return s;
    };
    
    return mapVal("humidity-to-location map:", mapVal("temperature-to-humidity map:", mapVal("light-to-temperature map:", mapVal("water-to-light map:", mapVal("fertilizer-to-water map:", mapVal("soil-to-fertilizer map:", mapVal("seed-to-soil map:", seed)))))));

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

template<typename T>
int run2(T input) {

#if PRINT_TOTAL_SEEDS
    int totalSeeds = 0;
    for (int i = 0; i < input.seeds.size(); i += 2) {
        totalSeeds += input.seeds[i + 1];
    }
    std::cout << "TOTAL SEEDS: " << totalSeeds << '\n';
#endif

#if PRINT_COUNT_100000
    int count = 0;
#endif

    int64_t min = INT64_MAX;
    for (int i = 0; i < input.seeds.size(); i += 2) {
        int64_t seed = input.seeds[i];
        int64_t range = input.seeds[i + 1];
        for (int64_t j = 0; j < range; j++) {
            int64_t loc = traverseMap(input, seed + j);
            min = (loc < min) ? loc : min;
#if PRINT_COUNT_100000
            if (++count % 100000 == 0) { std::cout << "COUNT: " << count << '\n'; }
#endif
        }
    }
    return min;
}


int main(int argc, char **argv) {
    auto input = getInput();

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

//Part 1: 662197086
//Part 2: 52510809
//
//
//Days: 0
//Hours : 0
//Minutes : 21
//Seconds : 51
//Milliseconds : 23
//Ticks : 13110233263
//TotalDays : 0.0151738810914352
//TotalHours : 0.364173146194444
//TotalMinutes : 21.8503887716667
//TotalSeconds : 1311.0233263
//TotalMilliseconds : 1311023.3263