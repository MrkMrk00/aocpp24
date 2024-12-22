#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int NOT_FOUND = -1;

int index_of(const std::vector<int> haystack, int needle)
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

    Rule(int first_, int second_)
      : first{ first_ }
      , second{ second_ }
    {
    }
};

int main()
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

    int valid_updates_middle_sum = 0;

    // read the update integers
    std::vector<int> update;
    while (input) {
        update.clear();
        while (input.peek() == '\n') {
            input.ignore();
        }

        if (input.peek() == EOF) {
            break;
        }

        while (input && input.peek() != '\n') {
            int read;
            input >> read;
            update.push_back(read);

            if (input.peek() == ',') {
                input.ignore(1);
            }
        }

        // check if update is valid
        bool is_valid = true;
        for (const auto& rule : rules) {
            int first_idx = index_of(update, rule.first);
            if (first_idx == NOT_FOUND) {
                continue;
            }

            int second_idx = index_of(update, rule.second);
            if (second_idx == NOT_FOUND) {
                continue;
            }

            if (first_idx > second_idx) {
                is_valid = false;

                break;
            }
        }

        if (is_valid) {
            int middle_value = update[static_cast<int>(update.size() / 2)];

            valid_updates_middle_sum += middle_value;
        }
    }

    std::cout << std::format("Valid updates middle value sum = {}\n",
                             valid_updates_middle_sum);
}
