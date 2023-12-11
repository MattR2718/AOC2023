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
#include <chrono>


auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::string instructions;
    std::vector<std::string> out;
    std::vector<bool> blankRows;
    std::vector<std::pair<int, int>> galaxies;

    int j = 0;
    while (std::getline(file, linetxt)) {
        out.push_back(linetxt);
        blankRows.push_back(true);

        for (int i = 0; i < linetxt.length(); i++) {
            if (linetxt[i] == '#') {
                galaxies.push_back(std::make_pair(i, j));
                blankRows[j] = false;
            }
        }
        
        j++;
    }

    return std::make_tuple(out, blankRows, galaxies);
}

auto findBlankCols(const auto& map) {
    std::vector<bool> bc;
    for (int i = 0; i < map[0].length(); i++) {
        bc.push_back(true);
        for (int j = 0; j < map.size(); j++) {
            if (map[j][i] == '#') {
                bc[i] = false;
            }
        }
    }
    return bc;
}


template<typename T>
auto run1(T& input, int64_t blankSize = 1000000) {
    auto [map, blankRows, galaxies] {input};
    auto blankCols = findBlankCols(map);

    int64_t dist1 = 0;
    int64_t dist2 = 0;

    for (int i = 0; i < galaxies.size() - 1; i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            auto [minx, maxx] {std::minmax(galaxies[i].first, galaxies[j].first) };
            auto [miny, maxy] {std::minmax(galaxies[i].second, galaxies[j].second) };
            for (int x = minx + 1; x <= maxx; x++) {
                dist1 += (blankCols[x]) ? 2 : 1;
                dist2 += (blankCols[x]) ? blankSize : 1;
            }
            for (int y = miny + 1; y <= maxy; y++) {
                dist1 += (blankRows[y]) ? 2 : 1;
                dist2 += (blankRows[y]) ? blankSize : 1;
            }
        }
    }
    
    return std::make_pair(dist1, dist2);
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Got input in: " << time << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    auto [p1, p2] {run1(input)};
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 1: " << p1 << "\nPart 2: " << p2 << '\n';
    std::cout << "Part 1 + 2 ran in: " << time << '\n';
}

//Got input in : 249us
//
//Part 1 : 10276166
//Part 2 : 598693078798
//Part 1 + 2 ran in : 11213us
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 19
//Ticks : 199061
//TotalDays : 2.30394675925926E-07
//TotalHours : 5.52947222222222E-06
//TotalMinutes : 0.000331768333333333
//TotalSeconds : 0.0199061
//TotalMilliseconds : 19.9061