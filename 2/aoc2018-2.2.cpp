#include <fstream>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <array>

constexpr int LABEL_LEN = 26;

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());

    std::vector<std::string> labels;

    while (true) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            break;
        }
        labels.push_back(line);
    }

    std::string found_a;
    std::string found_b;

    for (int index_a = 0; index_a < labels.size(); index_a++) {
        const std::string& label_a = labels[index_a];
        for (int index_b = 0; index_b < labels.size(); index_b++) {
            const std::string& label_b = labels[index_b];

            // Check if we are comparing the string to itself
            if (index_a == index_b) {
                continue;
            }

            // Stores if we have seen a different letter already
            bool diff_letter = false;
            for (int index_l = 0; index_l < LABEL_LEN; index_l++) {
                if (label_a[index_l] != label_b[index_l]) {
                    // Letters are different
                    if (!diff_letter) {
                        // We have not seen a different letter before, so this
                        // is our first one
                        diff_letter = true;
                    } else {
                        // This is our second difference already, skip the
                        // combination entirely
                        goto endcomparison;
                    }
                }
            }

            if (diff_letter) {
                // We have at least 1 different letter and because of the goto
                // in the loop it must have been the only one
                found_a = label_a;
                found_b = label_b;
                // Stop the search, we have found our match
                goto endsearch;
            }
endcomparison:
            continue;
        }
    }

endsearch:
    for (int index_l = 0; index_l < LABEL_LEN; index_l++) {
        if (found_a[index_l] == found_b[index_l]) {
            // If the character is the same in both labels, output it
            std::cout << found_a[index_l];
        }
    }
    std::cout << '\n';

    return 0;
}
