#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <map>
#include <numeric>
#include <regex>
#include <execution>
#include <chrono>

#include <boost/iostreams/device/mapped_file.hpp>

//Memory map input, faster than getline
auto getInputMMAP(const std::string file = "input.txt") {
    boost::iostreams::mapped_file mmap(file, boost::iostreams::mapped_file::readonly);
    auto f = mmap.const_data();
    auto l = f + mmap.size();

    std::vector<std::vector<int>> blankRows(2);
    std::vector<std::vector<int>> blankCols(2);
    std::vector<std::pair<int, int>> galaxies;
    int j = 0;

    std::string linetxt;
    std::vector<std::string> lines;
    const char* start_of_line = f;
    while (f && f != l) {
        if ((f = static_cast<const char*>(memchr(f, '\n', l - f))))
        {
            linetxt = std::string(start_of_line, f - start_of_line);
            linetxt.erase(std::remove(linetxt.begin(), linetxt.end(), '\r'), linetxt.end());

            blankRows[0].push_back(2);
            blankRows[1].push_back(1000000);

            for (int i = 0; i < linetxt.length(); i++) {

                blankCols[0].push_back(2);
                blankCols[1].push_back(1000000);

                if (linetxt[i] == '#') {
                    galaxies.push_back(std::make_pair(i, j));
                    blankRows[0][j] = 1;
                    blankCols[0][i] = 1;
                    blankRows[1][j] = 1;
                    blankCols[1][i] = 1;
                }
            }
            j++;


            lines.push_back(linetxt);
            start_of_line = ++f;
        }
    }

    if (start_of_line != l) {
        linetxt = std::string(start_of_line, l - start_of_line);
        linetxt.erase(std::remove(linetxt.begin(), linetxt.end(), '\r'), linetxt.end());

        blankRows[0].push_back(2);
        blankRows[1].push_back(1000000);

        for (int i = 0; i < linetxt.length(); i++) {

            blankCols[0].push_back(2);
            blankCols[1].push_back(1000000);

            if (linetxt[i] == '#') {
                galaxies.push_back(std::make_pair(i, j));
                blankRows[0][j] = 1;
                blankCols[0][i] = 1;
                blankRows[1][j] = 1;
                blankCols[1][i] = 1;
            }
        }
    }

    return std::make_tuple(lines, blankRows, blankCols, galaxies);
}

template<typename T>
auto run1(T& input) {
    auto [map, blankRows, blankCols, galaxies] {input};

    int64_t dist1 = 0;
    int64_t dist2 = 0;

    std::vector<int64_t> cumBlankCols0(blankCols[0].size(), 0);
    std::vector<int64_t> cumBlankCols1(blankCols[1].size(), 0);
    std::vector<int64_t> cumBlankRows0(blankRows[0].size(), 0);
    std::vector<int64_t> cumBlankRows1(blankRows[1].size(), 0);

    // Precompute cumulative sums to remove triply nested loop
    for (int i = 1; i < blankCols[0].size(); i++) {
        cumBlankCols0[i] = cumBlankCols0[i - 1] + blankCols[0][i];
        cumBlankCols1[i] = cumBlankCols1[i - 1] + blankCols[1][i];
    }
    for (int i = 1; i < blankRows[0].size(); i++) {
        cumBlankRows0[i] = cumBlankRows0[i - 1] + blankRows[0][i];
        cumBlankRows1[i] = cumBlankRows1[i - 1] + blankRows[1][i];
    }

    for (int i = 0; i < galaxies.size() - 1; i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            auto [minx, maxx] {std::minmax(galaxies[i].first, galaxies[j].first) };
            auto [miny, maxy] {std::minmax(galaxies[i].second, galaxies[j].second) };

            dist1 += cumBlankCols0[maxx] - cumBlankCols0[minx];
            dist1 += cumBlankRows0[maxy] - cumBlankRows0[miny];

            dist2 += cumBlankCols1[maxx] - cumBlankCols1[minx];
            dist2 += cumBlankRows1[maxy] - cumBlankRows1[miny];
        }
    }

    return std::make_pair(dist1, dist2);
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInputMMAP();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Got input in: " << time << "\n\n";

    /*auto start = std::chrono::high_resolution_clock::now();
    auto input = getInputMMAP();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Got input in: " << time << "\n\n";*/

    start = std::chrono::high_resolution_clock::now();
    auto [p1, p2] {run1(input)};
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 1: " << p1 << "\nPart 2: " << p2 << '\n';
    std::cout << "Part 1 + 2 ran in: " << time << '\n';
}

//Got input in : 386us
//
//Part 1 : 10276166
//Part 2 : 598693078798
//Part 1 + 2 ran in : 522us
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 13
//Ticks : 130023
//TotalDays : 1.50489583333333E-07
//TotalHours : 3.61175E-06
//TotalMinutes : 0.000216705
//TotalSeconds : 0.0130023
//TotalMilliseconds : 13.0023