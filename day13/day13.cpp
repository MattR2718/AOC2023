#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <execution>
#include <chrono>
#include <algorithm>
#include <set>

struct Map {
    std::vector<char> map;
    int width = -1;
    int height = -1;

    void print() {
        std::cout << "\n---------------------------------------------\n";
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                std::cout << map[j * width + i];
            }
            std::cout << '\n';
        }
        std::cout << "---------------------------------------------\n";

    }
};




auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Map> out;

    int j = 0;
    Map m;
    while (std::getline(file, linetxt)) {
        if (linetxt.length() > 1) {
            for (int i = 0; i < linetxt.size(); i++) {
                m.map.emplace_back(linetxt[i]);
            }
            m.width = linetxt.length();
            j++;
        }else {
            m.height = j;
            out.push_back(m);
            m.map.clear();
            m.width = -1;
            m.height = -1;
            j = 0;
        }
    }
    m.height = j;
    out.push_back(m);
    return out;
}

//Check if given row is same as row below
bool checkRows(const Map& m, int row, int offset) {
    if (row + offset >= m.height) { return true; }
    if (row - offset == -1) { return true; }
    for (int i = 0; i < m.width; i++) {
        if (m.map[(row - offset) * m.width + i] != m.map[(row + 1 + offset) * m.width + i]) { return false; }
    }
    return true;

}

//Check if given row is same as row below
bool checkColumns(const Map& m, int col, int offset) {
    if (col + offset >= m.width) { return true; }
    if (col - offset == -1) { return true; }
    for (int j = 0; j < m.height; j++) {
        if (m.map[j * m.width + (col - offset)] != m.map[j * m.width + (col + 1 + offset)]) { return false; }
    }
    return true;

}


int64_t calculateScore(Map& m) {
    //m.print();
    int64_t sum = 0;
    //Check horizontal
    for (int j = 0; j < m.height - 1; j++) {
        int offset = 0;
        bool fine = false;
        while (fine = checkRows(m, j, offset)) {
            offset++;
            if (j + offset == m.height - 1 || j - offset == -1) { break; }
        }
        if (fine) {
           sum += 100 * (j + 1);
           break;
        }
    }

    //Check vertical
    for (int i = m.width - 2; i > -1; i--) {
        int offset = 0;
        bool fine = false;
        while (fine = checkColumns(m, i, offset)) {
            offset++;
            if (i + offset >= m.width - 1 || i - offset < 0) { break; }
        }
        if (fine) { 
            sum += (i + 1);
            break;
        }
    }
    return sum;
}

template<typename T>
int64_t run1(T input) {
    int64_t sum = 0;

    for (auto& m : input) {
        int val = calculateScore(m);

        sum += val;
    }


    return sum;
}

std::pair<int64_t, std::pair<int, int>> calculateScore2(Map& m, std::pair<int, int> original = {-1, -1}) {
    int64_t sum = 0;
    int j = 0;
    int i = 0;
    //Check horizontal
    for (j = 0; j < m.height - 1; j++) {
        int offset = 0;
        bool fine = false;
        while (fine = checkRows(m, j, offset)) {
            offset++;
            if (j + offset == m.height - 1 || j - offset == -1) { break; }
        }
        if (fine && j != original.second) {
            sum += 100 * (j + 1);
            break;
        }
    }

    //Check vertical
    for (i = 0; i < m.width - 1; i++) {
        int offset = 0;
        bool fine = false;
        while (fine = checkColumns(m, i, offset)) {
            offset++;
            if (i + offset >= m.width - 1 || i - offset < 0) { break; }
        }
        if (fine && i != original.first) {
            sum += (i + 1);
            break;
        }
    }
    return { sum, {i, j} };
}


template<typename T>
int64_t run2(T input) {
    int64_t sum = 0;

    std::vector<int> scores(input.size());
    std::iota(scores.begin(), scores.end(), 0);

    std::for_each(std::execution::par_unseq, scores.begin(), scores.end(), [&](int& score) {
        Map m = input[score];

        //for (auto& m : input) {
        auto orig = m;

        std::pair<int64_t, std::pair<int, int>> newlines = { 0, {-1, -1} };
        std::pair<int64_t, std::pair<int, int>> originallines = calculateScore2(orig);

        std::vector< std::pair<int, std::pair<int64_t, std::pair<int, int>>>> allNewOptions;

        for (int i = 0; i < m.map.size(); i++) {
            if (m.map[i] == '#') {
                m.map[i] = '.';
                newlines = calculateScore2(m, originallines.second);

                m.map[i] = '#';
            }
            else if (m.map[i] == '.') {
                m.map[i] = '#';
                newlines = calculateScore2(m, originallines.second);
                m.map[i] = '.';
            }
            if (newlines.second != originallines.second && !(newlines.second.first == (m.width - 1) && newlines.second.second == (m.height - 1))) { /*std::cout << i << " ";*/  allNewOptions.emplace_back(std::make_pair(i, newlines)); }
        }

        int scr = 0;
        if (allNewOptions.size()) {
            newlines = allNewOptions[0].second;
        }

        if (newlines.second.first < (m.width - 1) && newlines.second.first != originallines.second.first) { scr += newlines.second.first + 1; }
        if (newlines.second.second < (m.height - 1) && newlines.second.second != originallines.second.second) { scr += (newlines.second.second + 1) * 100; }

        //sum += scr;
        score = scr;
        });

    return std::reduce(std::execution::par, scores.cbegin(), scores.cend());
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GOT INPUT IN: " << inputT << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "PART 2 TOOK: " << part2T << "\n\n";

}

//GOT INPUT IN : 257us
//
//Part 1 : 29213
//PART 1 TOOK : 45us
//
//Part 2 : 37453
//PART 2 TOOK : 862us
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 9
//Ticks : 98872
//TotalDays : 1.14435185185185E-07
//TotalHours : 2.74644444444444E-06
//TotalMinutes : 0.000164786666666667
//TotalSeconds : 0.0098872
//TotalMilliseconds : 9.8872