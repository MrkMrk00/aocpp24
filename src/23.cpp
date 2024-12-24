#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#ifdef TEST
const char* test_input = "kh-tc\n"
                         "qp-kh\n"
                         "de-cg\n"
                         "ka-co\n"
                         "yn-aq\n"
                         "qp-ub\n"
                         "cg-tb\n"
                         "vc-aq\n"
                         "tb-ka\n"
                         "wh-tc\n"
                         "yn-cg\n"
                         "kh-ub\n"
                         "ta-co\n"
                         "de-co\n"
                         "tc-td\n"
                         "tb-wq\n"
                         "wh-td\n"
                         "ta-ka\n"
                         "td-qp\n"
                         "aq-cg\n"
                         "wq-ub\n"
                         "ub-vc\n"
                         "de-ta\n"
                         "wq-aq\n"
                         "wq-vc\n"
                         "wh-yn\n"
                         "ka-de\n"
                         "kh-ta\n"
                         "co-tc\n"
                         "wh-qp\n"
                         "tb-vc\n"
                         "td-yn\n";
#endif

typedef std::unordered_map<std::string, std::unordered_set<std::string>>
  ConnectionsMap;

int main()
{
#ifdef TEST
    std::istringstream input_stream{ test_input };
#else
    std::ifstream input_stream{ "./input/23.txt" };
#endif

    ConnectionsMap connections_map;

    std::string line;
    while (std::getline(input_stream, line)) {
        std::string from{ line.data(), 2 };
        std::string to{ line.data() + 3, 2 };

        connections_map[from].insert(to);
        connections_map[to].insert(from);
    }

    std::unordered_set<std::string> triple_connections;

    // used for sorting the connection keys
    std::array<std::string, 3> keys;

    for (auto a = connections_map.begin(); a != connections_map.end(); a++) {
        for (auto b = std::next(a); b != connections_map.end(); b++) {
            // A, B connected
            if (!b->second.contains(a->first)) {
                continue;
            }

            // A, C connected
            for (const auto& maybe_common_key : a->second) {
                // B, C connected
                if (!b->second.contains(maybe_common_key)) {
                    continue;
                }

                // ...with a name that starts with t?
                if (a->first[0] != 't' && b->first[0] != 't' &&
                    maybe_common_key[0] != 't') {
                    continue;
                }

                // both sets contain the key...
                keys[0] = a->first;
                keys[1] = b->first;
                keys[2] = maybe_common_key;

                // sort -> set will throw out duplicates
                std::sort(keys.begin(), keys.end());

                std::string combined_key =
                  std::format("{}-{}-{}", keys[0], keys[1], keys[2]);

                triple_connections.insert(std::move(combined_key));
            }
        }
    }

    std::cout << '\n'
              << "Connections count: " << triple_connections.size()
              << std::endl;
}
