#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <execution>
#include <chrono>
#include <algorithm>

struct Row {
    std::string record;
    std::string record5;
    std::vector<int> groups;
    std::vector<int> groups5;

    void print() {
        std::cout << "-------------------------------------------\nROW: [" << record << "]\nDAMAGED: [";
        for (const auto& n : groups) {
            std::cout << n << " ";
        }
        std::cout << "\n" << record5 << '\n';
        for (const auto& n : groups5) {
            std::cout << n << " ";
        }
        std::cout << "]\n-------------------------------------------\n";
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Row> out;

    while (std::getline(file, linetxt)) {
        Row r;
        
        int space = linetxt.find(" ");
        r.record = linetxt.substr(0, space);
        linetxt.erase(0, space + 1);

        int pos = 0;
        while ((pos = linetxt.find(",")) != std::string::npos) {
            r.groups.emplace_back(std::stoi(linetxt.substr(0, pos)));
            linetxt.erase(0, pos + 1);
        }

        r.groups.emplace_back(std::stoi(linetxt.substr(0, pos)));

        r.record5 = r.record;
        r.groups5 = r.groups;

        for (int i = 0; i < 4; i++) {
            r.record5 += "?" + r.record;
            r.groups5.insert(r.groups5.end(), r.groups.begin(), r.groups.end());
        }

        out.push_back(r);
    }

    return out;
}

int64_t count(std::string s, std::vector<int> g, bool n, std::map<std::tuple<std::string, std::vector<int>, bool>, int64_t>& cache) {
    auto tup = std::make_tuple(s, g, n);
    
    //If seen this position before, return
    //if (cache.contains(std::make_tuple(s, g, n))) { return cache[std::make_tuple(s, g, n)]; }
    if (cache.contains(tup)) { return cache[tup]; }

    //No more groups of springs
    if (g.size() == 0) {
        //If there are still springs left, not a valid configuration
        if (std::ranges::count(s, '#')) {
            cache[tup] = 0;
            return 0;
        }
        //No springs left, valid configuration
        else {
            cache[tup] = 1;
            return 1;
        }
    }
    //No more string left
    else if (s.length() == 0) {
        // If there are still groups of springs, invalid configuration
        if (std::accumulate(g.begin(), g.end(), 0)) {
            cache[tup] = 0;
            return 0;
        }
        //No more springs left to put in, valud configuration
        else {
            cache[tup] = 1;
            return 1;
        }
    }
    //Reached end of hot spring group
    else if (g[0] == 0) {
        //If hot springs don't continue to appear, potentially valid, remove 0-ed hot spring group and recureively call function, removing the first character
        if (s[0] == '?' || s[0] == '.') {
            g.erase(g.begin());
            auto val = count(s.substr(1, s.length() - 1), g, false, cache);
            cache[tup] = val;
            return val;
        }
        //Still hot springs when group empty, invalid configuration
        else {
            cache[tup] = 0;
            return 0;
        }
    }
    // Next is a hot spring
    else if (n) {
        // If possibly a hot spring, potentially valid, decrease hot spring group and recursively call function, cutting off first character
        if (s[0] == '?' || s[0] == '#') {
            g[0]--;
            //In hot spring so n set to true for next call, since expecting anothe hot spring, group is decreased so will find end
            auto val = count(s.substr(1, s.length() - 1), g, true, cache);
            cache[tup] = val;
            return val;
        }
        //Should be hot spring next but blank space found, invalid configuration
        else {
            cache[tup] = 0;
            return 0;
        }
    }
    //If found start of hot spring, recursively call, decreasing the number in group and cutting off first character
    else if (s[0] == '#') {
        g[0]--;
        auto val = count(s.substr(1, s.length() - 1), g, true, cache);
        cache[tup] = val;
        return val;
    }
    //If on a blank space, recursively call, cutting off front character
    else if (s[0] == '.') {
        auto val = count(s.substr(1, s.length() - 1), g, false, cache);
        cache[tup] = val;
        return val;
    }
    // Otherwise at ? and dont know what it is, call recursively both options (next is a hot spring and isn't), if invalid sum is zero, if valid then sum is number of configurations
    else {
        auto val1 = count(s.substr(1, s.length() - 1), g, false, cache);
        g[0]--;
        auto val2 = count(s.substr(1, s.length() - 1), g, true, cache);
        cache[tup] = val1 + val2;
        return val1 + val2;
    }
}


template<typename T>
int64_t run1(T input) {
    int64_t sum = 0;

    for (Row& r : input) {
        std::map<std::tuple<std::string, std::vector<int>, bool>, int64_t> cache;
        auto v = count(r.record, r.groups, false, cache);
        sum += v;
    }

    return sum;
}



template<typename T>
int64_t run2(T input) {
    int64_t sum = 0;

    for (Row& r : input) {
        std::map<std::tuple<std::string, std::vector<int>, bool>, int64_t> cache;
        auto v = count(r.record5, r.groups5, false, cache);
        sum += v;
    }
    return sum;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GOT INPUT IN: " << inputT << "\n\n";

    //std::sort(std::execution::par_unseq, input.begin(), input.end(), [](const Row& a, const Row& b) { return a.record > b.record; });

    /*for (auto& r : input) {
        r.print();
    }*/

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "PART 2 TOOK: " << part2T << "\n\n";

}

//GOT INPUT IN : 1464us
//
//Part 1 : 7025
//PART 1 TOOK : 28ms
//
//Part 2 : 11461095383315
//PART 2 TOOK : 496ms
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 535
//Ticks : 5356115
//TotalDays : 6.19920717592593E-06
//TotalHours : 0.000148780972222222
//TotalMinutes : 0.00892685833333333
//TotalSeconds : 0.5356115
//TotalMilliseconds : 535.6115