#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <execution>
#include <chrono>
#include <algorithm>

struct Map {
    std::vector<char> map;
    int width = -1;
    int height = -1;

    void print() {
        std::cout << "\n---------------------------------------------\n";
        std::cout << "WIDTH: " << width << " HEIGHT: " << height << '\n';
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

    int j = 0;
    Map m;
    while (std::getline(file, linetxt)) {
        m.width = linetxt.length() + 2;
        m.map.emplace_back('#');
        for (auto c : linetxt) {
            m.map.emplace_back(c);
        }
        m.map.emplace_back('#');
        j++;
    }
    std::vector<char> top(m.width, '#');
    m.map.insert(m.map.begin(), top.begin(), top.end());
    m.map.insert(m.map.end(), top.begin(), top.end());
    m.height = j + 2;
    return m;
}

void moveNorth(Map& m) {
    int x = 0;
    int y = m.height - 1;

    for (int x = 0; x < m.width; x++) {
        int numRound = 0;
        for (int y = m.height - 1; y > -1; y--) {
            int index = y * m.width + x;
            if (m.map[index] == 'O') { m.map[index] = '.'; numRound++; }
            else if (m.map[index] == '#') {
                for (int j = y + 1; j < y + 1 + numRound; j++) {
                    m.map[j * m.width + x] = 'O';
                }
                numRound = 0;
            }
        }

    }
}

void moveSouth(Map& m) {
    int x = 0;
    int y = 0;

    for (int x = 0; x < m.width; x++) {
        int numRound = 0;
        for (int y = 0; y < m.height; y++) {
            int index = y * m.width + x;
            if (m.map[index] == 'O') { m.map[index] = '.'; numRound++; }
            else if (m.map[index] == '#') {
                for (int j = y - 1; j > y - 1 - numRound; j--) {
                    m.map[j * m.width + x] = 'O';
                }
                numRound = 0;
            }
        }

    }
}

void moveEast(Map& m) {
    int x = 0;
    int y = 0;

    for (int y = 0; y < m.height; y++) {
        int numRound = 0;
        for (int x = 0; x < m.width; x++) {
            int index = y * m.width + x;
            if (m.map[index] == 'O') { m.map[index] = '.'; numRound++; }
            else if (m.map[index] == '#') {
                for (int i = x - 1; i > x - 1 - numRound; i--) {
                    m.map[y * m.width + i] = 'O';
                }
                numRound = 0;
            }
        }

    }
}

void moveWest(Map& m) {
    int x = 0;
    int y = 0;

    for (int y = 0; y < m.height; y++) {
        int numRound = 0;
        for (int x = m.width - 1; x > -1; x--) {
            int index = y * m.width + x;
            if (m.map[index] == 'O') { m.map[index] = '.'; numRound++; }
            else if (m.map[index] == '#') {
                for (int i = x + 1; i < x + 1 + numRound; i++) {
                    m.map[y * m.width + i] = 'O';
                }
                numRound = 0;
            }
        }

    }
}

template<typename T>
int run1(T input) {
    int sum = 0;

    moveNorth(input);
    //input.print();
    for (int x = 0; x < input.width; x++) {
        for (int y = 0; y < input.height; y++) {
            if (input.map[y * input.width + x] == 'O') { sum += input.height - y - 1; }
        }
    }

    return sum;
}

template<typename T>
int run2(T input) {
    int sum = 0;

    std::vector<std::vector<char>> history;

    while (std::find(history.begin(), history.end(), input.map) == history.end()) {
        history.push_back(input.map);
        moveNorth(input);
        moveWest(input);
        moveSouth(input);
        moveEast(input);
    }
    int index = std::find(history.begin(), history.end(), input.map) - history.begin();
    int numIts = (1000000000 - index) % (history.size() - index);

    for (int i = 0; i < numIts; i++) {
        moveNorth(input);
        moveWest(input);
        moveSouth(input);
        moveEast(input);
    }

    for (int i = 0; i < input.map.size(); i++) {
        if (input.map[i] == 'O') {
            int y = i / input.width;
            sum += input.height - y - 1;
        }
    }

    return sum;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GOT INPUT IN: " << inputT << "\n\n";

    //input.print();

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "PART 2 TOOK: " << part2T << "\n\n";

}

//GOT INPUT IN : 172us
//
//Part 1 : 108826
//PART 1 TOOK : 74us
//
//Part 2 : 99291
//PART 2 TOOK : 43ms
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 53
//Ticks : 531869
//TotalDays : 6.1558912037037E-07
//TotalHours : 1.47741388888889E-05
//TotalMinutes : 0.000886448333333333
//TotalSeconds : 0.0531869
//TotalMilliseconds : 53.1869