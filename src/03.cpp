#include "03.h"

#include <cassert>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <string_view>
#include <format>

static inline bool is_terminating_char(char ch)
{
    return ch == '\0' || ch == EOF;
}

static std::string stream_into_string(const std::istream& stream)
{
    std::ostringstream result;
    result << stream.rdbuf();

    return result.str();
}

int main()
{
#ifdef TEST
    std::istringstream input_stream{
        "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]"
        "then(mul(11,8)mul(8,5))"
    };
#else
    std::ifstream input_stream{ "./input/03.txt" };
#endif

    parse_calls(input_stream, { "mul" });

    const std::string input = stream_into_string(input_stream);

    const int max_pos_in_str = input.length() - 1;

    int instruction_sum = 0;

    uint32_t position = 0;
    while (true) {
        char c = input[position];
        if (is_terminating_char(c)) {
            break;
        }

        if (c != 'm') {
            position++;

            continue;
        }

        const std::string target_identifier = "mul";
        const size_t identifier_len = target_identifier.length();

        // check if there is enough characters left in the string
        // for the 'mul' identifier
        if ((position + identifier_len) > max_pos_in_str) {
            break;
        }

        std::string_view maybe_identifier{ input.data() + position,
                                           identifier_len };

        if (maybe_identifier != target_identifier) {
            position++;

            continue;
        }

        position += maybe_identifier.length();

        // read arguments - 2 comma separated integers
        // If the current characted isn't opening brace, then increment the
        // position and leave.
        if (input[position] != '(') {
            std::cerr << std::format(
              "Unexpected token: expected = '('; got = '{}'\n",
              input[position]);
            position++;

            continue;
        }

        uint32_t position_arg = position + 1;
        std::string arguments = "";

        char c_arg;
        while (::isdigit(c_arg = input[position_arg++]) || c_arg == ',') {
            arguments += c_arg;
        }

        if (is_terminating_char(c_arg)) {
            break;
        }

        // if the current argument (the one, that ended the loop) isn't a
        // closing brace
        if (c_arg != ')') {
            std::cerr << std::format(
              "Unexpected token: expected = ')'; got = '{}'\n",
              input[position_arg]);
            position++;

            continue;
        }

        std::istringstream arguments_stream{ arguments };
        int first, second;

        if (!(arguments_stream >> first) ||
            !(arguments_stream.ignore(1, ',')) ||
            !(arguments_stream >> second)) {
            std::cerr << std::format("Failed to read arguments list \"{}\"!\n",
                                     arguments);
            position++;

            continue;
        }

        std::cout << std::format("First: {}; second: {}\n", first, second);
        std::cout << std::format("Arguments: {}\n", arguments);
        std::cout << std::format("Identifier: {}\n", maybe_identifier);
        std::cout << "\n";

        instruction_sum += first * second;
    }

    std::cout << std::format("instruction_sum: {}\n", instruction_sum);
}
