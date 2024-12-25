#include <cctype>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Position
{
    int row, col;

    std::string to_string() const
    {
        return std::format("Position{{ .row={}, .col={} }}", row, col);
    }
};

struct Board
{
    int n_rows, n_cols;

    bool is_valid_position(const Position& position) const
    {
        return position.row < n_rows && position.row >= 0 &&
               position.col < n_cols && position.col >= 0;
    }

    int get_index(const Position& position) const
    {
        // with newlines included = n_cols + 1
        return position.row * (n_cols + 1) + position.col;
    }
};

typedef std::unordered_map<char, std::vector<Position>> AntennaPositions;

#ifdef TEST
const char* test_input = "............\n"
                         "........0...\n"
                         ".....0......\n"
                         ".......0....\n"
                         "....0.......\n"
                         "......A.....\n"
                         "............\n"
                         "............\n"
                         "........A...\n"
                         ".........A..\n"
                         "............\n"
                         "............\n";
#endif
int main()
{
#ifdef TEST
    std::istringstream input{ test_input };
#else
    std::ifstream input{ "./input/08.txt" };
#endif

    AntennaPositions antennae;

    int row = 0;
    int n_cols;
    std::string line;
    while (std::getline(input, line)) {
        if (row == 0) {
            n_cols = line.length();
        }

        if (line.empty()) {
            continue;
        }

        for (int col = 0; col < n_cols; col++) {
            char ch = line[col];

            if (ch != '.') {
                antennae[ch].emplace_back(Position{ .row = row, .col = col });
            }
        }

        row++;
    }

    Board board{ row, n_cols };
    std::unordered_set<int> unique_positions;

    for (const auto& [frequency, antena_positions] : antennae) {
        for (auto a = antena_positions.begin(); a != antena_positions.end();
             a++) {
            for (auto b = std::next(a); b != antena_positions.end(); b++) {
                Position a2b_vec{ .row = b->row - a->row,
                                  .col = b->col - a->col };

                // make a negative vector a add it to the point a
                Position pos_from_a{ .row = a->row - a2b_vec.row,
                                     .col = a->col - a2b_vec.col };

                if (board.is_valid_position(pos_from_a)) {
                    unique_positions.insert(board.get_index(pos_from_a));
                }

                Position pos_from_b{ .row = b->row + a2b_vec.row,
                                     .col = b->col + a2b_vec.col };

                if (board.is_valid_position(pos_from_b)) {
                    unique_positions.insert(board.get_index(pos_from_b));
                }
            }
        }
    }

    std::cout << "Antinode count = " << unique_positions.size() << '\n';
}
