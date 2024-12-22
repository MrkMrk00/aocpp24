#pragma once

#include <algorithm>
#include <cctype>
#include <cstring>
#include <format>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

inline bool is_second_solution(int argc, char* argv[])
{
    return argc >= 2 && std::strcmp(argv[1], "-b") == 0;
}

template<typename T>
std::string vec_to_string(const std::vector<T>& vec)
{
    std::ostringstream printed_vec;
    printed_vec << "[ ";

    for (int i = 0; i < vec.size(); i++) {
        printed_vec << vec[i];

        if (i != vec.size() - 1) {
            printed_vec << ", ";
        }
    }

    printed_vec << " ]";

    return printed_vec.str();
}

template<typename MapType>
std::string map_to_string(const MapType& map)
{
    std::ostringstream printed_map;
    printed_map << "{ ";

    int cur = 0;
    for (auto [key, value] : map) {
        if (cur >= 5 && cur % 5 == 0) {
            printed_map << "\t";
        }
        printed_map << std::format("{}: {}", key, value);

        if (cur++ != map.size() - 1) {
            printed_map << ", ";
        }

        if (cur % 5 == 0) {
            printed_map << "\n";
        }
    }

    printed_map << " }";

    return printed_map.str();
}
