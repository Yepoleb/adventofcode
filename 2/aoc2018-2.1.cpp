#include <cstdio>
#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <array>
#include <stx/textserializer.hpp>

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());

    int have_2 = 0;
    int have_3 = 0;

    while (true) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            break;
        }
        // change to char, 32 later
        std::array<int, 26> char_counts = {};
        for (char c : line) {
            char_counts[c - 'a']++;
        }

        bool has_2 = false;
        bool has_3 = false;
        for (int i = 0; i < 26; i++) {
            if (char_counts[i] == 2) {
                has_2 = true;
            } else if (char_counts[i] == 3) {
                has_3 = true;
            }
        }

        have_2 += has_2;
        have_3 += has_3;
    }

    std::printf("Checksum: %d\n", have_2 * have_3);

    return 0;
}
