#include "./utils.h"

#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

struct Board
{
    std::string_view data;
    size_t n_rows, n_cols;

    explicit Board(const std::string& data_)
      : data{ data_ }
    {
        // find the newline character
        // subtract 1
        //  -> get the number of columns with data inside
        n_rows = data_.find('\n');
        n_cols = (data_.size() / n_rows) - 1;
    }

    char at(size_t row, size_t col) const
    {
        return data[get_data_index(row, col)];
    }

    /**
     * @return the pointer into the string at the given position
     */
    const char* get_data_offset(size_t row, size_t col) const
    {
        return data.data() + get_data_index(row, col);
    }

    size_t get_data_index(size_t row, size_t col) const
    {
        assert(row < n_rows && col < n_cols);

        return row * (n_cols + 1) + col;
    }

    std::string to_string() const
    {
        return std::format("Board{{n_rows: {}, n_cols: {}}}", n_rows, n_cols);
    }
};

// Jo, použiju to jednou, ale aspoň se trochu naučim s C++ STL :).
// Stejně tak polovina metod v tý classe Board...
bool reverse_equals(const std::string_view& one, const std::string_view& second)
{
    if (one.length() != second.length()) {
        return false;
    }

    for (size_t i = 0, j = second.length() - 1; i < one.length() && j >= 0;
         i++, j--) {
        if (one[i] != second[j]) {
            return false;
        }
    }

    return true;
}

int solve_and_print_a(const Board& board)
{
    const std::string_view needle = "XMAS";

    const size_t max_col = board.n_cols - needle.length();
    const size_t max_row = board.n_rows - needle.length();

    int count = 0;

    // ->, <- Inside row (backwards and forwards) {{{
    std::cout << "Inside row (backwards and forwards):\n\n";

    for (size_t row = 0; row < board.n_rows; row++) {
        std::cout << std::format("row {}\t", row);

        for (size_t col = 0; col <= max_col; col++) {
            std::string_view maybe_phrase{ board.get_data_offset(row, col),
                                           needle.length() };

            if (maybe_phrase == needle ||
                reverse_equals(maybe_phrase, needle)) {
                std::cout << std::format("!{}!", maybe_phrase);
                count++;
            } else {
                std::cout << maybe_phrase;
            }

            std::cout << '\t';
        }
        std::cout << '\n';
    }
    // }}}

    std::cout << '\n';

    // v A Inside column (backwards and forwards) {{{
    std::cout << "Inside column (backwards and forwards):\n\n";

    for (size_t col = 0; col < board.n_cols; col++) {
        std::cout << std::format("col {}\t", col);

        for (size_t row = 0; row <= max_row; row++) {
            // Compare strings -> LTR {{{
            bool matches_forwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col);

                if (from_board != needle[offset]) {
                    matches_forwards = false;

                    break;
                }
            }

            if (matches_forwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    std::cout << board.at(row + offset, col);
                }
                std::cout << '!' << '\t';
                count++;

                continue;
            }
            // }}}

            // Compare strings <- RTL {{{
            bool matches_backwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col);

                if (from_board != needle[needle.length() - offset - 1]) {
                    matches_backwards = false;

                    break;
                }
            }

            if (matches_backwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    std::cout << board.at(row + offset, col);
                }
                std::cout << '!' << '\t';
                count++;

                continue;
            }
            // }}}

            // String was not matched -> just print it and continue
            for (size_t offset = 0; offset < needle.length(); offset++) {
                std::cout << board.at(row + offset, col);
            }
            std::cout << '\t';
        }
        std::cout << '\n';
    }
    // }}}

    // Diagonally \ {{{
    //
    std::cout << '\n' << "Diagonally \\ \n" << '\n';
    for (size_t row = 0; row <= max_row; row++) {
        std::cout << std::format("row {}\t", row);
        for (size_t col = 0; col <= max_col; col++) {

            bool matches_forwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col + offset);

                if (from_board != needle[offset]) {
                    matches_forwards = false;

                    break;
                }
            }

            if (matches_forwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    char from_board = board.at(row + offset, col + offset);
                    std::cout << from_board;
                }
                std::cout << '!' << '\t';

                count++;

                continue;
            }

            bool matches_backwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col + offset);

                if (from_board != needle[needle.length() - offset - 1]) {
                    matches_backwards = false;

                    break;
                }
            }

            if (matches_backwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    char from_board = board.at(row + offset, col + offset);
                    std::cout << from_board;
                }
                std::cout << '!' << '\t';

                count++;

                continue;
            }

            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col + offset);
                std::cout << from_board;
            }
            std::cout << '\t';
        }
        std::cout << '\n';
    }
    // }}}

    // Diagonally / {{{
    std::cout << '\n' << "Diagonally / \n" << '\n';
    for (size_t row = 0; row <= max_row; row++) {
        std::cout << std::format("row {}\t", row);

        size_t min_col = needle.length() - 1;
        for (size_t col = board.n_cols - 1; col >= min_col; col--) {

            bool matches_forwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col - offset);

                if (from_board != needle[offset]) {
                    matches_forwards = false;

                    break;
                }
            }

            if (matches_forwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    char from_board = board.at(row + offset, col - offset);
                    std::cout << from_board;
                }
                std::cout << '!' << '\t';

                count++;

                continue;
            }

            bool matches_backwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col - offset);

                if (from_board != needle[needle.length() - offset - 1]) {
                    matches_backwards = false;

                    break;
                }
            }

            if (matches_backwards) {
                std::cout << '!';
                for (size_t offset = 0; offset < needle.length(); offset++) {
                    char from_board = board.at(row + offset, col - offset);
                    std::cout << from_board;
                }
                std::cout << '!' << '\t';

                count++;

                continue;
            }

            for (size_t offset = 0; offset < needle.length(); offset++) {
                char from_board = board.at(row + offset, col - offset);
                std::cout << from_board;
            }

            std::cout << '\t';
        }
        std::cout << '\n';
    }

    std::cout << '\n';
    std::cout << "Count: " << count << '\n';

    return 0;
}

struct Position
{
    size_t row, col;
};

struct MasOccurrence
{
    std::array<Position, 3> positions{};

    bool intersects(const MasOccurrence& other) const
    {
        return other.positions[1].col == positions[1].col &&
               other.positions[1].row == positions[1].row;
    }
};

// Bylo by jednodušší a rychleší ukládat jen prostřední char
// do nějaký mapy a pak vybrat jen klíče, které > 1.
// Ale takhle to je jednodušší debuggovat, kdybych to někde
// napsal špatně.
int solve_and_print_b(const Board& board)
{
    const std::string needle = "MAS";
    std::vector<MasOccurrence> occurrences;

    const size_t max_row = board.n_rows - needle.length();
    const size_t max_col = board.n_cols - needle.length();

    // \ Diagonally

    for (size_t row = 0; row <= max_row; row++) {
        for (size_t col = 0; col <= max_col; col++) {
            MasOccurrence occ;

            bool matches_forwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                if (board.at(row + offset, col + offset) != needle[offset]) {
                    matches_forwards = false;
                }

                occ.positions[offset] = { .row = row + offset,
                                          .col = col + offset };
            }

            if (matches_forwards) {
                occurrences.push_back(std::move(occ));

                continue;
            }

            bool matches_backwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                if (board.at(row + offset, col + offset) !=
                    needle[needle.length() - offset - 1]) {
                std::internal(ios_base &base)
                    matches_backwards = false;
                }

                occ.positions[offset] = { .row = row + offset,
                                          .col = col + offset };
            }

            if (matches_backwards) {
                occurrences.push_back(std::move(occ));
            }
        }
    }

    // / Diagonally
    for (size_t row = 0; row <= max_row; row++) {
        for (size_t col = board.n_cols - 1; col >= needle.length() - 1; col--) {
            MasOccurrence occ;

            bool matches_forwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                if (board.at(row + offset, col - offset) != needle[offset]) {
                    matches_forwards = false;
                }

                occ.positions[offset] = { .row = row + offset,
                                          .col = col - offset };
            }

            if (matches_forwards) {
                occurrences.push_back(std::move(occ));

                continue;
            }

            bool matches_backwards = true;
            for (size_t offset = 0; offset < needle.length(); offset++) {
                if (board.at(row + offset, col - offset) !=
                    needle[needle.length() - offset - 1]) {
                    matches_backwards = false;
                }

                occ.positions[offset] = { .row = row + offset,
                                          .col = col - offset };
            }

            if (matches_backwards) {
                occurrences.push_back(std::move(occ));
            }
        }
    }

    for (const auto& occ : occurrences) {
        std::cout << std::format("Occurence ({}, {}), ({}, {}), ({}, {})\n",
                                 occ.positions[0].row,
                                 occ.positions[0].col,
                                 occ.positions[1].row,
                                 occ.positions[1].col,
                                 occ.positions[2].row,
                                 occ.positions[2].col);
    }

    int intersections_count = 0;

    for (size_t i = 0; i < occurrences.size(); i++) {
        const MasOccurrence& ith_occ = occurrences[i];

        for (size_t j = i + 1; j < occurrences.size(); j++) {
            if (ith_occ.intersects(occurrences[j])) {
                intersections_count++;
            }
        }
    }

    std::cout << "Intersections count: " << intersections_count << '\n';

    return 0;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
#ifdef TEST
    std::string input{ "MMMSXXMASM\n"
                       "MSAMXMSMSA\n"
                       "AMXSXMAAMM\n"
                       "MSAMASMSMX\n"
                       "XMASAMXAMM\n"
                       "XXAMMXXAMA\n"
                       "SMSMSASXSS\n"
                       "SAXAMASAAA\n"
                       "MAMMMXMMMM\n"
                       "MXMXAXMASX\n" };
#else
    std::ifstream input_stream{ "./input/04.txt" };
    std::ostringstream oss;
    oss << input_stream.rdbuf();
    std::string input = oss.str();
#endif

    Board board{ input };

    if (!is_second_solution(argc, argv)) {
        return solve_and_print_a(board);
    } else {
        return solve_and_print_b(board);
    }
}
