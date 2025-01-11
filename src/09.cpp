#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

constexpr int ASCII_FIRST_NUMBER = 48;
constexpr int EMPTY_BYTE = -1;

static std::vector<int> parse_fs(std::istream& stream)
{
    std::vector<int> bytes;

    char ch;
    while (stream >> ch) {
        int stream_pos = static_cast<int>(stream.tellg()) - 1;
        int block_size = ch - ASCII_FIRST_NUMBER;
        int block_id = stream_pos / 2;

        for (int i = 0; i < block_size; i++) {
            bytes.push_back(stream_pos % 2 == 0 ? block_id : EMPTY_BYTE);
        }
    }

    return bytes;
}

static void defragment(std::vector<int>& fs)
{
    int i = 0;

    for (int j = fs.size() - 1; j > i; j--) {
        if (fs[j] == EMPTY_BYTE) {
            continue;
        }

        for (; i < j; i++) {
            if (fs[i] != EMPTY_BYTE) {
                continue;
            }

            fs[i] = fs[j];
            fs[j] = EMPTY_BYTE;

            break;
        }
    }
}

static uint64_t checksum(const std::vector<int>& fs)
{
    uint64_t checksum = 0;

    for (size_t i = 0; i < fs.size(); i++) {
        if (fs[i] == EMPTY_BYTE) {
            break;
        }

        checksum += fs[i] * i;
    }

    return checksum;
}

int main()
{
#ifdef TEST
    std::istringstream input{ "2333133121414131402" };
#else
    std::ifstream input{ "./input/09.txt" };
#endif

    std::vector<int> fs = parse_fs(input);
    defragment(fs);

    std::cout << "Checksum = " << checksum(fs) << std::endl;
}
