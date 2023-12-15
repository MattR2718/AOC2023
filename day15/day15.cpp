#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <execution>
#include <chrono>
#include <algorithm>

#include <ctre.hpp>

struct Lens {
    std::string label;
    int focalLength;

    void print() {
        std::cout << "LABEL: " << label << "  FL: " << focalLength << '\n';
    }
};

struct Box {
    std::vector<Lens> lenses;

    void print() {
        std::cout << "------------------------------------------\n";
        for (auto& l : lenses) { l.print(); }
        std::cout << "------------------------------------------\n";
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;

    std::vector<std::string> out;

    while (std::getline(file, linetxt)) {
        for (const auto& m : ctre::search_all <"([\\-=\\w]+)">(linetxt)) {
            out.push_back(m.to_string());
        }
    }
    
    return out;
}

int HASH(const std::string& s) {
    int64_t sum = 0;
    for (const auto& c : s) {
        sum += c;
        sum *= 17;
        sum %= 256;
    }
    return sum;
}

template<typename T>
int64_t run1(T input) {
    int64_t sum = 0;

    for (auto& s : input) {
        sum += HASH(s);
    }

    return sum;
}

template<typename T>
int64_t run2(T input) {
    int64_t sum = 0;

    std::vector<Box> boxes(256);

    for (auto& i : input) {
        // Dash
        if (i.find("-") != std::string::npos) {
            std::string label = i.substr(0, i.length() - 1);
            auto box = HASH(label);
            for(int x = 0; x < boxes[box].lenses.size(); x++){
                if (boxes[box].lenses[x].label == label) {
                    boxes[box].lenses.erase(boxes[box].lenses.begin() + x);
                    x--;
                }
            }
        }
        else { // Equals
            std::string label = i.substr(0, i.length() - 2);
            auto box = HASH(label);
            int focalLength = i[i.length() - 1] - '0';
            bool sameLabel = false;
            for (int x = 0; x < boxes[box].lenses.size(); x++) {
                if (boxes[box].lenses[x].label == label) {
                    sameLabel = true;
                    boxes[box].lenses[x].focalLength = focalLength;
                }
            }
            if (!sameLabel) {
                boxes[box].lenses.emplace_back(Lens{ label, focalLength });
            }

        }
    }

    int i = 0;
    for (auto& b : boxes) {
        i++;
        for (int j = 0; j < b.lenses.size(); j++) {
            sum += i * (j + 1) * b.lenses[j].focalLength;
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

    /*for (auto& s : input) {
        std::cout << s << '\n';
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

//GOT INPUT IN : 965us
//
//Part 1 : 515210
//PART 1 TOOK : 106us
//
//Part 2 : 246762
//PART 2 TOOK : 268us
//
//
//
//Days: 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 13
//Ticks : 131346
//TotalDays : 1.52020833333333E-07
//TotalHours : 3.6485E-06
//TotalMinutes : 0.00021891
//TotalSeconds : 0.0131346
//TotalMilliseconds : 13.1346