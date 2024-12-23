#include <cassert>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_set>

enum Direction
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
};

struct Position
{
    size_t row, col;
};

struct Guard
{
    Position position;
    Direction direction;
};

struct Board
{
    std::string data;
    size_t n_rows, n_cols;
    size_t guard_position;

    explicit Board(std::istream& stream)
    {
        std::ostringstream oss;
        oss << stream.rdbuf();

        data = oss.str();
        n_rows = data.find('\n');
        n_cols = (data.size() / n_rows) - 1;

        // remove the newline characters
        size_t newline_pos;
        while ((newline_pos = data.find('\n')) != std::string::npos) {
            data.erase(newline_pos, 1);
        }

        guard_position = data.find('^');
        data[guard_position] = '.';
    }

    size_t get_index(Position pos) const
    {
        assert(pos.row < n_rows);
        assert(pos.col < n_cols);

        return (pos.row * n_cols) + pos.col;
    }

    bool is_valid_pos(Position pos) const
    {
        // The size_t type is unsigned and will therefor
        // underflow if the guard leaves the top or left
        // part of the board.
        // -> Making it larger then n_rows of n_cols respectivelly.

        return pos.row < n_rows && pos.col < n_cols;
    }

    Position index_to_pos(size_t index) const
    {
        return {
            .row = index / n_rows,
            .col = index % n_cols,
        };
    }

    Guard get_initial_guard() const
    {
        return {
            .position = index_to_pos(guard_position),
            .direction = NORTH,
        };
    }
};

int main()
{
#ifdef TEST
    std::istringstream input{ "....#.....\n"
                              ".........#\n"
                              "..........\n"
                              "..#.......\n"
                              ".......#..\n"
                              "..........\n"
                              ".#..^.....\n"
                              "........#.\n"
                              "#.........\n"
                              "......#...\n" };
#else
    std::ifstream input{ "./input/06.txt" };
#endif

    Board board{ input };
    Guard guard = board.get_initial_guard();

    // The positions can be visited multiple times -> a set is needed.
    // (Or a vector would suffice, the puzzle input isn't too large).
    // (Or a bitset, but there would be a hardcoded size.)
    std::unordered_set<size_t> visited_positions;

    // insert the initial position of the guard
    // "...Including the guard's starting position..."
    visited_positions.insert(board.get_index(guard.position));

    while (board.is_valid_pos(guard.position)) {
        Position new_position = guard.position;
        switch (guard.direction) {
            case NORTH:
                new_position.row--;
                break;
            case EAST:
                new_position.col++;
                break;
            case SOUTH:
                new_position.row++;
                break;
            case WEST:
                new_position.col--;
                break;
        }

        // the new position is outside of the board space
        if (!board.is_valid_pos(new_position)) {
            break;
        }

        // the new positions contains an obstacle
        //  -> turn right and continue
        if (board.data[board.get_index(new_position)] == '#') {
            switch (guard.direction) {
                case NORTH:
                    guard.direction = EAST;
                    break;
                case EAST:
                    guard.direction = SOUTH;
                    break;
                case SOUTH:
                    guard.direction = WEST;
                    break;
                case WEST:
                    guard.direction = NORTH;
                    break;
            }

            continue;
        }

        // The new position is valid -> assign it to the guard
        // and add it to the set of visited positions.
        guard.position = new_position;
        visited_positions.insert(board.get_index(new_position));
    }

    std::cout << "Positions traversed: " << visited_positions.size() << '\n';
}
