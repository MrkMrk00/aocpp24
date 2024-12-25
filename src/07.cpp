#include "./utils.h"

#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

enum Operation : uint8_t
{
    ADD = 0,
    MULTIPLY,
    CONCAT,
};

// to be initialized in main
uint8_t OPERATIONS_COUNT;

#ifdef TEST
static char operation_to_char(Operation operation)
{
    switch (operation) {
        case ADD:
            return '+';
        case MULTIPLY:
            return '*';
        case CONCAT:
            return '|';
    }
}
#endif

static int64_t int_concat(int64_t a, int64_t b)
{
    int64_t pow = 1;
    while (pow <= b) {
        pow *= 10;
    }

    return a * pow + b;
}

typedef std::vector<Operation> OpCombination;

struct Equation
{
    int64_t result;
    std::vector<int64_t> operands;

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << result << ':';

        for (int64_t operand : operands) {
            oss << ' ' << operand;
        }

        return oss.str();
    }

    bool try_solve(const OpCombination& operators) const
    {
        assert(operators.size() == operands.size() - 1);

        int64_t sum = operands[0];

        for (size_t pos_in_equation = 0; pos_in_equation < operators.size();
             pos_in_equation++) {
            // early escape, if the sum is already too high
            if (sum > result) {
                break;
            }

            Operation operation = operators[pos_in_equation];
            int64_t operand = operands[pos_in_equation + 1];

            switch (operation) {
                case ADD:
                    sum += operand;
                    break;
                case MULTIPLY:
                    sum *= operand;
                    break;
                case CONCAT:
                    sum = int_concat(sum, operand);
                    break;
            }
        }

        return sum == result;
    }
};

static std::vector<Equation> parse_equations(std::istream& stream)
{
    std::vector<Equation> equations;
    std::vector<int64_t> equation_operands;

    uint64_t result;
    while (stream >> result) {
        stream.ignore(1, ':');
        equation_operands.clear();

        int64_t operand;
        while (stream.peek() != '\n' && stream >> operand) {
            equation_operands.push_back(operand);
        }

        assert(equation_operands.size() > 1);
        equations.emplace_back(result, std::move(equation_operands));

        stream.ignore(1, '\n');
    }

    return equations;
}

static OpCombination empty_combination(size_t n_positions)
{
    OpCombination combination;
    combination.reserve(n_positions);

    for (size_t i = 0; i < n_positions; i++) {
        combination.push_back(ADD);
    }

    return combination;
}

int main(int argc, char* argv[])
{
#ifdef TEST
    std::istringstream input{ "190: 10 19\n"
                              "3267: 81 40 27\n"
                              "83: 17 5\n"
                              "156: 15 6\n"
                              "7290: 6 8 6 15\n"
                              "161011: 16 10 13\n"
                              "192: 17 8 14\n"
                              "21037: 9 7 18 13\n"
                              "292: 11 6 16 20\n" };
#else
    std::ifstream input{ "./input/07.txt" };
#endif

    Operation last_supported_operation = MULTIPLY;
    if (is_second_solution(argc, argv)) {
        last_supported_operation = CONCAT;
    }
    OPERATIONS_COUNT = last_supported_operation + 1;

    int64_t valid_equations_sum = 0;

    std::vector<OpCombination> combinations;

    for (const Equation& equation : parse_equations(input)) {
        combinations.clear();

        const size_t n_positions = equation.operands.size() - 1;

        // Initialize the initial state with
        // the amount of combinations equal to
        // the number of operators.
        for (Operation op = static_cast<Operation>(0); op < OPERATIONS_COUNT;
             op = static_cast<Operation>(op + 1)) {
            OpCombination combination = empty_combination(n_positions);

            combination[0] = op;

            combinations.push_back(std::move(combination));
        }

        combinations.reserve(std::pow(OPERATIONS_COUNT, n_positions));

        // other positions
        for (size_t position = 1; position < n_positions; position++) {
            // here I need to actually copy the combinations
            for (const auto& combination : combinations) {
                for (Operation op = static_cast<Operation>(1);
                     op < OPERATIONS_COUNT;
                     op = static_cast<Operation>(op + 1)) {

                    OpCombination new_combination = combination;
                    new_combination[position] = op;
                    combinations.push_back(std::move(new_combination));
                }
            }
        }

        for (const OpCombination& combination : combinations) {
            if (!equation.try_solve(combination)) {
                continue;
            }

            // The current combination of operators solve
            // the current equation.

            valid_equations_sum += equation.result;

#ifdef TEST
            std::cout << std::format("Matches ({})\n", equation.to_string());

            std::cout << equation.operands.at(0);
            for (size_t i = 0; i < combination.size(); i++) {
                std::cout << operation_to_char(combination[i])
                          << equation.operands.at(i + 1);
            }

            std::cout << " = " << equation.result << '\n';
#endif

            break;
        }
    }

    std::cout << "Sum of valid equations: " << valid_equations_sum << '\n';
}
