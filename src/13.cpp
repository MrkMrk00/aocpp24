#include <charconv>
#include <fstream>
#include <iostream>
#include <print>
#include <sstream>
#include <string>

#ifdef TEST
static const char* test_input = "Button A: X+94, Y+34\n"
                                "Button B: X+22, Y+67\n"
                                "Prize: X=8400, Y=5400\n"
                                "\n"
                                "Button A: X+26, Y+66\n"
                                "Button B: X+67, Y+21\n"
                                "Prize: X=12748, Y=12176\n"
                                "\n"
                                "Button A: X+17, Y+86\n"
                                "Button B: X+84, Y+37\n"
                                "Prize: X=7870, Y=6450\n"
                                "\n"
                                "Button A: X+69, Y+23\n"
                                "Button B: X+27, Y+71\n"
                                "Prize: X=18641, Y=10279\n";
#endif

static constexpr int max_presses = 100;

struct IVec2
{
    int x, y;
};

IVec2 read_coords(const std::string& line)
{
    IVec2 v{};

    std::from_chars(
      line.data() + line.find("X") + 2, line.data() + line.find(","), v.x);

    std::from_chars(
      line.data() + line.find("Y") + 2, line.data() + line.size(), v.y);

    return v;
}

static constexpr int a_cost = 3;
static constexpr int b_cost = 1;

int main()
{
#ifdef TEST
    std::istringstream input{ test_input };
#else
    std::ifstream input{ "input/13.txt" };
#endif
    std::string line;

    long total_cost = 0;

    while (input) {
        std::getline(input, line);
        if (line.empty()) {
            continue;
        }

        IVec2 A = read_coords(line);

        std::getline(input, line);
        IVec2 B = read_coords(line);

        std::getline(input, line);
        IVec2 prize = read_coords(line);

        long min_cost = -1;
        for (int a = 0; a <= max_presses; a++) {
            for (int b = max_presses; b >= 0; b--) {
                long x, y;

                x = a * A.x + b * B.x;
                y = a * A.y + b * B.y;

                if (x != prize.x || y != prize.y) {
                    continue;
                }

                long cost = a * a_cost + b * b_cost;

                if (min_cost == -1 || cost < min_cost) {
                    min_cost = cost;
                }
            }
        }

        std::println("Min cost = {}", min_cost);

        if (min_cost != -1) {
            total_cost += min_cost;
        }
    }

    std::cout << '\n';
    std::println("Total cost = {}", total_cost);
}
