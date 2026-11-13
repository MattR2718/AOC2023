#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <set>
#include <sstream>
#include <sys/types.h>
#include <vector>

#include <ctre.hpp>
enum dir { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

struct data {
  dir d;
  int dist;
  std::string col;
};

auto getInput() {
  std::vector<data> in;
  std::string linetxt{""};
  while (std::getline(std::cin, linetxt)) {
    auto [w_, d, num,
          col]{ctre::match<"([UDLR]) (\\d+) \\(#([a-f0-9]{6})\\)">(linetxt)};
    in.emplace_back(
        [](char d) {
          switch (d) {
          case 'U':
            return UP;
          case 'D':
            return DOWN;
          case 'L':
            return LEFT;
          case 'R':
            return RIGHT;
          }
          return UP;
        }(d.str()[0]),
        num.to_number(), col.str());
  }

  return in;
}

struct point {
  int64_t x = 0, y = 0;

  point operator+(const point &other) {
    return point{x + other.x, y + other.y};
  }

  point operator*(const int64_t other) { return point{x * other, y * other}; }

  auto operator<=>(const point &) const = default;
};

struct point_f {
  double x = 0.0, y = 0.0;

  point_f operator+(const point_f &other) {
    return point_f{x + other.x, y + other.y};
  }

  point_f operator*(const double other) {
    return point_f{x * other, y * other};
  }

  auto operator<=>(const point_f &) const = default;
};

struct rect {
  point tl, br;

  rect(int64_t x_1, int64_t x_2, int64_t y_1, int64_t y_2) {
    tl.x = x_1;
    tl.y = y_1;
    br.x = x_2;
    br.y = y_2;
  }

  auto operator<=>(const rect &) const = default;
};

bool inside(const std::vector<point> &vertices, const rect &r) {
  point_f p = {((double)(r.br.x - r.tl.x) / 2.0f) + (double)r.tl.x,
               ((double)(r.br.y - r.tl.y) / 2.0f) + (double)r.tl.y};
  int c = 0;
  for (int i = 0; i < vertices.size(); i++) {
    point_f p1 = point_f{(double)vertices[i].x, (double)vertices[i].y};
    point_f p2 = point_f{(double)vertices[(i + 1) % vertices.size()].x,
                         (double)vertices[(i + 1) % vertices.size()].y};

    if ((p.y > std::min(p1.y, p2.y)) && (p.y <= std::max(p1.y, p2.y)) &&
        (p.x <= std::max(p1.x, p2.x))) {
      double x_i = (p.y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y) + p1.x;
      if (p1.x == p2.x || p.x <= x_i) {
        c++;
      }
    }
  }
  return c % 2 == 1;
}

auto p1(const std::vector<data> &input) {
  point p{0, 0};
  point dxy{0, 0};

  std::vector<point> vertices;

  // Generate vertices
  for (int i = 0; i < input.size(); i++) {
    const auto &in = input[i];
    switch (in.d) {
    case UP: {
      dxy = {0, -1};
      break;
    }
    case DOWN: {
      dxy = {0, 1};
      break;
    }
    case LEFT: {
      dxy = {-1, 0};
      break;
    }
    case RIGHT: {
      dxy = {1, 0};
      break;
    }
    }

    p = p + (dxy * in.dist);

    if ((i < input.size() - 1 && input[i + 1].d != in.d) ||
        (i == input.size() - 1 && input[0].d != in.d)) {
      vertices.push_back(p);
    }
  }

  // Rearrange vector to start at (0, 0)
  std::ranges::rotate(vertices, vertices.end() - 1);
  vertices.push_back(vertices.front());

  // Check we are going clockwise, otherwise reverse
  if (vertices[1].y != 0) {
    std::ranges::reverse(vertices);
  }

  std::vector<point> fixed_vert = vertices;

  dir curr_dir = UP;
  for (int i = 1; i < vertices.size() - 1; i++) {
    point &curr = vertices[i - 1];
    point &next = vertices[i];
    if (curr.x == next.x) { // Vertical
      assert(curr_dir == RIGHT ||
             curr_dir == LEFT); // SHould only be walking left or right when
                                // hitting vertical wall
      if (curr.y > next.y) {
        curr_dir = UP;
        continue;
      } else {
        // int dx = (curr_dir == LEFT) ? 1 : -1;
        int dx = 1;
        curr_dir = DOWN;
        fixed_vert[i].x += dx;
        fixed_vert[i - 1].x += dx;
      }

    } else if (curr.y == next.y) { // Horizontal
      assert(curr_dir == UP || curr_dir == DOWN);

      if (curr.x < next.x) {
        curr_dir = RIGHT;
        continue;
      } else {
        // int dy = (curr_dir == DOWN) ? 1 : -1;
        int dy = 1;
        curr_dir = LEFT;
        fixed_vert[i].y += dy;
        fixed_vert[i - 1].y += dy;
      }

    } else {
      std::println(
          "ERROR: i={}, vertices.size()={}, curr:({}, {}), next:({}, {})", i,
          vertices.size(), curr.x, curr.y, next.x, next.y);
    }
  }

  vertices = fixed_vert;

  // Get unique x and y
  std::vector<int64_t> u_x;
  std::vector<int64_t> u_y;
  for (const auto &vert : vertices) {
    if (std::ranges::find(u_x, vert.x) == u_x.end()) {
      u_x.push_back(vert.x);
    }

    if (std::ranges::find(u_y, vert.y) == u_y.end()) {
      u_y.push_back(vert.y);
    }
  }

  // Sort
  std::ranges::sort(u_x);
  std::ranges::sort(u_y);

  // Generate rectangles
  std::vector<rect> rects;

  // Generate view of adjacent values in vector as points
  auto get_pairs = [](const std::vector<int64_t> &r) {
    return r | std::ranges::views::pairwise |
           std::ranges::views::transform([](const auto &pair) {
             const auto &[prev_val, curr_val]{pair};
             return point{prev_val, curr_val};
           });
  };

  // Cartesian product to get all possible rectangles
  // e.g. first two x values and first two y values generate top left rectangle,
  // second and third x and first two y generate the rectangle next to that one
  // first two x and last two y generate bottom left rectangle
  // last two x and last two y generate bottom right rectangle
  // etc...
  std::ranges::transform(
      std::ranges::views::cartesian_product(get_pairs(u_x), get_pairs(u_y)),
      std::back_inserter(rects), [](const auto tup) {
        const auto &[a, b]{tup};
        return rect{a.x, a.y, b.x, b.y};
      });

  // for (const auto &r : rects) {
  //   std::println("rect -> tl:({}, {}), br: ({}, {})", r.tl.x, r.tl.y, r.br.x,
  //                r.br.y);
  // }

  return std::ranges::fold_left(
      rects | std::views::transform([&](const auto &r) {
        int64_t area = (int64_t)(r.br.x - r.tl.x) * (int64_t)(r.br.y - r.tl.y);
        return inside(vertices, r) ? area : 0;
      }),
      0ull, std::plus{});
}

auto p2(const std::vector<data> &input) {

  std::vector<data> in_2;
  for (const auto &d : input) {
    data dat;
    dat.d = [&](const std::string &s) {
      switch (s[5]) {
      case '0':
        return RIGHT;
      case '1':
        return DOWN;
      case '2':
        return LEFT;
      case '3':
        return UP;
      };
      return UP;
    }(d.col);
    std::istringstream conv(d.col.substr(0, 5));
    conv >> std::hex >> dat.dist;
    in_2.push_back(dat);
  }

  return p1(in_2);
}

int main() {
  auto in = getInput();

  std::println("Part 1: {}", p1(in));
  std::println("Part 2: {}", p2(in));
}

/*

Part 1: 48503
Part 2: 148442153147147

________________________________________________________
Executed in  219.86 millis    fish           external
   usr time  211.04 millis  324.00 micros  210.72 millis
   sys time    7.19 millis  237.00 micros    6.95 millis



* */
