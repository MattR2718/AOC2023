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
#include <chrono>

#include <boost/icl/interval_set.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/range/algorithm/copy.hpp>

#define PRINT_TOTAL_SEEDS false
#define PRINT_COUNT_100000 false
#define TRY_BRUTE_FORCE false

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
int run2BruteForce(T input) {

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
            std::cout << loc << '\n';
            min = (loc < min) ? loc : min;
#if PRINT_COUNT_100000
            if (++count % 100000 == 0) { std::cout << "COUNT: " << count << '\n'; }
#endif
        }
    }
    return min;
}

template<typename T>
int64_t run2Intervals(T input) {
    int64_t minOut = INT64_MAX;

    // Calculate start and ends of seed ranges, [s, e)
    std::vector<std::pair<int64_t, int64_t>> seedRanges;
    for (int i = 0; i < input.seeds.size() - 1; i+=2) {
        seedRanges.emplace_back(std::make_pair(input.seeds[i], input.seeds[i] + input.seeds[i + 1]));
    }

    // Names of different maps
    std::vector<std::string> mapNames = {"seed-to-soil map:",
                                        "soil-to-fertilizer map:",
                                        "fertilizer-to-water map:",
                                        "water-to-light map:",
                                        "light-to-temperature map:",
                                        "temperature-to-humidity map:",
                                        "humidity-to-location map:"};

    //Loop over all ranges of seeds
    for (auto& sr : seedRanges) {
        //Map to store seed intervals
        boost::icl::interval_map<int64_t, std::set<std::string>> is;
        //Add initial seed interval
        is += std::make_pair(boost::icl::interval<int64_t>::right_open(sr.first, sr.second), std::set<std::string>{"input"});
        
        //Loop over all maps
        for (const std::string& mn : mapNames) {
            //Loop over all maps in bigger maps, add interval to seedIntervalMap with value of map + index
            int i = 0;
            for (const auto& [ss, ms, l] : input.maps.at(mn)) {
                is += std::make_pair(boost::icl::interval<int64_t>::right_open(ss, ss + l), std::set<std::string>{"map" + std::to_string(i++)});
            }

            // Loop over all intervals and remove any which do not contain part of the input (contain the input tag)
            auto cleanis = is;
            for (auto& interval : is) {
                if (interval.second.size() < 2) {
                    for (auto& st : interval.second) {
                        if (st[0] == 'm') {
                            cleanis.subtract(interval);
                        }
                    }
                }
            }
            is = cleanis;

            // If intervals contain a map tag, map those values based on the map index at end of map tag, set all tags to input
            boost::icl::interval_map<int64_t, std::set<std::string>> ni;
            for (auto& interval : is) {
                if (interval.second.size() > 1) {
                    int mapIdx = -1;
                    for (auto& s : interval.second) {
                        if (s[0] == 'm') { mapIdx = std::stoi(s.substr(3, s.length() - 3)); }
                    }
                    if (mapIdx == -1) { std::cout << "ERROR WITH MAP INDEX\n"; }
                    auto& [ms, ss, l] {input.maps.at(mn)[mapIdx]};
                    ni += std::make_pair(boost::icl::discrete_interval<int64_t>::right_open(ss + (interval.first.lower() - ms), ss + (interval.first.upper() - ms)), std::set<std::string>{ "input" });
                }
                else {
                    ni += interval;
                }
            }
            is = ni;

            //if at location map, check for smallest location
            if (mn == "humidity-to-location map:") {
                for (auto& interval : is) {
                    if (interval.first.lower() < minOut) { minOut = interval.first.lower(); }
                    break;
                }
            }


        }
    }

    return minOut;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GOT INPUT IN: " << inputT << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    auto p1 = run1(input);
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 1: " << p1 << '\n';
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";

#if TRY_BRUTE_FORCE
    start = std::chrono::high_resolution_clock::now();
    auto p2B = run2BruteForce(input);
    end = std::chrono::high_resolution_clock::now();
    auto part2BT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 2 BRUTE FORCE: " << p2B << '\n';
    std::cout << "PART 2 BRUTE FORCE TOOK: " << part2BT << "\n\n";
#endif
    
    start = std::chrono::high_resolution_clock::now();
    auto p2I = run2Intervals(input);
    end = std::chrono::high_resolution_clock::now();
    auto part2IT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 2 INTERVALS: " << p2I << '\n';
    std::cout << "PART 2 INTERVALS TOOK: " << part2IT << "\n\n";

}

//GOT INPUT IN : 699us
//
//Part 1 : 662197086
//PART 1 TOOK : 16us
//
//Part 2 INTERVALS : 52510809
//PART 2 INTERVALS TOOK : 2677us
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 12
//Ticks : 124335
//TotalDays : 1.4390625E-07
//TotalHours : 3.45375E-06
//TotalMinutes : 0.000207225
//TotalSeconds : 0.0124335
//TotalMilliseconds : 12.4335


//ORIGINAL BRUTE FORCE
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