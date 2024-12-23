#include "./utils.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_set>

enum PositionOccupancy
{
    GUARD = '^',
    OBSTACLE = '#',
    INSERTED_OBSTACLE = 'O',
    EMPTY = '.',
};

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

    void move(const Direction direction)
    {
        switch (direction) {
            case NORTH:
                row--;
                break;
            case EAST:
                col++;
                break;
            case SOUTH:
                row++;
                break;
            case WEST:
                col--;
                break;
        }
    }
};

struct Guard
{
    Position position;
    Direction direction;

    void rotate_right()
    {
        switch (direction) {
            case NORTH:
                direction = EAST;
                break;
            case EAST:
                direction = SOUTH;
                break;
            case SOUTH:
                direction = WEST;
                break;
            case WEST:
                direction = NORTH;
                break;
        }
    }
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

    size_t get_index(const Position& pos) const
    {
        assert(pos.row < n_rows);
        assert(pos.col < n_cols);

        return (pos.row * n_cols) + pos.col;
    }

    bool is_valid_position(const Position& pos) const
    {
        // The size_t type is unsigned and will therefor
        // underflow if the guard leaves the top or left
        // part of the board.
        // -> Making it larger then n_rows of n_cols respectivelly.

        return pos.row < n_rows && pos.col < n_cols;
    }

    Position index_to_position(size_t index) const
    {
        return {
            .row = index / n_rows,
            .col = index % n_cols,
        };
    }

    Guard get_initial_guard() const
    {
        return {
            .position = index_to_position(guard_position),
            .direction = NORTH,
        };
    }

    bool has_obstacle_at(const Position& pos)
    {
        char board_value = data[get_index(pos)];

        return board_value == OBSTACLE || board_value == INSERTED_OBSTACLE;
    }
};

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

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

    while (board.is_valid_position(guard.position)) {
        Position new_position = guard.position;

        // move to the new appropriate position
        new_position.move(guard.direction);

        // The new position is outside of the board space.
        //  -> This is the end state.
        if (!board.is_valid_position(new_position)) {
            break;
        }

        if (board.has_obstacle_at(new_position)) {
            guard.rotate_right();

            continue;
        }

        // The new position is valid and not final.
        guard.position = new_position;
        visited_positions.insert(board.get_index(new_position));
    }

    std::cout << "Positions traversed: " << visited_positions.size() << '\n';

    if (!is_second_solution(argc, argv)) {
        return 0;
    }

    // Now we have all the possible positions.
    //  -> Try to insert obstacles at each of the positions
    //     the guard can travel through.
    //  -> Find the ones that cause the guard
    //     to get stuck inside a loop.
    int positions_causing_loops = 0;

    // First remove the initial guard position.
    // The only one, where an obstacle can't be placed.
    visited_positions.erase(board.guard_position);

    // Save the guard state when it encounters
    // an obstacle.
    //  -> If this position is visited more than once,
    //     the guard is stuck inside of a loop.
    std::unordered_map<size_t, Direction> obstacle_states;

    for (size_t position : visited_positions) {
        assert(board.data[position] == EMPTY);

        guard = board.get_initial_guard();
        obstacle_states.clear();

        // Insert the obstacle...
        board.data[position] = INSERTED_OBSTACLE;

        bool will_loop = false;

        // Traverse the same way as before
        while (board.is_valid_position(guard.position)) {
            Position new_position = guard.position;
            new_position.move(guard.direction);

            // The new position is outside of the board space.
            //  -> Failed with placement of this obstacle,
            //     the guard can escape the game plane.
            //  -> Continue with trying other positions.
            if (!board.is_valid_position(new_position)) {
                break;
            }

            size_t new_position_index = board.get_index(new_position);

            if (board.has_obstacle_at(new_position)) {
                // This position was already visited
                // and the guard was facing the same direction.
                //  -> Stuck inside a loop.
                if (obstacle_states.contains(new_position_index) &&
                    obstacle_states[new_position_index] == guard.direction) {
                    will_loop = true;

                    break;
                }

                // Save the direction in which the guard
                // encountered the current obstacle.
                obstacle_states[new_position_index] = guard.direction;

                guard.rotate_right();

                continue;
            }

            // Continue with the new position.
            guard.position = new_position;
        }

        board.data[position] = EMPTY;

        if (will_loop) {
            positions_causing_loops++;
        }
    }

    std::cout << "Positions causing loops: " << positions_causing_loops << '\n';
}
