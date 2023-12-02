#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <regex>


struct Round{
    int red = 0;
    int green = 0;
    int blue = 0;

    void print() {
        std::cout << "RED: " << this->red << " GREEN: " << this->green << " BLUE: " << this->blue << "\n";
    }
};

struct Game{
    int id = -1;
    std::vector<Round> rounds;

    void print() {
        std::cout << "ID: " << this->id << '\n';
        for (auto& r : this->rounds) {
            r.print();
        }
    }
};



auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Game> out;
    std::regex ex("Game ([0-9]+)");
    std::smatch m;

    while (std::getline(file, linetxt)) {
        Game game;
        std::string gameID = linetxt.substr(0, linetxt.find(":"));
        std::string roundAll = linetxt.substr(linetxt.find(":") + 2, linetxt.length() - linetxt.find(":") - 2);
        
        // Get gameID
        std::regex_search(gameID, m, ex);
        game.id = std::stoi(m[1].str());


        //Get rounds
        std::vector<std::string> rounds;
        int pos = 0;
        while ((pos = roundAll.find(";")) != std::string::npos) {
            rounds.push_back(roundAll.substr(0, pos));
            roundAll.erase(0, pos + 2);
        }
        rounds.push_back(roundAll);

        // Loop over rounds
        for (auto& s : rounds) {
            //Split up round
            std::vector<std::string> toks;
            while ((pos = s.find(" ")) != std::string::npos) {
                toks.push_back(s.substr(0, pos));
                s.erase(0, pos + 1);
            }
            toks.push_back(s);

            Round r;
            // Loop over round checking the colour and setting values appropriately
            for (int i = 1; i < toks.size(); i += 2) {
                if (toks[i] == "red" || toks[i] == "red,") {
                    r.red = std::stoi(toks[i - 1]);
                }else if (toks[i] == "green" || toks[i] == "green,") {
                    r.green = std::stoi(toks[i - 1]);
                }
                else if(toks[i] == "blue" || toks[i] == "blue,") {
                    r.blue = std::stoi(toks[i - 1]);
                }
            }

            game.rounds.push_back(r);
        
        }
        out.push_back(game);

    }
    return out;
}

template<typename T>
int run1(T input) {
    const int maxR = 12, maxG = 13, maxB = 14;
    int sum = 0;
    for (Game& g : input) {
        bool valid = true;
        for (Round& r : g.rounds) {
            if (r.red > maxR || r.green > maxG || r.blue > maxB) { valid = false; break; }
        }
        if (valid) { sum += g.id; }
    }

    return sum;
}

template<typename T>
int run2(T input) {
    int sum = 0;

    for (Game& g : input) {
        int minr = 0, ming = 0, minb = 0;
        for (Round& r : g.rounds) {
            minr = (minr < r.red) ? r.red : minr;
            ming = (ming < r.green) ? r.green : ming;
            minb = (minb < r.blue) ? r.blue : minb;
        }
        sum += minr * ming * minb;
    }

    return sum;
}


int main() {
    auto input = getInput();

    /*for (Game g : input) {
        g.print();
    }*/

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

// Part 1: 2720
// Part 2 : 71535