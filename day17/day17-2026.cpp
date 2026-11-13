#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <mdspan>
#include <print>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

auto getInput() {
  std::vector<int> grid;
  int width = 0;
  std::string linetxt;
  while (std::getline(std::cin, linetxt)) {
    if (!width) {
      width = linetxt.length();
    }
    for (const char c : linetxt) {
      grid.push_back(c - '0');
    }
  }
  return std::pair{grid, width};
}

using grid_t = std::mdspan<int, std::dextents<size_t, 2>>;

enum facing { UP, RIGHT, DOWN, LEFT };
struct point {
  int x = 0, y = 0;

  auto operator<=>(const point &) const = default;
};

struct state {
  point p{0, 0};
  int nid{0};
  facing f{facing::UP};
  int chl{0};

  auto operator<=>(const state &other) const { return chl <=> other.chl; }
};

point nextPos(const point &curr, const facing &fac) {
  switch (fac) {
  case facing::UP:
    return {curr.x, curr.y - 1};
  case facing::DOWN:
    return {curr.x, curr.y + 1};
  case facing::LEFT:
    return {curr.x - 1, curr.y};
  case facing::RIGHT:
    return {curr.x + 1, curr.y};
  }
  return {0};
}

bool pointValid(const point &p, const int width, const int height) {
  return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

auto neighbours(const grid_t &grid, const state &curr, bool p1 = true) {
  auto width = grid.extent(1);
  auto height = grid.extent(0);
  std::vector<state> ret;

  auto n_f = nextPos(curr.p, curr.f);
  if (((p1 && curr.nid < 3) || (!p1 && curr.nid < 10)) &&
      pointValid(n_f, width, height)) {
    ret.emplace_back(n_f, curr.nid + 1, curr.f, curr.chl + grid[n_f.y, n_f.x]);
  }

  if (!p1 && curr.nid < 4 && curr.nid > 0) {
    return ret;
  }

  auto push_new_in_dir = [&](point p, facing f) {
    auto n_p = nextPos(p, f);
    if (!pointValid(n_p, width, height)) {
      return;
    }
    ret.emplace_back(n_p, 1, f, curr.chl + grid[n_p.y, n_p.x]);
  };

  switch (curr.f) {
  case facing::DOWN:
  case facing::UP: {
    push_new_in_dir(curr.p, facing::LEFT);
    push_new_in_dir(curr.p, facing::RIGHT);
    break;
  }
  case facing::LEFT:
  case facing::RIGHT: {
    push_new_in_dir(curr.p, facing::UP);
    push_new_in_dir(curr.p, facing::DOWN);
    break;
  }
  }

  return ret;
}

struct st {
  point p;
  facing f;
  int nid;

  auto operator<=>(const st &) const = default;
};

auto solve(grid_t grid, bool p1 = true) {
  point curr{0, 0};
  std::priority_queue<state, std::vector<state>, std::greater<state>> q;
  q.push({{0, 0}, 0, facing::RIGHT, 0});
  q.push({{0, 0}, 0, facing::DOWN, 0});

  std::vector<char> vis(grid.extent(0) * grid.extent(1) * 4 * (p1 ? 4 : 11), 0);
  auto visited =
      std::mdspan(vis.data(), grid.extent(1), grid.extent(0), 4, (p1) ? 4 : 11);
  visited[0, 0, facing::RIGHT, 0] = 1;
  visited[0, 0, facing::DOWN, 0] = 1;

  while (!q.empty()) {
    auto curr = q.top();
    q.pop();

    if (curr.p.x == grid.extent(1) - 1 && curr.p.y == grid.extent(0) - 1 &&
        (p1 || curr.nid > 3 && curr.nid < 11)) {

      return curr.chl;
    } else {
      for (auto neigh : neighbours(grid, curr, p1)) {
        st tem{neigh.p, neigh.f, neigh.nid};
        if (visited[neigh.p.x, neigh.p.y, neigh.f, neigh.nid]) {
          continue;
        }
        visited[neigh.p.x, neigh.p.y, neigh.f, neigh.nid] = 1;
        q.push(neigh);
      }
    }
  }
}

auto p2(grid_t grid) { return 2; }

int main() {
  auto [vec, width]{getInput()};
  auto grid = std::mdspan(vec.data(), vec.size() / width, width);

  std::println("Part 1: {}", solve(grid));
  std::println("Part 2: {}", solve(grid, false));
}

/*
Part 1: 928
Part 2: 1104

________________________________________________________
Executed in   65.68 millis    fish           external
   usr time   62.05 millis    0.00 micros   62.05 millis
   sys time    3.52 millis  530.00 micros    2.99 millis

*/
