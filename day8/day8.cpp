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

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::string instructions;
    std::map<std::string, std::pair<std::string, std::string>> out;

    std::regex ex("([\\w]+) = \\(([\\w]+), ([\\w]+)\\)");
    std::smatch m;

    std::getline(file, instructions);
    std::getline(file, linetxt);

    while (std::getline(file, linetxt)) {
        std::regex_match(linetxt, m, ex);
        out[std::string(m[1])] = std::make_pair(std::string(m[2]), std::string(m[3]));
    }

    return std::make_pair(instructions, out);
}


template<typename T>
int run1(T input) {
    auto [instr, nodes] {input};

    std::string at = "AAA";
    int i = 0;
    int count = 0;
    while (at != "ZZZ") {
        count++;
        at = (instr[(i++ % instr.length())] == 'L') ? nodes.at(at).first : nodes.at(at).second;
    }
    
    return count;
}



template<typename T>
int64_t run2(T input) {
    auto [instr, nodes] {input};

    std::vector<std::string> ats;
    std::vector<std::string> starts;
    for (auto& [k, v] : nodes) {
        if (k[2] == 'A') { ats.push_back(k); starts.push_back(k); }
    }
    
    // Calculate length of all cycles, only works because cycles are contained
    std::vector<long long> lengths(starts.size(), 0);
    std::iota(lengths.begin(), lengths.end(), 0);

    std::for_each(std::execution::par, lengths.begin(), lengths.end(), [&](auto& len) {
        
        long long l = 0;
        int ins = 0;

        while (ats[len][2] != 'Z') {
            if (instr[ins] == 'L') { ats[len] = nodes[ats[len]].first; }
            else { ats[len] = nodes[ats[len]].second; }
            l++;
            ins = (ins + 1) % instr.length();
        }
        len = l;

        });

    // Calculate lcm of cycle lengths
    return std::accumulate(lengths.begin(), lengths.end(), 1LL, [](auto l, auto r) { return std::lcm(l, r); });
}


int main() {
    auto input = getInput();

    /*for (auto& h : input) {
        h.print();
    }*/

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

//Part 1: 12083
//Part 2: 13385272668829
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 15
//Ticks : 153538
//TotalDays : 1.77706018518519E-07
//TotalHours : 4.26494444444444E-06
//TotalMinutes : 0.000255896666666667
//TotalSeconds : 0.0153538
//TotalMilliseconds : 15.3538