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

enum class HAND_TYPE {
    five = 6,
    four = 5,
    full = 4,
    three = 3,
    two = 2,
    one = 1,
    high = 0,
    invalid = -1
};

struct Hand {
    std::string hand;
    int score;

    void print() {
        std::cout << "HAND [" << hand << "]  SCORE [" << score << "]\n";
    }

    void print2() {
        std::cout << "HAND: [";
        for (auto c : hand) {
            switch (c) {
            case '9' + 1: std::cout << "T"; break;
            case '1': std::cout << "J";  break;
            case '9' + 3: std::cout << "Q"; break;
            case '9' + 4: std::cout << "K"; break;
            case '9' + 5: std::cout << "A"; break;
            default: std::cout << c; break;
            }
        }
        std::cout << "]";
    }

    void print2Type() {
        std::cout << "HAND: [";
        for (auto c : hand) {
            switch (c) {
            case '9' + 1: std::cout << "T"; break;
            case '1': std::cout << "J";  break;
            case '9' + 3: std::cout << "Q"; break;
            case '9' + 4: std::cout << "K"; break;
            case '9' + 5: std::cout << "A"; break;
            default: std::cout << c; break;
            }
        }
        std::cout << "]     TYPE: [" << static_cast<int>(getTypePt2()) << "]\n";
    }

    HAND_TYPE getType() {
        std::map<char, int> cards;
        for (auto& c : hand) { cards[c]++; }
        if (cards.size() == 1) { return HAND_TYPE::five; }
        if (cards.size() == 2) { 
            for (auto& [k, v] : cards) {
                if (v == 1 || v == 4) { return HAND_TYPE::four; }
                return HAND_TYPE::full;
            }
        }
        if (cards.size() == 3) { 
            for (auto& [k, v] : cards) {
                if (v == 3) { return HAND_TYPE::three; }
                else if (v == 2) { return HAND_TYPE::two; }
            }
        }
        if (cards.size() == 4) { return HAND_TYPE::one; }
        if (cards.size() == 5) { return HAND_TYPE::high; }
        return HAND_TYPE::invalid;
    }

    // Disgusting function, could be tidied up but works
    HAND_TYPE getTypePt2() {
        std::map<char, int> cards;
        for (auto& c : hand) {
            if (c == '9' + 2) { c = '1'; }
            cards[c]++;
        }

        switch (cards['1']) {
        case 0: {
            return this->getType();
        };
        case 1: {
            int maxNum = 0;
            for (auto& [k, v] : cards) {
                if (k != '1' && v > maxNum) { maxNum = v; }
            }
            if (maxNum == 4) { return HAND_TYPE::five; } // Use joker to make five of a kind
            if (maxNum == 3) { return HAND_TYPE::four; } // Use joker to make four of a kind
            if (maxNum == 2) {                           // Use joker to make either fill house of three of a kind
                int num2 = 0;
                int num1 = 0;
                for (auto& [k, v] : cards) {
                    if (k != '1' && v == 1) { num1++; }
                    if (k != '1' && v == 2) { num2++; }
                }
                if (num2 == 2) { return HAND_TYPE::full; }
                else { return HAND_TYPE::three; }
            }
            if (maxNum == 1) { return HAND_TYPE::one; }
        }
        
        case 2: {
            int maxNum = 0;
            for (auto& [k, v] : cards) {
                if (k != '1' && v > maxNum) { maxNum = v; }
            }
            if (maxNum == 3) { return HAND_TYPE::five; } // Use joker to make five of a kind
            if (maxNum == 2) { return HAND_TYPE::four; } // Use joker to make four of a kind
            if (maxNum == 1) {                           // Use joker to make either fill house of three of a kind
                int num2 = 0;
                int num1 = 0;
                for (auto& [k, v] : cards) {
                    if (k != '1' && v == 1) { num1++; }
                    if (k != '1' && v == 2) { num2++; }
                }
                if (num2 == 2) { return HAND_TYPE::full; }
                else { return HAND_TYPE::three; }
            }
        }
        case 3: {
            int maxNum = 0;
            for (auto& [k, v] : cards) {
                if (k != '1' && v > maxNum) { maxNum = v; }
            }
            if (maxNum == 2) {  return HAND_TYPE::five; } // Use joker to make five of a kind
            if (maxNum == 1) { return HAND_TYPE::four; } // Use joker to make four of a kind
        }
        case 4: {
            int maxNum = 0;
            for (auto& [k, v] : cards) {
                if (k != '1' && v > maxNum) { maxNum = v; }
            }
            if (maxNum == 1) { return HAND_TYPE::five; } // Use joker to make five of a kind
        }
        case 5: { return HAND_TYPE::five; }

        }

        
        return HAND_TYPE::invalid;
    }
};

auto getInput(const std::string f = "input.txt") {
    std::ifstream file(f);
    std::string linetxt;
    std::vector<Hand> out;

    while (std::getline(file, linetxt)) {
        Hand h;
        std::istringstream ss(linetxt);
        ss >> h.hand >> h.score;

        for (auto& c : h.hand) {
            switch (c) {
                case 'T': c = '9' + 1; break;
                case 'J': c = '9' + 2; break;
                case 'Q': c = '9' + 3; break;
                case 'K': c = '9' + 4; break;
                case 'A': c = '9' + 5; break;
                default: c = c; break;
            }
        }

        out.push_back(h);
    }

    return out;
}


template<typename T>
int run1(T input) {
    
    std::vector<std::vector<Hand>> hands(7);
    for (Hand h : input) {
        hands[static_cast<int>(h.getType())].push_back(h);
    }

    for (auto& hv : hands) {
        std::sort(hv.begin(), hv.end(), [](const Hand& h1, const Hand& h2) { return h2.hand > h1.hand; });
    }

    int64_t n = 0;
    int count = 1;

    for (auto& hv : hands) {
        for (auto& h : hv) {
            n += count++ * h.score;
        }
    }

    return n;
}



template<typename T>
int run2(T input) {
    
    std::vector<std::vector<Hand>> hands(7);
    for (Hand h : input) {
        hands[static_cast<int>(h.getTypePt2())].push_back(h);
    }

    for (auto& hv : hands) {
        std::sort(hv.begin(), hv.end(), [](const Hand& h1, const Hand& h2) { return h2.hand > h1.hand; });
    }

    int64_t n = 0;
    int count = 1;

    for (auto& hv : hands) {
        for (auto& h : hv) {
            n += count++ * h.score;
        }
    }




    return n;
}


int main() {
    auto input = getInput();

    /*for (auto& h : input) {
        h.print();
    }*/

    std::cout << "Part 1: " << run1(input) << '\n';
    std::cout << "Part 2: " << run2(input) << '\n';
}


//Part 1: 253910319
//Part 2: 254083736
//
//
//Days : 0
//Hours : 0
//Minutes : 0
//Seconds : 0
//Milliseconds : 21
//Ticks : 216226
//TotalDays : 2.50261574074074E-07
//TotalHours : 6.00627777777778E-06
//TotalMinutes : 0.000360376666666667
//TotalSeconds : 0.0216226
//TotalMilliseconds : 21.6226