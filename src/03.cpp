#include "utils.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <fstream>
#include <iostream>
#include <istream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

static bool is_terminating_char(char ch)
{
    return ch == '\0' || ch == EOF;
}

struct Call
{
    const std::string identifier{};
    std::vector<int> arguments{};
};

static std::vector<Call> parse_calls(
  const std::string& input,
  const std::vector<std::string>& valid_identifiers)
{
    std::set<char> first_chars;

    // initialize a set with first chars of all valid identifiers
    // somewhat speedup lookup?
    for (const std::string& identifier : valid_identifiers) {
        first_chars.insert(identifier[0]);
    }

    std::vector<Call> calls;

    int position = 0;
    char ch;
next_char:
    while (!is_terminating_char(ch = input[position++])) {
        if (!first_chars.contains(ch)) {
            continue;
        }

        int current_position = position - 1;
        std::string found_identifier = "";
        for (const std::string& identifier : valid_identifiers) {
            // if the identifier would end after the input string ends
            if ((current_position + identifier.length()) >= input.length()) {
                break;
            }

            std::string_view maybe_identifier{ input.data() + current_position,
                                               identifier.length() };

            // the identifier in the string matches the searched
            if (maybe_identifier == identifier) {
                found_identifier = maybe_identifier;
            }
        }

        // no identifier matched for the current position
        // or the potencial "()" do not fit inside the input string

        size_t position_after_identifier_with_args =
          current_position + found_identifier.length() + 2;

        if (found_identifier == "" ||
            position_after_identifier_with_args > input.length()) {
            continue;
        }

        // parse the arguments of the "function call"

        current_position += found_identifier.length();
        if (input[current_position] != '(') {
#ifdef TEST
            std::cerr << std::format(
              "[Err] Invalid token: \"{}\" (expected \"{}\")\n",
              input[current_position],
              '(');
#endif

            // maybe increment the position by more then 1 char here?
            continue;
        }

        std::ostringstream args_oss;
        char ch_arg;
        while ((ch_arg = input[++current_position]) != ')') {
            // invalid argument format
            if (!::isdigit(ch_arg) && ch_arg != ',') {
                // Yes goto :), otherwise I would have to check
                // if the args_oss stream is complete and valid.
                // This is far easier (and arguably more readable).
                goto next_char;
            }

            args_oss << ch_arg;
        }

        std::string arguments = args_oss.str();
        int arguments_count = std::ranges::count(arguments, ',');
        if (arguments_count != 0) {
            arguments_count++;
        }

        if (arguments_count == 0) {
            calls.emplace_back(std::move(found_identifier));

            continue;
        }

        if (arguments_count == 2) {
            std::istringstream args_stream{ arguments };

            int first, second;
            assert(args_stream >> first);
            assert(args_stream.ignore(1, ','));
            assert(args_stream >> second);

            calls.emplace_back(std::move(found_identifier),
                               std::vector<int>{ first, second });

            continue;
        }

        std::cerr << std::format("[Err] Invalid format of arguments: {}\n",
                                 arguments);
    }

    for (const auto& item : calls) {
        std::string arguments = vec_to_string(item.arguments);
        std::cout << std::format("Call{{ identifer: {}, arugments: {} }}\n",
                                 item.identifier,
                                 arguments);
    }

    return calls;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

#ifdef TEST
    std::string input;
    if (is_second_solution(argc, argv)) {
        input = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo("
                ")?mul(8,5))";
    } else {
        input = "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]"
                "then(mul(11,8)mul(8,5))";
    }
#else
    std::ifstream input_stream{ "./input/03.txt" };
    std::ostringstream oss;
    oss << input_stream.rdbuf();
    std::string input = oss.str();
#endif

    // First part of the puzzle
    if (!is_second_solution(argc, argv)) {
        int instruction_sum = 0;
        std::vector<Call> calls = parse_calls(input, { "mul" });
        for (const Call& call : calls) {
            // there will always be 2 arugments for the `mul` identifier
            instruction_sum += call.arguments.at(0) * call.arguments.at(1);
        }

        std::cout << std::format("instruction_sum = {}\n", instruction_sum);
    } else {
        int instruction_sum = 0;
        bool instruction_enabled = true;
        std::vector<Call> calls = parse_calls(input, { "mul", "do", "don't" });

        for (const Call& call : calls) {
            if (call.identifier == "do") {
                instruction_enabled = true;

                continue;
            }

            if (call.identifier == "don't") {
                instruction_enabled = false;

                continue;
            }

            // if (call.identifier == "mul") {...

            std::cout << std::format("enabled=({})\t:: {} * {}\n",
                                     instruction_enabled,
                                     call.arguments.at(0),
                                     call.arguments.at(1));

            // instruction should be enabled
            if (!instruction_enabled) {
                continue;
            }

            instruction_sum += call.arguments.at(0) * call.arguments.at(1);
        }

        std::cout << std::format("instruction_sum = {}\n", instruction_sum);
    }
}
