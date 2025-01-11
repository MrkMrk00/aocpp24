#include "utils.h"

#include <cstdint>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>

constexpr int ASCII_FIRST_NUMBER = 48;
constexpr int EMPTY_BYTE = -1;

static std::string fs_to_string(const std::vector<int>& fs)
{
    std::ostringstream oss;

    for (int i : fs) {
        if (i == EMPTY_BYTE) {
            oss << '.';

            continue;
        }

        oss << i;
    }

    return oss.str();
}

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

static void defragment_per_byte(std::vector<int>& fs)
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

static void defragment_per_file(std::vector<int>& fs)
{
    for (int j = fs.size() - 1; j > 0; j--) {
        if (fs[j] == EMPTY_BYTE) {
            continue;
        }

        // found a non-empty byte
        //  -> lookup file_size
        int file_start = j;
        while (file_start > 0 && fs[file_start - 1] == fs[j])
            file_start--;

        int file_size = j - file_start + 1;

        for (int i = 0; i < file_start; i++) {
            if (fs[i] != EMPTY_BYTE) {
                continue;
            }

            int empty_block_end = i;
            while (empty_block_end < file_start &&
                   fs[empty_block_end + 1] == EMPTY_BYTE)
                empty_block_end++;

            int empty_block_size = empty_block_end - i + 1;

            // File fits into the current empty block
            //  -> do the copy.
            if (empty_block_size >= file_size) {
                for (int offset = 0; offset < file_size; offset++) {
                    int free_index =
                      empty_block_end - empty_block_size + offset + 1;

                    fs[free_index] = fs[file_start + offset];
                    fs[file_start + offset] = EMPTY_BYTE;
                }

                break;
            }

            // will be incremented by one
            // by the for-loop
            i = empty_block_end;
        }

        j = file_start;
    }
}

static uint64_t checksum(const std::vector<int>& fs)
{
    uint64_t checksum = 0;

    for (size_t i = 0; i < fs.size(); i++) {
        if (fs[i] == EMPTY_BYTE) {
            continue;
        }

        checksum += fs[i] * i;
    }

    return checksum;
}

int main(int argc, char** argv)
{
#ifdef TEST
    std::istringstream input{ "2333133121414131402" };
#else
    std::ifstream input{ "./input/09.txt" };
#endif
    (void)fs_to_string;

    std::vector<int> fs = parse_fs(input);

    if (!is_second_solution(argc, argv)) {
        defragment_per_byte(fs);
#ifdef TEST
        std::cout << "0099811188827773336446555566..............\n"
                  << fs_to_string(fs) << '\n';
#endif
        std::cout << "Checksum = " << checksum(fs) << std::endl;

        return 0;
    }

    defragment_per_file(fs);
#ifdef TEST
    std::cout << "00992111777.44.333....5555.6666.....8888..\n"
              << fs_to_string(fs) << '\n';
#endif
    std::cout << "Checksum = " << checksum(fs) << std::endl;
}
