#include "./utils.h"
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

static bool vec_is_safe(const std::vector<int>& vec)
{
    if (vec.size() <= 1) {
        return true;
    }

    // in increasing order
    if (vec[0] < vec[vec.size() - 1]) {
        for (int i = 1; i < vec.size(); i++) {
            int prev = vec[i - 1];
            int current = vec[i];

            int diff = current - prev;

            if (diff > 3 || diff < 1) {
                return false;
            }
        }
        // in decreasing order
    } else if (vec[0] > vec[vec.size() - 1]) {
        for (int i = 1; i < vec.size(); i++) {
            int prev = vec[i - 1];
            int current = vec[i];

            int diff = prev - current;

            if (diff > 3 || diff < 1) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}

int main()
{
#ifdef TEST
    std::istringstream input{
        R"(
        7 6 4 2 1
        1 2 7 8 9
        9 7 6 2 1
        1 3 2 4 5
        8 6 4 4 1
        1 3 6 7 9
        )"
    };
#else
    std::ifstream input{ "./input/02.txt" };
#endif

    int safe_count = 0;

    std::string line;
    while (std::getline(input, line)) {
        std::vector<int> report;

        std::istringstream line_stream{ line };
        int number;

        while (line_stream >> number) {
            report.push_back(number);
        }

        if (report.size() == 0) {
            continue;
        }

        bool is_safe = vec_is_safe(report);
        std::cout << std::format(
          "report: {} - safe?: {}\n", vec_to_string(report), is_safe);

        if (is_safe) {
            safe_count++;
        }
    }

    std::cout << std::format("safe_count: {}", safe_count) << std::endl;
}
