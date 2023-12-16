#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <execution>
#include <chrono>
#include <algorithm>
#include <set>
#include <ranges>

enum class Direction {
    UP, RIGHT, DOWN, LEFT
};

struct Beam {
    int x;
    int y;
    Direction dir;

    //std::vector<std::tuple<int, int, Direction>> history;

    void print() {
        std::cout << "X: " << x << " Y: " << y << " DIR: ";
        switch (dir) {
        case Direction::UP: std::cout << "UP\n"; break;
        case Direction::DOWN: std::cout << "DOWN\n"; break;
        case Direction::LEFT: std::cout << "LEFT\n"; break;
        case Direction::RIGHT: std::cout << "RIGHT\n"; break;
        default: std::cout << "ERROR\n";
        }
    }
};

struct Map {
    std::vector<char> map;
    int width = -1;
    int height = -1;

    void print() {
        std::cout << "-------------------------------------\n";
        std::cout << "WIDTH: " << width << " HEIGHT: " << height << '\n';
        int i = 0;
        for (int x = 0; x < map.size(); x++) {
            std::cout << map[x];
            if (++i == width) {
                i = 0;
                std::cout << '\n';
            }
        }
        std::cout << "-------------------------------------\n";
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;

    Map m;
    int j = 0;
    while (std::getline(file, linetxt)) {
        j++;
        for (auto& c : linetxt) {
            m.map.push_back(c);
        }
    }
    m.width = linetxt.length();
    m.height = j;
    return m;
}

template<typename T>
int runPos(T m, std::tuple<int, int, Direction> tup) {
    auto& [x, y, dir] {tup};
    switch (dir) {
    case Direction::UP: y++; break;
    case Direction::DOWN: y--; break;
    case Direction::RIGHT: x--; break;
    case Direction::LEFT: x++; break;
    }
    
    std::vector<Beam> beams{Beam{x, y, dir}};
    std::vector<Beam> oldbeams;
    
    std::set<std::tuple<int, int, Direction>> history;
    std::set<std::pair<int, int>> historyNoDir;

    int numInLoop = 0;
    int i = 0;
    while (beams.size()) {
        for (int bi = 0; bi < beams.size(); bi++) {
            //If seen this exact position before, exit
            if(history.contains(std::make_tuple(beams[bi].x, beams[bi].y, beams[bi].dir))){
                beams.erase(beams.begin() + bi);
                bi--;
            }
            else { //Otherwise work out what next position is
                history.emplace(std::make_tuple(beams[bi].x, beams[bi].y, beams[bi].dir));
                historyNoDir.emplace(std::make_pair(beams[bi].x, beams[bi].y));
                switch (beams[bi].dir) {
                case Direction::UP: {
                    if (beams[bi].y - 1 <= -1) { // At top of map
                        break;
                    }
                    else if (m.map[(beams[bi].y - 1) * m.width + beams[bi].x] == '.') { // Position above is empty
                        beams[bi].y--;
                    }
                    else if (m.map[(beams[bi].y - 1) * m.width + beams[bi].x] == '|') { //Position above is effectively empty
                        beams[bi].y--;
                    }
                    else if (m.map[(beams[bi].y - 1) * m.width + beams[bi].x] == '-') { //Position above is splitter, create copy and send in opposite directions
                        beams[bi].y--;
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::LEFT;
                        b2.dir = Direction::RIGHT;
                        beams.push_back(b2);
                    }
                    else if (m.map[(beams[bi].y - 1) * m.width + beams[bi].x] == '/') { //Position above is mirror to right
                        beams[bi].y--;
                        beams[bi].dir = Direction::RIGHT;
                    }
                    else if (m.map[(beams[bi].y - 1) * m.width + beams[bi].x] == '\\') { //Position above is mirror left
                        beams[bi].y--;
                        beams[bi].dir = Direction::LEFT;
                    }
                    break;
                }
                case Direction::RIGHT: {
                    if (beams[bi].x + 1 >= m.width) { //At right hand side of map
                        break;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x + 1)] == '.') { //Right is empty
                        beams[bi].x++;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x + 1)] == '-') { //Right is effecitvely empty
                        beams[bi].x++;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x + 1)] == '|') { //Right is splitter
                        beams[bi].x++;
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::UP;
                        b2.dir = Direction::DOWN;
                        beams.push_back(b2);
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x + 1)] == '/') { //Right is Right Mirror
                        beams[bi].x++;
                        beams[bi].dir = Direction::UP;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x + 1)] == '\\') { //Right is Left Mirror
                        beams[bi].x++;
                        beams[bi].dir = Direction::DOWN;
                    }

                    break;
                }
                case Direction::DOWN: {
                    if (beams[bi].y + 1 >= m.height) { // At bottom of map
                        break;
                    }
                    else if (m.map[(beams[bi].y + 1) * m.width + beams[bi].x] == '.') { // Position below is empty
                        beams[bi].y++;
                    }
                    else if (m.map[(beams[bi].y + 1) * m.width + beams[bi].x] == '|') { //Position below is effectively empty
                        beams[bi].y++;
                    }
                    else if (m.map[(beams[bi].y + 1) * m.width + beams[bi].x] == '-') { //Position below is splitter, create copy and send in opposite directions
                        beams[bi].y++;
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::LEFT;
                        b2.dir = Direction::RIGHT;
                        beams.push_back(b2);
                    }
                    else if (m.map[(beams[bi].y + 1) * m.width + beams[bi].x] == '/') { //Position below is mirror to left
                        beams[bi].y++;
                        beams[bi].dir = Direction::LEFT;
                    }
                    else if (m.map[(beams[bi].y + 1) * m.width + beams[bi].x] == '\\') { //Position below is mirror right
                        beams[bi].y++;
                        beams[bi].dir = Direction::RIGHT;
                    }
                    break;
                }
                case Direction::LEFT: {
                    if (beams[bi].x - 1 <= -1) { //At left hand side of map
                        break;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x - 1)] == '.') { //Left is empty
                        beams[bi].x--;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x - 1)] == '-') { //Left is effecitvely empty
                        beams[bi].x--;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x - 1)] == '|') { //Left is splitter
                        beams[bi].x--;
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::UP;
                        b2.dir = Direction::DOWN;
                        beams.push_back(b2);
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x - 1)] == '/') { //Left is Right Mirror
                        beams[bi].x--;
                        beams[bi].dir = Direction::DOWN;
                    }
                    else if (m.map[beams[bi].y * m.width + (beams[bi].x - 1)] == '\\') { //Left is Left Mirror
                        beams[bi].x--;
                        beams[bi].dir = Direction::UP;
                    }

                    break;
                }
                }
            }
        }
        i++;
    }

    return historyNoDir.size() - 1;
}

template<typename T>
int run1(T m) {
    return runPos(m, std::make_tuple(0, 0, Direction::RIGHT));
}

template<typename T>
int run2(T input) {
    std::vector<std::tuple<int, int, Direction>> starts;

    for (int x = 0; x < input.width; x++) {
        starts.emplace_back(std::make_tuple(x, 0, Direction::DOWN));
        starts.emplace_back(std::make_tuple(x, input.height - 1, Direction::UP));
    }

    for(int y = 0; y < input.height; y++){
        starts.emplace_back(std::make_tuple(0, y, Direction::RIGHT));
        starts.emplace_back(std::make_tuple(input.width - 1, y, Direction::LEFT));
    }

    std::vector<int> vals(starts.size());
    std::iota(vals.begin(), vals.end(), 0);
    std::for_each(std::execution::par, vals.begin(), vals.end(), [input, starts](int& v) {
        v = runPos(input, starts[v]);
        });
    return *std::ranges::max_element(vals);    
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
    auto part1T = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "PART 2 TOOK: " << part2T << "\n\n";

}

//GOT INPUT IN : 492us
//
//Part 1 : 7482
//PART 1 TOOK : 10ms
//
//Part 2 : 7896
//PART 2 TOOK : 257ms
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 291
//Ticks : 2916194
//TotalDays : 3.37522453703704E-06
//TotalHours : 8.10053888888889E-05
//TotalMinutes : 0.00486032333333333
//TotalSeconds : 0.2916194
//TotalMilliseconds : 291.6194