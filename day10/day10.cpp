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

enum class Direction {
    NUL, UP, RIGHT, DOWN, LEFT
};

struct Pos {
    int x = -1;
    int y = -1;

    Direction dir;

    bool empty() { return  x == -1 && y == -1; }
    void print() { std::cout << "X: " << x << " Y: " << y << " DIR: " << static_cast<std::underlying_type<Direction>::type>(dir) << '\n'; }
    void setVals(int x_, int y_, Direction dir_) { x = x_; y = y_; dir = dir_; }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::string instructions;
    std::vector<std::string> out;
    int x = 0, y = 0;
    int i = 0, j = 0;

    while (std::getline(file, linetxt)) {
        for (int i = 0; i < linetxt.size(); i++) {
            if (linetxt[i] == 'S') { x = i; y = j; }
        }
        out.push_back(linetxt);
        j++;
    }

    return std::make_tuple(x, y, out);
}

void step(Pos& p, const std::vector<std::string>& map) {
    switch (map[p.y][p.x]) {
    case '|': {
        if (p.dir == Direction::UP) { p.y--; }
        else if (p.dir == Direction::DOWN) { p.y++; }
        else { 
            std::cout << "ERROR WITH |\n";
            p.print();
        }
        return;
    }
    case '-': {
        if (p.dir == Direction::RIGHT) { p.x++; }
        else if (p.dir == Direction::LEFT) { p.x--; }
        else { 
            std::cout << "ERROR WITH -\n";
            p.print();
        }
        return;
    }
    case'L': {
        if (p.dir == Direction::DOWN) { p.x++; p.dir = Direction::RIGHT; }
        else if (p.dir == Direction::LEFT) { p.y--; p.dir = Direction::UP; }
        else { 
            std::cout << "ERROR WITH L\n";
            p.print();
        }
        return;
    }
    case 'J': {
        if (p.dir == Direction::DOWN) { p.x--; p.dir = Direction::LEFT; }
        else if (p.dir == Direction::RIGHT) { p.y--; p.dir = Direction::UP; }
        else { 
            std::cout << "ERROR WITH J\n";
            p.print();
        }
        return;
    }
    case '7': {
        if (p.dir == Direction::RIGHT) { p.y++; p.dir = Direction::DOWN; }
        else if (p.dir == Direction::UP) { p.x--; p.dir = Direction::LEFT; }
        else { 
            std::cout << "ERROR WITH 7\n";
            p.print();
        }
        return;
    }
    case 'F': {
        if (p.dir == Direction::UP) { p.x++; p.dir = Direction::RIGHT; }
        else if (p.dir == Direction::LEFT) { p.y++; p.dir = Direction::DOWN; }
        else { 
            std::cout << "ERROR WITH F\n";
            p.print();
        }
        return;
    }
    default: std::cout << "ERROR WITH CHAR\n";
    }
}


template<typename T>
int run1(T& input, bool drawPath = false) {
    auto&[x, y, map] {input};
    Pos p1{ -1, -1, Direction::NUL };
    Pos p2{ -1, -1, Direction::NUL };

    auto pathMap = map;

    //North
    if (y - 1 > -1 && (map[y - 1][x] == '|' || map[y - 1][x] == '7' || map[y - 1][x] == 'F')) {
        p1 = { x, y - 1, Direction::UP };
    }
    //East
    if (x + 1 < map[y].length() && (map[y][x + 1] == '-' || map[y][x + 1] == '7' || map[y][x + 1] == 'J')) {
        if (p1.empty()) { p1 = { x + 1, y, Direction::RIGHT }; }
        else{p2 = { x + 1, y, Direction::RIGHT };}
    }
    //South
    if (y + 1 < map.size() && (map[y + 1][x] == '|' || map[y + 1][x] == 'L' || map[y + 1][x] == 'J')) {
        if (p1.empty()) { p1 = { x, y + 1,Direction::DOWN }; }
        else if (p2.empty()) { p2 = { x, y + 1,Direction::DOWN }; }
    }
    //West
    if (x - 1 > -1 && (map[y][x - 1] == '-' || map[y][x - 1] == 'L' || map[y][x - 1] == 'F')) {
        if (p1.empty()) { p1 = { x - 1, y,Direction::LEFT }; }
        else if (p2.empty()) { p2 = { x - 1, y,Direction::LEFT }; }
    }

    int count = 1;
    while ((p1.x != p2.x) || (p1.y != p2.y)) {
        if (drawPath) {
            pathMap[p1.y][p1.x] = '#';
            pathMap[p2.y][p2.x] = '#';
        }

        step(p1, map);
        step(p2, map);
        count++;
    }

    if (drawPath) {
        pathMap[p1.y][p1.x] = '#';
    }

    map = pathMap;
    
    return count;
}

std::vector<std::string> expandMap(const std::vector<std::string>& map) {
    std::vector<std::string> exmap;

    std::string blank = "";
    for (int i = 0; i < map[0].length(); i++) { blank += ".."; }
    exmap.push_back(blank);

    for (int j = 0; j < map.size(); j++) {
        std::string exLine = "";
        for (auto& c : map[j]) { exLine += c; exLine += "."; }
        exmap.push_back(exLine);
        exmap.push_back(blank);
    }

    for (int j = 1; j < exmap.size() - 1; j+=2) {
        for (int i = 0; i < exmap[j].length() - 1; i+=2) {
            //std::cout << "i: " << i << " j: " << j << '\n';
            switch (exmap[j][i]) {
            case 'S':
            case '.': break;
            case '|': {
                if (j > 0) { exmap[j - 1][i] = '|'; }
                if (j < exmap.size() - 1) { exmap[j + 1][i] = '|'; }
                break;
            }
            case '-': {
                if (i + 1 < exmap[j].length()) { exmap[j][i + 1] = '-'; }
                if (i > 0) { exmap[j][i - 1] = '-'; }
                break;
            }
            case 'L': {
                if (j > 0) { exmap[j - 1][i] = '|'; }
                if (i + 1 < exmap[j].length()) { exmap[j][i + 1] = '-'; }
                break;
            }
            case 'J': {
                if (j > 0) { exmap[j - 1][i] = '|'; }
                if (i > 0) { exmap[j][i - 1] = '-'; }
                break;
            }
            case '7': {
                if (j + 1 < exmap.size()) { exmap[j + 1][i] = '|'; }
                if (i > 0) { exmap[j][i - 1] = '-'; }
                break;
            }
            case 'F': {
                if (j + 1 < exmap.size()) { exmap[j + 1][i] = '|'; }
                if (i + 1 < exmap[j].length()) { exmap[j][i + 1] = '-'; }
                break;
            }
            default: break;
            }
        }
    }

    return exmap;
}

void floodFill(int x, int y, std::vector<std::string> &map) {
    if (x < 0 || y < 0 || x > map[0].length() || y >= map.size()) { return; }
    if (map[y][x] == '.') { map[y][x] = '_'; }
    else { return; }

    floodFill(x - 1, y, map);
    floodFill(x + 1, y, map);
    floodFill(x, y - 1, map);
    floodFill(x, y + 1, map);
}

template<typename T>
int run2(T input) {
    auto [x, y, m] {input};

    // Expand map so there are gaps in adjacent pipes for flood fill to get through
    auto exMap = expandMap(m);

    //Find new start point
    for (int i = 0; i < exMap[0].length(); i++) {
        for (int j = 0; j < exMap.size(); j++) {
            if (exMap[j][i] == 'S') { x = i; y = j; break; }
        }
    }
    
    //Use part 1 to mark on the path
    auto p1 = std::make_tuple(x, y, exMap);
    auto _ = run1(p1, true);
    exMap = std::get<2>(p1);

    //Convert all points not part of path to '.'
    for (auto& s : exMap) {
        for (auto& c : s) {
            if (c != '#' && c != 'S') { c = '.'; }
        }
    }

    // Do flood fill on all outside points to eliminate all points not enclosed by the loop
    for (int i = 0; i < exMap[0].length(); i++) {
        floodFill(i, 0, exMap);
        floodFill(i, exMap.size() - 1, exMap);
    }
    for (int j = 0; j < exMap.size(); j++) {
        floodFill(exMap[j].length(), j, exMap);
        floodFill(0, j, exMap);
    }

    /*
              .
    Look for ... since as expanded, that is a single point inside the loop in the non expanded
              .  
    */
    std::vector<std::pair<int, int>> points;
    std::vector<std::pair<int, int>> origPoints;
    for (int j = 1; j < exMap.size() - 1; j+=2) {
        for (int i = 2; i < exMap[j].length() - 1; i+=2) {
            if (exMap[j][i] == '.') {
                if (exMap[j - 1][i] == '.' &&
                    exMap[j + 1][i] == '.' &&
                    exMap[j][i - 1] == '.' &&
                    exMap[j][i + 1] == '.'
                    ) {
                    points.emplace_back(std::make_pair(i, j));
                    origPoints.emplace_back(std::make_pair(i / 2, (j - 1) / 2));
                    exMap[j][i] = 'I';
                }
            }
        }
    }

    return points.size();
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Got input in: " << time << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 1: " << run1(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 1 ran in: " << time << '\n';

    start = std::chrono::high_resolution_clock::now();
    std::cout << "Part 2: " << run2(input) << '\n';
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 2 ran in: " << time << '\n';
}

//Got input in : 799us
//
//Part 1 : 6823
//Part 1 ran in : 582us
//Part 2 : 415
//Part 2 ran in : 3479us
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 24
//Ticks : 249086
//TotalDays : 2.88293981481481E-07
//TotalHours : 6.91905555555556E-06
//TotalMinutes : 0.000415143333333333
//TotalSeconds : 0.0249086
//TotalMilliseconds : 24.9086