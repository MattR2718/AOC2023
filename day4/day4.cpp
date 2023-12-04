#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <numeric>
#include <regex>
#include <set>

struct Card {
    int id;
    std::set<int> wn;
    std::set<int> nums;

    void print() {
        std::cout << "--------------------------------------------\n";
        std::cout << "CARD: " << id << '\n';
        std::cout << "WINNING NUMS: ";
        for (auto& n : wn) { std::cout << n << ' '; }
        std::cout << "\nNUMBERS: ";
        for (auto& n : nums) { std::cout << n << ' '; }
        std::cout << "\n---------------------------------------------\n";
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Card> out;

    std::regex ex("\\D*([\\d]+)\\D*");
    std::smatch m;

    std::string card;
    std::string winning;
    std::string nums;


    /*
    Couldn't work out regex to parse all in single pass so had to split up and am left with gross function
    */

    while (std::getline(file, linetxt)) {
        // Split to get crad number
        int colon = linetxt.find(":");
        card = linetxt.substr(0, colon);
        linetxt.erase(0, colon + 2);

        // Split to get winning numbers
        int pipe = linetxt.find("|");
        winning = linetxt.substr(0, pipe - 1);
        linetxt.erase(0, pipe + 2);

        //Numbers
        nums = linetxt;
    
        Card c;
        std::regex_match(card, m, ex);
        c.id = std::stoi(m[1]);

        //Match all numbers in winning numbers
        while (std::regex_search(winning, m, ex)) {
            c.wn.insert(std::stoi(m[1]));
            winning.erase(0, winning.find(m[0]) + m[0].length());
        }

        //Match all numbers
        while (std::regex_search(nums, m, ex)) {
            c.nums.insert(std::stoi(m[1]));
            nums.erase(0, nums.find(m[0]) + m[0].length());
        }
    
        //Store card
        out.push_back(c);
    
    }
    return out;
}

template<typename T>
int run1(T input) {
    int sum = 0;
    for (const Card& c : input) {
        std::set<int> isct;
        std::set_intersection(c.wn.begin(), c.wn.end(), c.nums.begin(), c.nums.end(), std::inserter(isct, isct.begin()));
        sum += (!isct.size()) ? 0 : 1 << (isct.size() - 1);
    }
    return sum;
}

template<typename T>
int run2(T input) {
    
    std::vector<int> count(input.size() + 1, 0);
    std::map<int, int> cache;
    std::vector<int> todo(input.size());
    std::iota(todo.begin(), todo.end(), 1);
    
    for (int i = 0; i < todo.size(); i++) {
        count[input[todo[i] - 1].id]++;
        if (cache.find(input[todo[i] - 1].id) == cache.end()) {
            std::set<int> isct;
            std::set_intersection(input[todo[i] - 1].wn.begin(), input[todo[i] - 1].wn.end(), input[todo[i] - 1].nums.begin(), input[todo[i] - 1].nums.end(), std::inserter(isct, isct.begin()));
            cache[input[todo[i] - 1].id] = isct.size();
        }
        for (int j = input[todo[i] - 1].id; j < input[todo[i] - 1].id + cache[input[todo[i] - 1].id]; j++) {
            todo.push_back(input[j].id);
        }
    }
    return std::accumulate(count.begin(), count.end(), 0);
}


int main(int argc, char **argv) {
    auto input = getInput();

    /*for (Card& c : input) { c.print(); }*/

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}

//Part 1: 20407
//Part 2: 23806951
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 761
//Ticks : 7614586
//TotalDays : 8.81317824074074E-06
//TotalHours : 0.000211516277777778
//TotalMinutes : 0.0126909766666667
//TotalSeconds : 0.7614586
//TotalMilliseconds : 761.4586