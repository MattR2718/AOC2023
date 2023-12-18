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

struct Instruction {
    char dir;
    int num;
    std::string colour;

    void print() {
        std::cout << "DIR " << dir << "  NUM: " << num << "  COLOUR: " << colour << '\n';
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;

    std::vector<Instruction> out;
    
    while (std::getline(file, linetxt)) {
        Instruction i;
        i.dir = linetxt[0];
        linetxt.erase(0, 2);

        i.num = std::stoi(linetxt.substr(0, linetxt.find(" ")));
        linetxt.erase(0, linetxt.find(" ") + 1);

        i.colour = linetxt;

        out.emplace_back(i);
    }

    return out;
}

std::vector<std::pair<int, int>> generatePoints(const std::vector<Instruction>& ins) {
    std::vector<std::pair<int, int>> points;
    points.emplace_back(std::make_pair( 0, 0 ));

    std::pair<int, int> curr{0, 0};

    for(auto& in : ins){
        switch (in.dir){
        case 'U': {
            curr.second -= in.num;
            points.push_back(curr);
            break;
        }
        case 'D': {
            curr.second += in.num;
            points.push_back(curr);
            break;
        }
        case 'L': {
            curr.first -= in.num;
            points.push_back(curr);
            break;
        }
        case 'R': {
            curr.first += in.num;
            points.push_back(curr);
            break;
        }
        }
    }

    return points;
}

void printMap(const std::vector<std::vector<char>>& map, const int maxx, const int maxy) {
    for (int j = 0; j < maxy; j++) {
        for (int i = 0; i < maxx; i++) {
            std::cout << map[j][i];
        }
        std::cout << '\n';
    }
}

void floodFill(std::vector<std::vector<char>>& map, const int maxx, const int maxy) {
    std::queue<std::pair<int, int>> q;
    q.push({ 0, 0 });

    std::set<std::pair<int, int>> visited;

    while (!q.empty()) {
        //std::cout << "QUEUE SIZE: " << q.size() << '\n';
        auto c = q.front();
        q.pop();
        if (visited.contains(c)) { continue;  }
        visited.insert(c);
        //std::cout << c.second << " " << c.first << '\n';
        if (map[c.second][c.first] == '#') { continue; }
        map[c.second][c.first] = '-';

        if (c.first > 0 && !visited.contains({ c.first - 1, c.second })) { q.push({ c.first - 1, c.second }); }
        if (c.second > 0 && !visited.contains({ c.first, c.second - 1 })) { q.push({ c.first, c.second - 1 }); }
        if (c.first < maxx - 1 && !visited.contains({ c.first + 1, c.second })) { q.push({ c.first + 1, c.second }); }
        if (c.second < maxy - 1 && !visited.contains({ c.first, c.second + 1 })) { q.push({ c.first, c.second + 1 }); }
    }
}

template<typename T>
int run1(T input) {
    std::vector<std::pair<int, int>> points = generatePoints(input);
    int minx = INT_MAX;
    int miny = INT_MAX;
    for (auto& p : points) {
        //std::cout << "X: " << p.first << "  Y: " << p.second << '\n';
        if (p.first < minx) { minx = p.first; }
        if (p.second < miny) { miny = p.second; }
    }
    //std::cout << minx << " " << miny << '\n';
    minx = std::abs(minx);
    minx += 3;
    miny = std::abs(miny);
    miny += 3;

    int maxx = 0;
    int maxy = 0;
    for (auto& p : points) {
        p.first += minx;
        p.second += miny;

        if (p.first > maxx) { maxx = p.first; }
        if (p.second > maxy) { maxy = p.second; }

        //std::cout << "X: " << p.first << "  Y: " << p.second << '\n';
    }

    maxx += 3;
    maxy += 3;

    //std::cout << maxx << " - " << maxy << '\n';

    std::vector<std::vector<char>> map(maxy, std::vector<char>(maxx, '.'));
    
    //printMap(map, maxx, maxy);

    points.push_back(points[0]);
    for (int i = 1; i < points.size(); i++) {
        auto p1 = points[i - 1];
        auto p2 = points[i];
        
        if (p1.first < p2.first) {
            for (int x = 0; x <= p2.first - p1.first; x++) {
                //std::cout << "1\n";
                //std::cout << p1.first << " " <<x << '\n';
                map[p1.second][p1.first + x] = '#';
            }
        }
        else if(p2.first < p1.first) {
            for (int x = 0; x <= p1.first - p2.first; x++) {
                //std::cout << "1\n";
                map[p1.second][p2.first + x] = '#';
            }
        }else if (p1.second < p2.second) {
            for (int x = 0; x <= p2.second - p1.second; x++) {
                //std::cout << "1\n";
                map[p1.second + x][p1.first] = '#';
            }
        }
        else if (p2.second < p1.second) {
            for (int x = 0; x <= p1.second - p2.second; x++) {
                //std::cout << "1\n";
                map[p2.second + x][p1.first] = '#';
            }
        }

        //printMap(map, maxx, maxy);

    }

    //printMap(map, maxx, maxy);

    floodFill(map, maxx, maxy);

    printMap(map, maxx, maxy);

    int sum = 0;
    for (auto& v : map) {
        for (auto& c : v) {
            if (c != '-') { sum++; }
        }
    }

    return sum;
}

template<typename T>
int run2(T input) {
    return 0;
}


int main() {
    auto start = std::chrono::high_resolution_clock::now();
    auto input = getInput();
    auto end = std::chrono::high_resolution_clock::now();
    auto inputT = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "GOT INPUT IN: " << inputT << "\n\n";

    /*for (auto i : input) {
        i.print();
    }*/

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
