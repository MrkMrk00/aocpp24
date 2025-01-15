#include "utils.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

static inline int count_digits(uint64_t number)
{
    int digits = 0;

    while (number != 0) {
        number /= 10;
        digits++;
    }

    return digits;
}

static uint64_t count_expanded_stones(uint64_t stone, int n)
{
    using CountCache = std::array<std::unordered_map<uint64_t, uint64_t>, 75>;
    static CountCache count_cache;

    if (n == 0) {
        return 1;
    }

    n--;

    if (stone == 0) {
        if (count_cache[n].contains(stone)) {
            return count_cache[n][stone];
        }

        uint64_t value = count_expanded_stones(1, n);
        count_cache[n][stone] = value;

        return value;
    }

    int digit_count = count_digits(stone);
    if (digit_count % 2 == 0) {
        if (count_cache[n].contains(stone)) {
            return count_cache[n][stone];
        }

        uint64_t multiplier = std::pow(10, digit_count / 2);

        uint64_t left = stone / multiplier;
        uint64_t right = stone - (left * multiplier);

        uint64_t value =
          count_expanded_stones(left, n) + count_expanded_stones(right, n);
        count_cache[n][stone] = value;

        return value;
    }

    if (count_cache[n].contains(stone)) {
        return count_cache[n][stone];
    }

    uint64_t value = count_expanded_stones(stone * 2024, n);
    count_cache[n][stone] = value;

    return value;
}

int main(int argc, char** argv)
{
#ifdef TEST
    std::istringstream input{ "125 17\n" };
#else
    std::ifstream input{ "./input/11.txt" };
#endif

    std::vector<uint64_t> stones;

    long input_stone;
    while (input >> input_stone) {
        stones.push_back(input_stone);
    }

    int n_blinks = 25;
    if (is_second_solution(argc, argv)) {
        n_blinks = 75;
    }

    uint64_t stones_sum = 0;
    for (size_t i = 0; i < stones.size(); i++) {
        stones_sum += count_expanded_stones(stones[i], n_blinks);
    }

    std::cout << "Number of stones = " << stones_sum << std::endl;
}
