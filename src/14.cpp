#include "utils.h"
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <sstream>
#include <unordered_map>
#include <vector>

struct Vec2
{
    int x, y;
};

struct Robot
{
    Vec2 pos;
    Vec2 vel;
};

static void print_bots(const std::vector<Robot>& robots,
                       int board_width,
                       int board_height)
{
    std::unordered_map<int, int> robots_at_pos;
    for (const auto& robot : robots) {
        robots_at_pos[robot.pos.y * board_width + robot.pos.x]++;
    }

    for (int y = 0; y < board_height; y++) {
        for (int x = 0; x < board_width; x++) {
            int index = y * board_width + x;

            if (robots_at_pos.contains(index)) {
                std::cout << robots_at_pos[index];
            } else {
                std::cout << '.';
            }
        }

        std::cout << '\n';
    }
}

int main(int argc, char** argv)
{
    (void)print_bots;

#ifdef TEST
    std::istringstream input{
        "p=0,4 v=3,-3\n"
        "p=6,3 v=-1,-3\n"
        "p=10,3 v=-1,2\n"
        "p=2,0 v=2,-1\n"
        "p=0,0 v=1,3\n"
        "p=3,0 v=-2,-2\n"
        "p=7,6 v=-1,-3\n"
        "p=3,0 v=-1,-2\n"
        "p=9,3 v=2,3\n"
        "p=7,3 v=-1,2\n"
        "p=2,4 v=2,-3\n"
        "p=9,5 v=-3,-3\n",
    };
#else
    std::ifstream input{ "./input/14.txt" };
#endif

    std::vector<Robot> robots;

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            break;
        }

        std::istringstream iss{ line };
        iss.ignore(2);

        int pos_x, pos_y;
        iss >> pos_x;
        iss.ignore(1);
        iss >> pos_y;

        iss.ignore(3);

        int vel_x, vel_y;
        iss >> vel_x;
        iss.ignore(1);
        iss >> vel_y;

        robots.emplace_back(Vec2{ .x = pos_x, .y = pos_y },
                            Vec2{ .x = vel_x, .y = vel_y });
    }

    bool is_second = is_second_solution(argc, argv);

    int seconds = 100;
    if (is_second) {
        seconds = INT_MAX;
    }

#ifdef TEST
    constexpr int board_width = 11;
    constexpr int board_height = 7;
#else
    constexpr int board_width = 101;
    constexpr int board_height = 103;
#endif

    for (int second = 1; second <= seconds; second++) {
        for (auto& robot : robots) {
            robot.pos.x += robot.vel.x;
            if (robot.pos.x < 0) {
                robot.pos.x += board_width;
            }

            robot.pos.x %= board_width;

            robot.pos.y += robot.vel.y;
            if (robot.pos.y < 0) {
                robot.pos.y += board_height;
            }

            robot.pos.y %= board_height;
        }

        if (!is_second) {
            continue;
        }

        size_t neighbor_count = 0;
        for (auto a = robots.begin(); a < robots.end(); a++) {
            bool has_neighbor = false;

            for (auto b = std::next(a); b < robots.end(); b++) {
                int dx = a->pos.x - b->pos.x;
                int dy = a->pos.y - b->pos.y;

                if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
                    has_neighbor = true;

                    break;
                }
            }

            if (has_neighbor) {
                neighbor_count++;
            }
        }

        if (neighbor_count > 0.5 * robots.size()) {
            print_bots(robots, board_width, board_height);

            std::cout << '\n'
                      << "Found tree!\n"
                      << std::format("Took seconds = {}\n", second);
            return 0;
        }
    }

    const int quadrant_width = static_cast<int>(board_width / 2);
    const int quadrant_height = static_cast<int>(board_height / 2);

    int safety_factor = 1;
    for (int x = 0; x <= 1; x++) {
        int x_min = (quadrant_width + 1) * x;
        int x_max = x_min + quadrant_width - 1;

        for (int y = 0; y <= 1; y++) {
            int y_min = (quadrant_height + 1) * y;
            int y_max = y_min + quadrant_height - 1;

            int robots_in_quadrant = 0;
            for (const auto& robot : robots) {
                if (robot.pos.x >= x_min && robot.pos.x <= x_max &&
                    robot.pos.y >= y_min && robot.pos.y <= y_max) {
                    robots_in_quadrant++;
                }
            }

            safety_factor *= robots_in_quadrant;
        }
    }

#ifdef TEST
    print_bots(robots, board_width, board_height);
#endif

    std::println("Safety factor = {}", safety_factor);
}
