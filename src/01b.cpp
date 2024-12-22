#include "./utils.h"
#include <format>
#include <fstream>
#include <iostream>
#include <unordered_map>

int main()
{
#ifdef TEST
    std::stringstream input{
        R"(
        3   4
        4   3
        2   5
        1   3
        3   9
        3   3
        )",
    };
#else
    std::ifstream input{ "./input/01.txt" };
#endif
    constexpr int line_count = 1000;

    std::vector<int> A;
    std::vector<int> B;

    A.reserve(line_count);
    B.reserve(line_count);

    int a, b;
    while (input >> a >> b) {
        A.push_back(a);
        B.push_back(b);
    }

    std::unordered_map<int, int> counts;
    for (auto b : B) {
        counts[b]++;
    }

    std::cout << map_to_string(counts) << std::endl;

    int sum = 0;
    for (auto a : A) {
        int count = counts[a];

        sum += a * count;
    }

    std::cout << std::format("Result = {}", sum) << std::endl;
}
