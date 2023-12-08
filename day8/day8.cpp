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
int run2(T input) {
    auto [instr, nodes] {input};

    std::vector<std::string> ats;
    std::vector<std::string> starts;
    for (auto& [k, v] : nodes) {
        if (k[2] == 'A') { ats.push_back(k); starts.push_back(k); }
    }
    std::vector<int64_t> loopSizes(ats.size(), -1);
    std::map<std::string, std::vector<std::pair<std::string, char>>> history;
    std::vector<bool> stop(ats.size(), false);

    int i = 0;
    int count = 0;
    //while (std::count_if(ats.begin(), ats.end(), [](const std::string s) { return s[2] == 'Z'; }) < ats.size()) {
    while (true) {
        count++;
        
        for (int j = 0; j < starts.size(); j++) {
            if (ats[j][2] == 'Z') { std::cout << "S: " << starts[j] << " A: " << ats[j] << '\n'; }
        }

        for (int j = 0; j < ats.size(); j++) {
           
            if (!stop[j] && std::find(history[starts[j]].begin(), history[starts[j]].end(), std::make_pair(ats[j], instr[(i % instr.length())])) != history[starts[j]].end()) { /*In loop*/
                loopSizes[j] = history[starts[j]].size() - (std::distance(history[starts[j]].begin(), std::find(history[starts[j]].begin(), history[starts[j]].end(), std::make_pair(ats[j], instr[(i % instr.length())]))));
                
                history[starts[j]].push_back(std::make_pair(ats[j], instr[(i % instr.length())]));

                stop[j] = true;
            }
            if (!stop[j]) { history[starts[j]].push_back(std::make_pair(ats[j], instr[(i % instr.length())])); }
        }
        
        std::for_each(std::execution::par, ats.begin(), ats.end(), [nodes, i, instr](std::string& at) { at = (instr[(i % instr.length())] == 'L') ? nodes.at(at).first : nodes.at(at).second; });
        
        /*std::cout << "[\n";
        for (auto& ls : loopSizes) {
            std::cout << ls << '\n';
        }
        std::cout << "]\n\n";*/
        
        if (std::count_if(stop.begin(), stop.end(), [](const bool& b){ return b; }) == stop.size()) { break; }

        //if (count % 1000 == 0) { std::cout << count << '\n'; }
        i++;
    }

    for (auto& ls : loopSizes) {
        std::cout << ls << '\n';
    }

    std::cout << "--------HISTORIES--------\n";
    for (auto& [k, v] : history) {
        std::cout << k << ": ";
        for (auto& s : v) {
            std::cout << "(" << s.first << ", " << s.second << "), ";
        }
        std::cout << '\n';
    }
    std::cout<< "-------------------------\n";

    for (int k = 0; k < ats.size(); k++) {
        std::cout << "INITIAL: " << history[starts[k]].size() - loopSizes[k] - 1 << "  LOOP SIZE: " << loopSizes[k] << '\n';
    }

    int n = 0;
    int num = 0;
    while (num < starts.size()) {
        num = 0;
        /*for (int k = 0; k < starts.size(); k++) {
            num += (((history[starts[k]].size() - loopSizes[k]) + n * loopSizes[k]) % history[starts[k]].size() == 0) ? 1 : 0;
        }
        n++;*/

        for (int k = 0; k < starts.size(); k++) {
            num += ((n - (history[starts[k]].size() - (history[starts[k]].size() - loopSizes[k]))) % loopSizes[k] == 0) ? 1 : 0;
        }
        n++;
        if (n % 100000 == 0) { std::cout <<"n: "<<  n << '\n'; }
    }



    std::cout << "N: " << --n << '\n';

    for (int k = 0; k < starts.size(); k++) {
        std::cout << "[" << (history[starts[k]].size() - loopSizes[k]) + n * loopSizes[k] << "]\n";
    }

    return count;
}


int main() {
    auto input = getInput();

    /*for (auto& h : input) {
        h.print();
    }*/

    //std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}


// not 47,634,422,309