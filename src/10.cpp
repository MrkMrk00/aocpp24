#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

struct Pos
{
    int row, col;
};

struct Board
{
    std::string data;
    int n_rows, n_cols;

    explicit Board(const std::string& input)
      : data{ input }
      , n_rows{ 0 }
      , n_cols{ 0 }
    {
        n_cols = input.find('\n');

        size_t newline_pos;
        while ((newline_pos = data.find('\n')) != std::string::npos) {
            n_rows++;
            data.erase(newline_pos, 1);
        }
    }

    size_t get_index(Pos position) const
    {
        return position.row * n_cols + position.col;
    }

    Pos index_to_pos(size_t index) const
    {
        return {
            .row = static_cast<int>(index / n_cols),
            .col = static_cast<int>(index % n_cols),
        };
    }

    bool is_valid(Pos position) const
    {
        return position.row >= 0 && position.row < n_rows &&
               position.col >= 0 && position.col < n_cols;
    }

    void find_valid_next_positions(std::vector<Pos>& neighbors,
                                   Pos position) const
    {
        char at_current = data[get_index(position)];

        Pos n0{ position.row - 1, position.col };
        if (is_valid(n0) && data[get_index(n0)] == at_current + 1) {
            neighbors.push_back(n0);
        }

        Pos n1{ position.row + 1, position.col };
        if (is_valid(n1) && data[get_index(n1)] == at_current + 1) {
            neighbors.push_back(n1);
        }
        Pos n2{ position.row, position.col - 1 };
        if (is_valid(n2) && data[get_index(n2)] == at_current + 1) {
            neighbors.push_back(n2);
        }

        Pos n3{ position.row, position.col + 1 };
        if (is_valid(n3) && data[get_index(n3)] == at_current + 1) {
            neighbors.push_back(n3);
        }
    }
};

int main(int argc, char** argv)
{
#ifdef TEST
    std::istringstream input{
        "89010123\n"
        "78121874\n"
        "87430965\n"
        "96549874\n"
        "45678903\n"
        "32019012\n"
        "01329801\n"
        "10456732\n",
    };
#else
    std::ifstream input{ "./input/10.txt" };
#endif

    std::ostringstream oss;
    oss << input.rdbuf();

    Board board{ oss.str() };

    int trailend_count = 0;

    std::vector<Pos> to_visit;
    std::vector<size_t> visited;

    for (size_t i = 0; i < board.data.size(); i++) {
        if (board.data[i] != '0') {
            continue;
        }

        size_t trailhead_index = i;
        Pos trailhead = board.index_to_pos(trailhead_index);

        to_visit.clear();
        visited.clear();

        to_visit.push_back(trailhead);

        while (!to_visit.empty()) {
            Pos current = to_visit[to_visit.size() - 1];
            to_visit.pop_back();

            size_t current_index = board.get_index(current);
            if (!is_second_solution(argc, argv) &&
                std::find(visited.begin(), visited.end(), current_index) !=
                  visited.end()) {
                continue;
            }

            visited.push_back(current_index);
            board.find_valid_next_positions(to_visit, current);
        }

        for (size_t index : visited) {
            if (board.data[index] == '9') {
                trailend_count++;
            }
        }
    }

    std::cout << "Trailend count = " << trailend_count << std::endl;
}
