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
#include <queue>

enum class Direction {
    UP, RIGHT, DOWN, LEFT
};

bool operator<(Direction a, Direction b) {
    switch (a) {
    case Direction::UP: return b ==Direction::RIGHT || b == Direction::DOWN || b == Direction::LEFT; break;
    case Direction::DOWN: return b == Direction::RIGHT || b == Direction::LEFT; break;
    case Direction::LEFT: return b == Direction::RIGHT; break;
    case Direction::RIGHT: return false; break;
    }
}

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

auto getInput(const std::string f = "test.txt") {
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

/*
Index into map -> {id, weight, direction from prev/Direction currently travelling}

*/
std::map<int, std::vector<std::tuple<int, int, Direction>>> generateAdjList(const Map& m) {
    std::map<int, std::vector<std::tuple<int, int, Direction>>> adj;

    for (int idx = 0; idx < m.map.size(); idx++) {
        int x = idx % m.width;
        int y = idx / m.width;

        // RIGHT
        if (x + 1 < m.width) {
            adj[idx].emplace_back(std::make_tuple(y * m.width + (x + 1), m.map[y * m.width + (x + 1)] - '0', Direction::RIGHT));
        }
        //LEFT
        if (x - 1 > -1) {
            adj[idx].emplace_back(std::make_tuple(y * m.width + (x - 1), m.map[y * m.width + (x - 1)] - '0', Direction::LEFT));
        }

        if (y + 1 < m.height) {
            adj[idx].emplace_back(std::make_tuple((y + 1) * m.width + x, m.map[(y + 1) * m.width + x] - '0', Direction::DOWN));
        }
        if (y - 1 > -1) {
            adj[idx].emplace_back(std::make_tuple((y - 1) * m.width + x, m.map[(y - 1) * m.width + x] - '0', Direction::UP));
        }
    }

    return adj;
}

void printAdj(const std::map<int, std::vector<std::tuple<int, int, Direction>>>& adj) {
    std::cout << "=================ADJACENCY LIST===================\n";
    for (const auto& [k, v] : adj) {
        std::cout << k << ": ";
        for (const auto& [i, w, d] : v) {
            std::cout << "{" << i<< ", " << w << ", ";
            switch (d) {
            case Direction::UP: std::cout << "UP} "; break;
            case Direction::DOWN: std::cout << "DOWN} "; break;
            case Direction::LEFT: std::cout << "LEFT} "; break;
            case Direction::RIGHT: std::cout << "RIGHT} "; break;
            }
        }
        std::cout << '\n';
    }
    std::cout << "==================================================\n";
}

void printPrev(const std::map<std::tuple<int, Direction, int>, std::pair<int, int>>& p) {
    std::cout << "======================PREVIOUS====================\n";
    for (const auto& [k, v] : p) {
        //std::cout << "KEY: " << k << " PREVID: " << v.first << " PREVDIST: " << v.second << '\n';;
        std::cout << "KEY: {" << std::get<0>(k) << ", ";
        switch (std::get<1>(k)) {
        case Direction::UP: std::cout << "UP, "; break;
        case Direction::DOWN: std::cout << "DOWN, "; break;
        case Direction::LEFT: std::cout << "LEFT, "; break;
        case Direction::RIGHT: std::cout << "RIGHT, "; break;
        }
        std::cout << std::get<2>(k) << "} -> {" << v.first << ", " << v.second << "}\n";
    }
    std::cout << "==================================================\n";
}

void printMapWithPath(const Map& m, const std::vector<int>& path) {
    std::cout << "========================PATH======================\n";

    int i = 0;
    for (int x = 0; x < m.map.size(); x++) {
        if (std::find(path.begin(), path.end(), x) != path.end()) { std::cout << "#"; }
        else { std::cout << m.map[x]; }
        if (++i == m.width) {
            i = 0;
            std::cout << '\n';
        }
    }

    std::cout << "==================================================\n";
}

std::map<std::tuple<int, Direction, int>, std::pair<int, int>> dijkstra(const std::map<int, std::vector<std::tuple<int, int, Direction>>>& adj, int start, int end) {
    // Visited: tuple<id(index), Direction(in from), Current Amount in that direction, dist>
    std::vector<std::tuple<int, Direction, int, int>> visited;
    
    //Prev: id, Direction, numindir -> prevId, Distance
    std::map<std::tuple<int, Direction, int>, std::pair<int, int>> prev;
    for (auto& [k, v] : adj) {
        prev[std::make_tuple(k, Direction::RIGHT, 0)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::RIGHT, 1)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::RIGHT, 2)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::RIGHT, 3)] = { -1, INT_MAX };

        prev[std::make_tuple(k, Direction::LEFT, 0)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::LEFT, 1)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::LEFT, 2)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::LEFT, 3)] = { -1, INT_MAX };

        prev[std::make_tuple(k, Direction::UP, 0)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::UP, 1)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::UP, 2)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::UP, 3)] = { -1, INT_MAX };

        prev[std::make_tuple(k, Direction::DOWN, 0)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::DOWN, 1)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::DOWN, 2)] = { -1, INT_MAX };
        prev[std::make_tuple(k, Direction::DOWN, 3)] = { -1, INT_MAX };
    }
    prev[std::make_tuple(0, Direction::RIGHT, 0)] = { -1, 0 };
    prev[std::make_tuple(0, Direction::DOWN, 0)] = { -1, 0 };

    class CMP {
    public:
        bool operator()(const std::tuple<int, Direction, int, int> a, const std::tuple<int, Direction, int, int> b) { return std::get<3>(a) > std::get<3>(b); }
    };

    //Priority Queue: id, direction, amount in that direction, dist
    std::priority_queue<std::tuple<int, Direction, int, int>, std::vector<std::tuple<int, Direction, int, int>>, CMP> q;
    q.emplace(std::tuple<int, Direction, int, int>{ 0, Direction::RIGHT, 0, 0 });
    q.emplace(std::tuple<int, Direction, int, int>{ 0, Direction::DOWN, 0, 0 });


    int i = 0;
    //Loop until queue empty
    while (!q.empty()) {
        //Get current <id, direction, numinThatDirection, currentDist>
        std::tuple<int, Direction, int, int> curr = q.top();
        //Pop off top of queue
        q.pop();

        if (std::get<0>(curr) == end) { break; }

        //std::cout << q.size() << '\n';

        //If seen this before, leave
        if (std::find(visited.begin(), visited.end(), curr) != visited.end()) { continue; }
        else {
            //Set that node to visited int this configuration
            visited.push_back(curr);

            //Loop over all neighbours
            //<id, weight, direction from prev>
            for (std::tuple<int, int, Direction> next : adj.at(std::get<0>(curr))) {
                i++;
                if (i % 10000 == 0) { std::cout << q.size() << '\n'; }
                //Calculate distance to go to that node
                int dist = std::get<3>(curr) + std::get<1>(next);
                if (dist < prev[std::make_tuple(std::get<0>(next), std::get<2>(next), ((std::get<2>(next) == std::get<1>(curr) && std::get<2>(curr) < 4) ? (std::get<2>(curr) + 1) : 0))].second) {
                //std::cout << q.size() << '\n';
                    if (std::get<2>(next) == std::get<1>(curr) && std::get<2>(curr) < 4) {
                        prev[std::make_tuple(std::get<0>(next), std::get<2>(next), std::get<2>(curr) + 1)] = std::make_pair(std::get<0>(curr), dist);
                        //prev[std::get<0>(next)] = { std::get<0>(curr), dist };
                        q.emplace(std::tuple<int, Direction, int, int>{std::get<0>(next), std::get<2>(next), std::get<2>(curr) + 1, dist});
                    }
                    else if(std::get<2>(next) != std::get<1>(curr)) {
                        auto d = std::get<2>(next);

                        if (d != Direction::UP) {
                            prev[std::make_tuple(std::get<0>(next), Direction::UP, 0)] = std::make_pair(std::get<0>(curr), dist);
                            q.emplace(std::tuple<int, Direction, int, int>{std::get<0>(next), Direction::UP, 0, dist});
                        }

                        if (d != Direction::DOWN) {
                            prev[std::make_tuple(std::get<0>(next), Direction::DOWN, 0)] = std::make_pair(std::get<0>(curr), dist);
                            q.emplace(std::tuple<int, Direction, int, int>{std::get<0>(next), Direction::DOWN, 0, dist});
                        }

                        if (d != Direction::LEFT) {
                            prev[std::make_tuple(std::get<0>(next), Direction::LEFT, 0)] = std::make_pair(std::get<0>(curr), dist);
                            q.emplace(std::tuple<int, Direction, int, int>{std::get<0>(next), Direction::LEFT, 0, dist});
                        }

                        if (d != Direction::RIGHT) {
                            prev[std::make_tuple(std::get<0>(next), Direction::RIGHT, 0)] = std::make_pair(std::get<0>(curr), dist);
                            q.emplace(std::tuple<int, Direction, int, int>{std::get<0>(next), Direction::RIGHT, 0, dist});
                        }
                    }
                }
            }
        }
    }

    return prev;
}

struct Node {
    int id;
    int weight;
    Direction dir;
    int numDirection;
};

void printNode(const Node& n) {
    std::cout << "{" << n.id << ", " << n.weight << ", ";
    switch (n.dir) {
    case Direction::UP: std::cout << "UP, "; break;
    case Direction::DOWN: std::cout << "DOWN, "; break;
    case Direction::LEFT: std::cout << "LEFT, "; break;
    case Direction::RIGHT: std::cout << "RIGHT, "; break;
    }
    std::cout << n.numDirection << "}";
}
/*
std::map<Node, std::vector<Node>> generate3DAdj(const Map& m) {
    //Map: id, weight, direction, umindirecion -> nextid, weigth, direction, umIndirection
    std::map<Node, std::vector<Node>> adj;

    for (int idx = 0; idx < m.map.size(); idx++) {
        int x = idx % m.width;
        int y = idx / m.width;

        // RIGHT
        if (x + 1 < m.width) {
            //adj[idx].emplace_back(std::make_tuple(y * m.width + (x + 1), m.map[y * m.width + (x + 1)] - '0', Direction::RIGHT));
            
            int id = y * m.width + x;
            int nid = y * m.width + (x + 1);
            
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 1 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 2 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 3 });

            adj[Node{id, m.map[id] - '0', Direction::LEFT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });

            adj[Node{id, m.map[id] - '0', Direction::UP, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });

            adj[Node{id, m.map[id] - '0', Direction::DOWN, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::RIGHT, 0 });
        }
        //LEFT
        if (x - 1 > -1) {
            //adj[idx].emplace_back(std::make_tuple(y * m.width + (x - 1), m.map[y * m.width + (x - 1)] - '0', Direction::LEFT));

            int id = y * m.width + x;
            int nid = y * m.width + (x - 1);

            adj[Node{id, m.map[id] - '0', Direction::LEFT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 1 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 2 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 3 });

            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });

            adj[Node{id, m.map[id] - '0', Direction::UP, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });

            adj[Node{id, m.map[id] - '0', Direction::DOWN, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::LEFT, 0 });
        }

        if (y + 1 < m.height) {
            //adj[idx].emplace_back(std::make_tuple((y + 1) * m.width + x, m.map[(y + 1) * m.width + x] - '0', Direction::DOWN));

            int id = y * m.width + x;
            int nid = (y + 1) * m.width + x;

            adj[Node{id, m.map[id] - '0', Direction::DOWN, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 1 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 2 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 3 });

            adj[Node{id, m.map[id] - '0', Direction::LEFT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });

            adj[Node{id, m.map[id] - '0', Direction::UP, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });

            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::DOWN, 0 });


        }
        if (y - 1 > -1) {
            //adj[idx].emplace_back(std::make_tuple((y - 1) * m.width + x, m.map[(y - 1) * m.width + x] - '0', Direction::UP));

            int id = y * m.width + x;
            int nid = (y - 1) * m.width + (x + 1);

            adj[Node{id, m.map[id] - '0', Direction::UP, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 1 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 2 });
            adj[Node{id, m.map[id] - '0', Direction::UP, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 3 });

            adj[Node{id, m.map[id] - '0', Direction::LEFT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::LEFT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });

            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::RIGHT, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });

            adj[Node{id, m.map[id] - '0', Direction::DOWN, 0}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 1}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0});
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 2}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
            adj[Node{id, m.map[id] - '0', Direction::DOWN, 3}].emplace_back(Node{ nid, m.map[nid] - '0', Direction::UP, 0 });
        }
    }


    return adj;
}
*/
void print3DAdj(std::map<Node, std::vector<Node>> adj) {
    for (auto [k, v] : adj) {
        printNode(k);
        std::cout << ": ";
        for (auto n : v) {
            printNode(n);
            std::cout << " ";
        }
        std::cout << '\n';
    }
}

int getMinPrev(const std::map<std::tuple<int, Direction, int>, std::pair<int, int>>& prev, int currV) {
    int min = INT_MAX;
    int minid = 0;
    std::cout << "CURRV: " << currV << '\n';
    for(auto& [k, v] : prev){
        if (std::get<0>(k) == currV && v.second < min && (v.second > 0 && v.first != -1)) { min = v.second; minid = v.first; }
    
    }

    return minid;
}

template<typename T>
int run1(T input) {

    auto adj = generateAdjList(input);
    printAdj(adj);
    auto prev = dijkstra(adj, 0, input.map.size() - 1);
    printPrev(prev);

    std::vector<int> path;
    int currV = input.map.size() - 1;
    do{
        path.push_back(currV);
        //currV = prev[currV].first;
        currV = getMinPrev(prev, currV);
    } while (currV != 0);

    for (auto& p : path) {
        std::cout << p << "<-";
    }
    std::cout << '\n';

    printMapWithPath(input, path);

    //auto adj = generate3DAdj(input);
    //print3DAdj(adj);

    return 0;
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

    input.print();

    start = std::chrono::high_resolution_clock::now();
    auto p1 = run1(input);
    end = std::chrono::high_resolution_clock::now();
    auto part1T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 1: " << p1 << '\n';
    std::cout << "PART 1 TOOK: " << part1T << "\n\n";

    start = std::chrono::high_resolution_clock::now();
    auto p2 = run2(input);
    end = std::chrono::high_resolution_clock::now();
    auto part2T = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Part 2: " << p2 << '\n';
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