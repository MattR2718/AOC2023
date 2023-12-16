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

bool valid(int x, int y, int mx, int my) {
    return (x > -1) && (y > -1) && (x < mx) && (y < my);
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

    std::vector<uint8_t> history(m.map.size(), 0);

    int numInLoop = 0;
    int i = 0;
    while (beams.size()) {
        for (int bi = 0; bi < beams.size(); bi++) {
            //If seen this exact position before, exit
            if(valid(beams[bi].x, beams[bi].y, m.width, m.height) && (history[beams[bi].y * m.width + beams[bi].x] & (1 << static_cast<typename std::underlying_type<Direction>::type>(beams[bi].dir)))) {
                beams.erase(beams.begin() + bi);
                bi--;
            }
            else { //Otherwise work out what next position is
                if (valid(beams[bi].x, beams[bi].y, m.width, m.height)) {
                    history[beams[bi].y * m.width + beams[bi].x] += (1 << static_cast<typename std::underlying_type<Direction>::type>(beams[bi].dir));
                }
                switch (beams[bi].dir) {
                case Direction::UP: {
                    if (beams[bi].y - 1 <= -1) { // At top of map
                        break;
                    }
                    beams[bi].y--;
                    auto c = m.map[beams[bi].y * m.width + beams[bi].x];
                    if (c == '-') { //Position above is splitter, create copy and send in opposite directions
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::LEFT;
                        b2.dir = Direction::RIGHT;
                        beams.push_back(b2);
                    }
                    else if (c == '/') { //Position above is mirror to right
                        beams[bi].dir = Direction::RIGHT;
                    }
                    else if (c == '\\') { //Position above is mirror left
                        beams[bi].dir = Direction::LEFT;
                    }
                    break;
                }
                case Direction::RIGHT: {
                    if (beams[bi].x + 1 >= m.width) { //At right hand side of map
                        break;
                    }
                    beams[bi].x++;
                    auto c = m.map[beams[bi].y * m.width + beams[bi].x];
                    if (c == '|') { //Right is splitter
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::UP;
                        b2.dir = Direction::DOWN;
                        beams.push_back(b2);
                    }
                    else if (c == '/') { //Right is Right Mirror
                        beams[bi].dir = Direction::UP;
                    }
                    else if (c == '\\') { //Right is Left Mirror
                        beams[bi].dir = Direction::DOWN;
                    }

                    break;
                }
                case Direction::DOWN: {
                    if (beams[bi].y + 1 >= m.height) { // At bottom of map
                        break;
                    }
                    beams[bi].y++;
                    auto c = m.map[beams[bi].y * m.width + beams[bi].x];
                    if (c == '-') { //Position below is splitter, create copy and send in opposite directions
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::LEFT;
                        b2.dir = Direction::RIGHT;
                        beams.push_back(b2);
                    }
                    else if (c == '/') { //Position below is mirror to left
                        beams[bi].dir = Direction::LEFT;
                    }
                    else if (c == '\\') { //Position below is mirror right
                        beams[bi].dir = Direction::RIGHT;
                    }
                    break;
                }
                case Direction::LEFT: {
                    if (beams[bi].x - 1 <= -1) { //At left hand side of map
                        break;
                    }
                    beams[bi].x--;
                    auto c = m.map[beams[bi].y * m.width + beams[bi].x];
                    if (c == '|') { //Left is splitter
                        Beam b2 = beams[bi];
                        beams[bi].dir = Direction::UP;
                        b2.dir = Direction::DOWN;
                        beams.push_back(b2);
                    }
                    else if (c == '/') { //Left is Right Mirror
                        beams[bi].dir = Direction::DOWN;
                    }
                    else if (c == '\\') { //Left is Left Mirror
                        beams[bi].dir = Direction::UP;
                    }

                    break;
                }
                }
            }
        }
        i++;
    }

    return std::ranges::count_if(history, [](const auto a) {return a > 0; });
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
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";
    
    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "PART 2 TOOK: " << part2T << "\n\n";

}

//GOT INPUT IN : 183us
//
//Part 1 : 7482
//PART 1 TOOK : 128us
//
//Part 2 : 7896
//PART 2 TOOK : 6226us
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 22
//Ticks : 226777
//TotalDays : 2.6247337962963E-07
//TotalHours : 6.29936111111111E-06
//TotalMinutes : 0.000377961666666667
//TotalSeconds : 0.0226777
//TotalMilliseconds : 22.6777