#include <unordered_set>
#include <cstdio>
#include <vector>
#include <fstream>
#include <cassert>



int find_freq(const std::vector<int>& changes)
{
    int num_changes = changes.size();

    std::unordered_set<int> base_freqs;
    base_freqs.reserve(num_changes);

    // Fill the set with the frequencies of the first iteration, because those
    // are the only ones we need to check
    int freq = 0;
    for (int i = 0; i < num_changes; i++) {
        freq += changes[i];

        if (base_freqs.find(freq) != base_freqs.end()) {
            return freq;
        }

        base_freqs.insert(freq);
    }

    // Apply changes endlessly until match is found
    for (int i = 0; ; i++) {
        if (i == num_changes) {
            i = 0;
        }
        freq += changes[i];

        if (base_freqs.find(freq) != base_freqs.end()) {
            return freq;
        }
    }

    return 0;
}

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());

    std::vector<int> changes;
    while (true) {
        int change = 0;
        file >> change;
        if (change == 0) break;
        changes.push_back(change);
    }

    volatile int result;
    for (int i = 0; i < 100; i++) {
        result = find_freq(changes);
    }
    std::printf("Frequency: %d\n", result);

    return 0;
}

