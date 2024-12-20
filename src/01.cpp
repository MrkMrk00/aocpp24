#include "./utils.h"
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

    std::vector<int> A, B;

    A.reserve(1000);
    B.reserve(1000);

    int a, b;
    while (input >> a >> b) {
        A.push_back(a);
        B.push_back(b);
    }

    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());

    int sum = 0;
    for (int i = 0; i < A.size(); i++) {
        int a = A[i];
        int b = B[i];

        if (a > b) {
            sum += a - b;
        } else {
            sum += b - a;
        }
    }

#ifdef TEST
    std::cout << std::format("A = {}\n", vec_to_string(A));
    std::cout << std::format("B = {}\n", vec_to_string(B));
#endif
    std::cout << std::format("Result = {}\n", sum);

    std::cout << std::endl;

    return 0;
}
