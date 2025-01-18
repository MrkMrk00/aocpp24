#include <deque>
#include <fstream>
#include <iostream>
#include <ostream>
#include <print>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class Region
{
  public:
    const char letter;

    explicit Region(char letter, size_t n_cols)
      : letter{ letter }
      , n_cols{ n_cols }
    {
    }

    void add_position(size_t position)
    {
        positions.push_back(position);
    }

    inline uint32_t area() const
    {
        return positions.size();
    }

    uint32_t perimeter() const
    {
        uint32_t perimeter = 0;

        for (auto a : positions) {
            int n_neighbors = 0;

            for (auto b : positions) {
                // right:
                // A B
                if ((b % n_cols) > 0 && b - 1 == a) {
                    n_neighbors++;
                }
                // down:
                // A
                // B
                else if (b >= n_cols && b - n_cols == a) {
                    n_neighbors++;
                }
                // left:
                // B A
                else if ((a % n_cols) > 0 && a - 1 == b) {
                    n_neighbors++;
                }
                // up:
                // B
                // A
                else if (a >= n_cols && a - n_cols == b) {
                    n_neighbors++;
                }
            }

            perimeter += 4 - n_neighbors;
        }

        return perimeter;
    }

  private:
    std::vector<size_t> positions;
    const size_t n_cols;
};

int main()
{
#ifdef TEST
    std::istringstream input{
        "AAAA\n"
        "BBCD\n"
        "BBCC\n"
        "EEEC\n",
    };
    (void)input;

    std::istringstream input2{
        "RRRRIICCFF\n"
        "RRRRIICCCF\n"
        "VVRRRCCFFF\n"
        "VVRCCCJFFF\n"
        "VVVVCJJCFE\n"
        "VVIVCCJJEE\n"
        "VVIIICJJEE\n"
        "MIIIIIJJEE\n"
        "MIIISIJEEE\n"
        "MMMISSJEEE\n",
    };
    (void)input2;
#else
    std::ifstream input{"./input/12.txt"};
#endif

    std::ostringstream oss;
    oss << input.rdbuf();
    std::string board = oss.str();
    size_t n_cols = board.find('\n');

    size_t new_line_pos = std::string::npos;
    while ((new_line_pos = board.find('\n')) != std::string::npos) {
        board.erase(board.begin() + new_line_pos);
    }

    std::vector<Region> regions;

    std::unordered_set<size_t> visited;

    std::deque<size_t> to_visit;
    std::vector<size_t> to_visit_local;
    to_visit.push_back(0);

    while (!to_visit.empty()) {
        size_t base_idx = to_visit.front();
        to_visit.pop_front();

        if (visited.contains(base_idx)) {
            continue;
        }

        char letter = board[base_idx];
        // find neighbors

        Region region{ letter, n_cols };

        to_visit_local.clear();
        to_visit_local.push_back(base_idx);

        while (!to_visit_local.empty()) {
            size_t current_idx = to_visit_local.back();
            to_visit_local.pop_back();

            if (visited.contains(current_idx)) {
                continue;
            }

            region.add_position(current_idx);
            visited.insert(current_idx);

            bool has_right = (current_idx + 1) % n_cols != 0;
            if (has_right && !visited.contains(current_idx + 1)) {
                size_t new_idx = current_idx + 1;

                if (board[new_idx] == region.letter) {
                    to_visit_local.push_back(new_idx);
                } else {
                    to_visit.push_back(new_idx);
                }
            }

            bool has_down = (current_idx + n_cols) < board.size();
            if (has_down && !visited.contains(current_idx + n_cols)) {
                size_t new_idx = current_idx + n_cols;

                if (board[new_idx] == region.letter) {
                    to_visit_local.push_back(new_idx);
                } else {
                    to_visit.push_back(new_idx);
                }
            }

            bool has_left = current_idx % n_cols != 0;
            if (has_left && !visited.contains(current_idx - 1)) {
                size_t new_idx = current_idx - 1;

                if (board[new_idx] == region.letter) {
                    to_visit_local.push_back(new_idx);
                } else {
                    to_visit.push_back(new_idx);
                }
            }

            bool has_up = current_idx >= n_cols;
            if (has_up && !visited.contains(current_idx - n_cols)) {
                size_t new_idx = current_idx - n_cols;

                if (board[new_idx] == region.letter) {
                    to_visit_local.push_back(new_idx);
                } else {
                    to_visit.push_back(new_idx);
                }
            }
        }

        regions.push_back(std::move(region));
    }

    uint32_t price = 0;
    for (const auto& region : regions) {
        price += region.area() * region.perimeter();

        std::println("Region: {}, {} * {} = {}",
                     region.letter,
                     region.area(),
                     region.perimeter(),
                     region.area() * region.perimeter());
    }

    std::println();
    std::println("Price = {}", price);
}
