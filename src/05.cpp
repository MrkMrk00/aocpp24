#include "utils.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int NOT_FOUND = -1;

static int index_of(const std::vector<int>& haystack, int needle)
{
    auto offset = std::find(haystack.begin(), haystack.end(), needle);
    if (offset == haystack.end()) {
        return NOT_FOUND;
    }

    return offset - haystack.begin();
}

struct Rule
{
    const int first, second;

    bool validate(const std::vector<int>& update) const
    {
        int first_idx = index_of(update, first);
        if (first_idx == NOT_FOUND) {
            return true;
        }

        int second_idx = index_of(update, second);
        if (second_idx == NOT_FOUND) {
            return true;
        }

        return first_idx < second_idx;
    }
};

static bool is_update_valid(const std::vector<Rule>& rules,
                            const std::vector<int>& update)
{
    for (const Rule& rule : rules) {
        if (!rule.validate(update)) {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
#ifdef TEST
    const std::string input_str{ R"(47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47
)" };

    std::istringstream input{ input_str };
#else
    std::ifstream input{ "./input/05.txt" };
#endif

    // Parse out all the rules into a vector.
    // They will be re-used on multiple places
    // to iterate over.
    std::vector<Rule> rules;
    do {
        int first, second;

        input >> first;
        input.ignore(1, '|');
        input >> second;
        input.ignore(1, '\n');

        rules.emplace_back(first, second);
    } while (input.peek() != '\n');

    input.ignore(1, '\n');

    // Read the update integers into a single shared vector.
    std::vector<int> update;

    int result = 0;

    while (input) {
        update.clear();
        while (input.peek() == '\n') {
            input.ignore();
        }

        if (input.peek() == EOF) {
            break;
        }

        // Read the comma separated update pages.
        while (input && input.peek() != '\n') {
            int read;
            input >> read;
            update.push_back(read);

            if (input.peek() == ',') {
                input.ignore(1);
            }
        }

        bool is_valid = is_update_valid(rules, update);

        // First part: sum up the middle numbers of
        // correctly ordered updates.
        if (!is_second_solution(argc, argv)) {
            if (is_valid) {
                int middle_value = update[static_cast<int>(update.size() / 2)];

                result += middle_value;
            }

            continue;
        }

        // Second part: sort the *invalid* updates
        // according to the rules.
        if (is_valid) {
            continue;
        }

        // Swap the pages until the update is correctly ordered.
        do {
            for (const Rule& rule : rules) {
                int first_idx = index_of(update, rule.first);
                if (first_idx == NOT_FOUND) {
                    continue;
                }

                int second_idx = index_of(update, rule.second);
                if (second_idx == NOT_FOUND) {
                    continue;
                }

                // is correct according to this rule
                if (first_idx < second_idx) {
                    continue;
                }

                // swap
                int temp = update[first_idx];
                update[first_idx] = update[second_idx];
                update[second_idx] = temp;
            }
        } while (!is_update_valid(rules, update));

        std::cout << std::format("Sorted invalid update: {}\n",
                                 vec_to_string(update));

        int middle_value = update[static_cast<int>(update.size() / 2)];
        result += middle_value;
    }

    std::cout << std::format("Updates middle sum = {}\n", result);
}
