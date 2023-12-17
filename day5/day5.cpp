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

#include <boost/icl/interval_set.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/range/algorithm/copy.hpp>

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
            std::cout << loc << '\n';
            min = (loc < min) ? loc : min;
#if PRINT_COUNT_100000
            if (++count % 100000 == 0) { std::cout << "COUNT: " << count << '\n'; }
#endif
        }
    }
    return min;
}

int64_t mapValue(int64_t input, int64_t input_start, int64_t input_end, int64_t output_start, int64_t output_end) {
    return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
}

template<typename T>
int64_t run2Intervals(T input) {
    int64_t minOut = INT64_MAX;
    typedef boost::icl::interval_set<int64_t> set_t;
    typedef set_t::interval_type ival;

    std::vector<std::pair<int64_t, int64_t>> seedRanges;
    for (int i = 0; i < input.seeds.size() - 1; i+=2) {
        seedRanges.emplace_back(std::make_pair(input.seeds[i], input.seeds[i] + input.seeds[i + 1]));
    }

    std::vector<std::string> mapNames = {"seed-to-soil map:",
                                        "soil-to-fertilizer map:",
                                        "fertilizer-to-water map:",
                                        "water-to-light map:",
                                        "light-to-temperature map:",
                                        "temperature-to-humidity map:",
                                        "humidity-to-location map:"};
    /*std::vector<std::string> mapNames = { "seed-to-soil map:" };*/

    for (auto& sr : seedRanges) {
        std::cout << "================================NEW SEED RANGE==================================\n";
        //std::cout << sr.first << " " << sr.second << '\n';

        boost::icl::interval_map<int64_t, std::set<std::string>> is;
        is += std::make_pair(boost::icl::interval<int64_t>::right_open(sr.first, sr.second), std::set<std::string>{"input"});
        
        for (const std::string& mn : mapNames) {
            /*std::cout << "INPUT MAP: \n";
            for (auto& interval : is) {
                std::cout << interval.first.lower() << ", " << interval.first.upper() << " -> ";
                for (auto& s : interval.second) {
                    std::cout << "{" << s << "} ";
                }
                std::cout << '\n';
            }*/
            
            //std::cout << mn << '\n';
            int i = 0;
            for (const auto& [ss, ms, l] : input.maps.at(mn)) {
                //std::cout << ms << ' ' << ss << ' ' << l << '\n';
                is += std::make_pair(boost::icl::interval<int64_t>::right_open(ss, ss + l), std::set<std::string>{"map" + std::to_string(i++)});
            }

            /*std::cout << "INTERVALS: \n";
            for (auto& interval : is) {
                std::cout << interval.first.lower() << ", " << interval.first.upper() << " -> ";
                for (auto& s : interval.second) {
                    std::cout << "{" << s << "} ";
                }
                std::cout << '\n';
            }*/

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

            /*std::cout << "CLEANED MAP: \n";
            for (auto& interval : is) {
                std::cout << interval.first.lower() << ", " << interval.first.upper() << " -> ";
                for (auto& s : interval.second) {
                    std::cout << "{" << s << "} ";
                }
                std::cout << '\n';
            }*/

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

            if (mn == "humidity-to-location map:") {
                std::cout << "OUTPUT MAP: \n";
                for (auto& interval : is) {
                    if (interval.first.lower() < minOut) { minOut = interval.first.lower(); }
                    std::cout << interval.first.lower() << ", " << interval.first.upper() << " -> ";
                    for (auto& s : interval.second) {
                        std::cout << "{" << s << "} ";
                    }
                    std::cout << '\n';
                }
            }


        }

        std::cout << "================================================================================\n";
    }

    return minOut;
}

void mapVals(boost::icl::interval_set<int>& s, auto input, auto from, auto to) {
    //s.add(input);
    s.subtract(from);

    auto inscrIn = input & from;

    s.add(boost::icl::interval<int>::right_open((to.lower() - from.lower()) + inscrIn.lower(), (to.upper() - from.upper()) + inscrIn.upper()));
}


template<typename T>
int run2Intervals2(T input) {
    std::vector<std::pair<int64_t, int64_t>> seedRanges;
    for (int i = 0; i < input.seeds.size() - 1; i += 2) {
        seedRanges.emplace_back(std::make_pair(input.seeds[i], input.seeds[i] + input.seeds[i + 1]));
    }

    std::vector<std::string> mapNames = { "seed-to-soil map:",
                                        "soil-to-fertilizer map:",
                                        "fertilizer-to-water map:",
                                        "water-to-light map:",
                                        "light-to-temperature map:",
                                        "temperature-to-humidity map:",
                                        "humidity-to-location map:" };

    //Loop over all map names
    for (auto& mn : mapNames) {
        //Loop over all mapping ranges
        for (auto& [from, to, length] : input.maps.at(mn)) {




        }
    }

}



int main(int argc, char **argv) {
    auto input = getInput();

    //input.print();

    {
        //std::cout << "Part 1: " << run1(input) << '\n';
        //std::cout << "Part 2: " << run2(input) << '\n';


        typedef boost::icl::interval_set<int> IntSet;
        IntSet input_range{ {79, 93} };
        IntSet mapping_range{ {50, 98} };
        IntSet mapped_range{ {52, 100} };

        // Subtract the mapping range from the input range
        input_range -= mapping_range;

        // Add the mapped range to the input range
        input_range += mapped_range;

        // Output the resulting ranges
        std::vector<int> output;
        for (const auto& interval : input_range) {
            output.push_back(interval.lower());
            output.push_back(interval.upper());
        }

        for (int i = 0; i < output.size() - 1; i += 2) {
            std::cout << output[i] << ", " << output[i + 1] << '\n';
        }

        boost::icl::interval_map<int, std::set<std::string>> is;
        is += std::make_pair(boost::icl::interval<int>::right_open(79, 94), std::set<std::string>{"input"});
        is += std::make_pair(boost::icl::interval<int>::right_open(50, 99), std::set<std::string>{"maps"});
        std::cout << "------------------\n";
        for (auto& interval : is) {
            std::cout << interval.first.lower() << ", " << interval.first.upper() << " -> ";
            for (auto& s : interval.second) {
                std::cout << "{" << s << "} ";
            }
            std::cout << '\n';
        }

        std::cout << "\n\n------------------\n";

        std::cout << run2Intervals(input) << '\n';
    }
    std::cout << "\n\n------------------\n";


    typedef boost::icl::interval_map<double, double> map_t;
    typedef boost::icl::interval_map<double, double>::interval_type ival;

    // Define the input range and the two mapping ranges
    map_t input_map;
    input_map.add(std::make_pair(ival::closed(1.0, 5.0), 1));

    map_t mapping_map1;
    mapping_map1.add(std::make_pair(ival::closed(2.0, 3.0), 2));

    map_t mapping_map2;
    mapping_map2.add(std::make_pair(ival::closed(4.0, 5.0), 3));

    // Apply the mappings to the input range
    input_map += mapping_map1;
    input_map += mapping_map2;

    // Print the resulting map
    for (auto& element : input_map) {
        std::cout << element.first << " -> " << element.second << "\n";
    }

    std::cout << "\n\n------------------\n";

    boost::icl::interval_set<int> s;
    boost::icl::interval_set<int> fi;
    
    auto inpt = boost::icl::interval<int>::right_open(1, 10);
    auto fromInterval = boost::icl::interval<int>::right_open(2, 5);
    auto toInterval = boost::icl::interval<int>::right_open(22, 25);

    /*s.add(inpt);
    s.subtract(fromInterval);

    auto insctInterval = inpt & fromInterval;

    for (auto& in : s) {
        std::cout << in << '\n';
    }
    std::cout << '\n';
    
    std::cout << insctInterval << '\n';

    std::cout << '\n';
    s.add(boost::icl::interval<int>::right_open((toInterval.lower() - fromInterval.lower()) + insctInterval.lower(), (toInterval.upper() - fromInterval.upper()) + insctInterval.upper()));
    */

    s.add(inpt);
    mapVals(s, inpt, fromInterval, toInterval);

    for (auto& in : s) {
        std::cout << in << '\n';
    }

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