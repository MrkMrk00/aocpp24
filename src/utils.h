#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

template<typename T>
std::string vec_to_string(const std::vector<T>& vec)
{
    std::string result = "[";

    for (int i = 0; i < vec.size(); i++) {
        auto item = vec[i];

        result.append(std::to_string(item));

        if (i != vec.size() - 1) {
            result.append(", ");
        }
    }

    result.append("]");

    return result;
}

std::string map_to_string(const std::unordered_map<int, int>& map) {
    std::string result = "{ ";

    int cur = 0;
    for (auto [key, value] : map) {
        result.append(std::to_string(key));
        result.append(": ");
        result.append(std::to_string(value));

        if (cur++ != map.size() - 1) {
            result.append(", ");
        }
    }

    result.append(" }");

    return result;
}

