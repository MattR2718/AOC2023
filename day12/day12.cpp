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

struct Row {
    std::string record;
    std::string record2;
    std::vector<int> groups;
    std::vector<int> groups2;

    void print() {
        std::cout << "-------------------------------------------\nROW: [" << record << "]\nDAMAGED: [";
        for (const auto& n : groups) {
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
        std::cout << r.record << " - " << linetxt << '\n';

        for (const auto& m : ctre::search_all<"(-?\\d+)">(linetxt)) {
            r.groups.emplace_back(m.to_number<int>());
        }

        r.record2 = r.record + "?" + r.record;
        r.groups2 = r.groups;
        r.groups2.insert(r.groups2.end(), r.groups.begin(), r.groups.end());

        out.push_back(r);
    }

    return out;
}

std::vector<std::string> split(const std::string& s) {

    std::string singleGaps = "";
    char prevC = '.';
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != '.' || (s[i] == '.' && s[i] != prevC)) {
            singleGaps += s[i];
        }
        prevC = s[i];
    }




    //std::cout << singleGaps << '\n';

    std::vector<std::string> splitStr;
    for (const auto& m : ctre::search_all<"[#?]+">(singleGaps)) {
        splitStr.emplace_back(m.to_string());
    }
    

    return splitStr;
}

int64_t generatePossibilities(std::string s, std::vector<int> group, int64_t& sum, std::map<std::string, int64_t>& cache) {
    if (cache.contains(s)) { return cache[s]; }
    if (s.length() == 0 && group.size() != 0) { return 0; }
    if (s.length() == 0 && group.size() == 0) { return 1; }
    if (s[0] == '.') {
        cache[s] = generatePossibilities(s.substr(1, s.length() - 1), group, sum, cache);
    }
    else if(s[0] == '?') {
        std::string s1 = s;
        s1[0] = '.';
        cache[s1] = generatePossibilities(s1, group, sum, cache);
        s1[0] = '#';
        cache[s1] = generatePossibilities(s1, group, sum, cache);
    }
    else if (s[0] == '#') {
        if (s.length() >= group[0]) {
            std::string firstG = s.substr(0, group[0]);
            if (firstG.find_first_not_of('#') == std::string::npos) {
                s.erase(0, group[0]);
                group.erase(group.begin());
                cache[s] = generatePossibilities(s, group, sum, cache);
            }
        }
    }
}

std::string removeDuplicateDots(std::string str) {
    std::string::iterator new_end = std::unique(str.begin(), str.end(), [](auto a, auto b) { return a == b && a == '.'; });
    str.erase(new_end, str.end());
    return str;
}

void generateOptions(std::string s, std::vector<std::string>& strings, int numSprings) {
    s = removeDuplicateDots(s);
    if (s.find_first_of("?") == std::string::npos) { 
        if (std::ranges::count(s, '#') == numSprings) {
            strings.push_back(s); return;
        }
        return;
    
    }

    s = removeDuplicateDots(s);

    int i = 0;
    while (i < s.length() && s[i] != '?') { i++; }
    if (i == s.length()) { std::cout << "ERROR: FOUND NO ? BUT DIDNT EXIT AT START [" << s << "]\n"; }
    s[i] = '.';
    generateOptions(s, strings, numSprings);
    s[i] = '#';
    generateOptions(s, strings, numSprings);
}

int64_t otherRecursiveMethod(std::string s, std::vector<int> groups, std::map<std::string, int64_t>& cache) {
    if (cache.contains(s)) { return cache[s];}
    if (s[0] == '.') {
        cache[s.substr(1, s.length() - 1)] = otherRecursiveMethod(s.substr(1, s.length() - 1), groups, cache);
    }
    if (s[0] == '?') {
        s[0] = '.';
        cache[s] = otherRecursiveMethod(s, groups, cache);
        s[0] = '#';
        cache [s] = otherRecursiveMethod(s, groups, cache);
    }
    if (s[0] == '#') {
        if (s.length() >= groups[0] && std::ranges::count(s.substr(0, groups[0]), '#') == groups[0]) {
            s.erase(0, groups[0]);
            groups.erase(groups.begin());
            cache[s] = otherRecursiveMethod(s, groups, cache);
        }
    }
    
    std::cout << "GOT TO BOTTOM\n";
    return 0;
}

int countConfigurations(std::string s, std::vector<int> nums) {
    int n = s.size();
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1; // base case: one way to arrange an empty string

    for (int i = 0; i < n; i++) {
        if (s[i] == '?') {
            // if current character is '?', it can be either '.' or '#'
            dp[i + 1] = dp[i]; // if it's '.', it doesn't change the number of arrangements
            for (int num : nums) {
                // if it's '#', we need to check if it forms a valid segment
                if (i + 1 >= num && (i + 1 == num || s[i - num] == '.')) {
                    dp[i + 1] += dp[i + 1 - num];
                }
            }
        }
        else {
            // if current character is not '?', it doesn't change the number of arrangements
            dp[i + 1] = dp[i];
        }
    }

    return dp[n];
}


template<typename T>
int64_t run1(T input) {
    int64_t sum = 0;

    for (const Row& r : input) {

        auto v = 0;
                
        std::vector<std::string> options;
        generateOptions(r.record, options, std::accumulate(r.groups.begin(), r.groups.end(), 0));


        //Print all options
        /*std::cout << r.record << '\n';
        for (auto& s : options) {
            std::cout << s << '\n';
        }
        std::cout << "\n\n";*/

        for (auto& s : options) {
            auto spl = split(s);
            if (spl.size() == r.groups.size()) {
                bool works = true;
                for (int j = 0; j < spl.size(); j++) {
                    if (spl[j].length() != r.groups[j]) { works = false; }
                }
                if (works) { v++; }
            }
        }

        std::cout << v << '\n';

        sum += v;

    }

    /*std::cout << "----------------------------\n";
    for (Row& r : input) {
        std::cout << "COUNT: "<<countConfigurations(r.record, r.groups) << '\n';
    }*/

    return sum;
}



template<typename T>
int64_t run2(T input) {
    int64_t sum = 0;

    //for (auto& r : input) {
    //    std::string rec = r.record;
    //    std::vector<int> grps = r.groups;
    //    for (int i = 0; i < 4; i++) {
    //        r.record += "?" + rec;
    //        r.groups.insert(r.groups.end(), grps.begin(), grps.end());
    //    }
    //}

    ///*std::cout << '\n';
    //for (auto& r : input) {
    //    r.print();
    //}*/


    //int curr = 0;
    ////for (const Row& r : input) {

    //std::for_each(std::execution::par, input.begin(), input.end(), [&](Row& r) {

    //    std::cout << curr++ << ": " << r.record << '\n';

    //    auto v = 0;
    //    std::vector<std::string> options;
    //    generateOptions(r.record, options, std::accumulate(r.groups.begin(), r.groups.end(), 0));

    //    for (auto& s : options) {
    //        auto spl = split(s);
    //        if (spl.size() == r.groups.size()) {
    //            bool works = true;
    //            for (int j = 0; j < spl.size(); j++) {
    //                if (spl[j].length() != r.groups[j]) { works = false; }
    //            }
    //            if (works) { v++; }
    //        }

    //    }

    //    sum += v;
    //    });
    std::cout << '\n';
    int curr = 0;
    //int idx = 0;
    //std::vector<int64_t> vals(input.size());
    //std::iota(vals.begin(), vals.end(), 0);
    //for (const Row& r : input) {
    std::vector<int64_t> vals(input.size());
    std::iota(vals.begin(), vals.end(), 0);
    std::for_each(std::execution::par, vals.begin(), vals.end(), [&](const int64_t& idx) {
        Row r = input[idx];
        std::cout << curr++ << " [" << r.record << "] ";
        auto v1 = 0;

        std::vector<std::string> options;
        generateOptions(r.record, options, std::accumulate(r.groups.begin(), r.groups.end(), 0));

        for (auto& s : options) {
            auto spl = split(s);
            if (spl.size() == r.groups.size()) {
                bool works = true;
                for (int j = 0; j < spl.size(); j++) {
                    if (spl[j].length() != r.groups[j]) { works = false; }
                }
                if (works) { v1++; }
            }
        }
        //std::cout << "v1: "<<v1 << '\n';

        auto v2 = 0;
        std::vector<std::string> options2;
        generateOptions(r.record2, options2, std::accumulate(r.groups2.begin(), r.groups2.end(), 0));

        for (auto& s : options2) {
            auto spl = split(s);
            if (spl.size() == r.groups2.size()) {
                bool works = true;
                for (int j = 0; j < spl.size(); j++) {
                    if (spl[j].length() != r.groups2[j]) { works = false; }
                }
                if (works) { v2++; }
            }
        }
        //std::cout << "v2: "<<v2 << '\n';


        /*auto v2 = 0;
        std::vector<std::string> options2;
        generateOptions("?" + r.record, options2, std::accumulate(r.groups.begin(), r.groups.end(), 0));

        for (auto& s : options2) {
            auto spl = split(s);
            if (spl.size() == r.groups.size()) {
                bool works = true;
                for (int j = 0; j < spl.size(); j++) {
                    if (spl[j].length() != r.groups[j]) { works = false; }
                }
                if (works) { v2++; }
            }
        }

        std::cout <<"VALS: "<< v1 << " " << v2 << '\n';*/




        //sum += v1 * std::pow(v2 / v1, 4);
        /*if (v2 % v1 != 0) { 
            std::cout << "POTENTIAL ERROR: " << v2 << " % " << v1 << " != 0. [" << v2 % v1 << "]\n";
            std::cout << "RUNNING MANUAL\n";

            std::string rec = r.record;
            std::vector<int> grps = r.groups;
            for (int i = 0; i < 4; i++) {
                r.record += "?" + rec;
                r.groups.insert(r.groups.end(), grps.begin(), grps.end());
            }
            
            generateOptions(r.record, options, std::accumulate(r.groups.begin(), r.groups.end(), 0));

            for (auto& s : options) {
                auto spl = split(s);
                if (spl.size() == r.groups.size()) {
                    bool works = true;
                    for (int j = 0; j < spl.size(); j++) {
                        if (spl[j].length() != r.groups[j]) { works = false; }
                    }
                    if (works) { vals[idx]++; }
                }
            }
            
        }
        else {
            vals[idx] = v1 * std::pow(v2 / v1, 4);
        }*/
        vals[idx] = v1 * std::pow(v2 / v1, 4);
        }
    );


    return std::accumulate(vals.begin(), vals.end(), 0);
}


int64_t count(std::string s, std::vector<int> g, bool n, std::map<std::tuple<std::string, std::vector<int>, bool>, int64_t>& cache) {
    //If seen this position before, return
    if(cache.contains(std::make_tuple(s, g, n))){ return cache[std::make_tuple(s, g, n)]; }

    //No more groups of springs
    if (g.size() == 0) {
        //If there are still springs left, not a valid configuration
        if (std::ranges::count(s, '#')) {
            cache[std::make_tuple(s, g, n)] = 0;
            return 0;
        }
        //No springs left, valid configuration
        else {
            cache[std::make_tuple(s, g, n)] = 1;
            return 1;
        }
    }
    //No more string left
    else if (s.length() == 0) {
        // If there are still groups of springs, invalid configuration
        if (std::accumulate(g.begin(), g.end(), 0)) {
            cache[std::make_tuple(s, g, n)] = 0;
            return 0;
        }
        //No more springs left to put in, valud configuration
        else {
            cache[std::make_tuple(s, g, n)] = 1;
            return 1;
        }
    }
    //Reached end of hot spring group
    else if (g[0] == 0) {
        //If hot springs don't continue to appear, potentially valid, remove 0-ed hot spring group and recureively call function, removing the first character
        if (s[0] == '?' || s[0] == '.') {
            auto gSave = g;
            g.erase(g.begin());
            auto val = count(s.substr(1, s.length() - 1), g, false, cache);
            cache[std::make_tuple(s, gSave, n)] = val;
            return val;
        }
        //Still hot springs when group empty, invalid configuration
        else {
            cache[std::make_tuple(s, g, n)] = 0;
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
            g[0]++;
            cache[std::make_tuple(s, g, n)] = val;
            return val;
        }
        //Should be hot spring next but blank space found, invalid configuration
        else {
            cache[std::make_tuple(s, g, n)] = 0;
            return 0;
        }
    }
    //If found start of hot spring, recursively call, decreasing the number in group and cutting off first character
    else if (s[0] == '#') {
        g[0]--;
        auto val = count(s.substr(1, s.length() - 1), g, true, cache);
        g[0]++;
        cache[std::make_tuple(s, g, n)] = val;
        return val;
    }
    //If on a blank space, recursively call, cutting off front character
    else if (s[0] == '.') {
        auto val = count(s.substr(1, s.length() - 1), g, false, cache);
        cache[std::make_tuple(s, g, n)] = val;
        return val;
    }
    // Otherwise at ? and dont know what it is, call recursively both options (next is a hot spring and isn't), if invalid sum is zero, if valid then sum is number of configurations
    else {
        auto val1 = count(s.substr(1, s.length() - 1), g, false, cache);
        g[0]--;
        auto val2 = count(s.substr(1, s.length() - 1), g, true, cache);
        g[0]++;
        cache[std::make_tuple(s, g, n)] = val1 + val2;
        return val1 + val2;
    }
}

template<typename T>
int64_t run22(T input) {
    int64_t sum = 0;

    for (auto& r : input) {
        std::string rec = r.record;
        std::vector<int> grps = r.groups;
        for (int i = 0; i < 4; i++) {
            r.record += "?" + rec;
            r.groups.insert(r.groups.end(), grps.begin(), grps.end());
        }
    }

    /*for (auto& r : input) {
        r.print();
    }*/

    std::map<std::tuple<std::string, std::vector<int>, bool>, int64_t> cache;

    for (Row& r : input) {
        r.print();
        auto v = count(r.record, r.groups, false, cache);
        std::cout << "VAL: " << v << '\n';
        sum += v;
    }

    return sum;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    for (auto& r : input) {
        r.print();
    }

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run22(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

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
//Part 2 : 1104
//
//INPUT GOT IN : 7297us
//CTRE INPUT GOT IN : 621us
//PART 1 DONE IN : 601us
//PART 2 DONE IN : 772us
//
//DOING ALL AT ONCE
//PART 1 : 1798691765
//PART 2 : 1104
//RAN IN : 8154us
//
//DOING ALL AT ONCE CTRE
//PART 1 : 1798691765
//PART 2 : 1104
//RAN IN : 2035us
//
//DOING ALL AT ONCE SPLIT
//PART 1 : 1798691765
//PART 2 : 1104
//RAN IN : 2044us
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 34
//Ticks : 345696
//TotalDays : 4.00111111111111E-07
//TotalHours : 9.60266666666667E-06
//TotalMinutes : 0.00057616
//TotalSeconds : 0.0345696
//TotalMilliseconds : 34.5696