#include <cassert>
#include <limits.h>
#include <cstdio>
#include <vector>
#include <fstream>



int find_freq(const std::vector<int>& changes)
{
    const int num_changes = changes.size();
    const int num_comb = num_changes * num_changes;

    // Fill the vector with frequencies of the first iteration
    std::vector<int> base_freqs;
    base_freqs.reserve(num_changes);
    int cur_freq = 0;
    for (int c : changes) {
        cur_freq += c;
        base_freqs.push_back(cur_freq);
    }

    // Define the amount by which each iteration differs from the last one
    const int drift = cur_freq;
    const float drift_f = static_cast<float>(drift);
    assert(drift != 0);
    bool drift_negative = drift < 0;

    // Calculate the difference between all base frequencies
    std::vector<float> differences;
    differences.reserve(num_comb);
    for (int freq_a : base_freqs) {
        for (int freq_b : base_freqs) {
            differences.push_back(static_cast<float>(freq_b - freq_a));
        }
    }

    int min_iter = INT_MAX;
    int min_i = 0;
    for (int i = 0; i < num_comb; i++) {
        float diff = differences[i];

        bool diff_negative = diff < 0;
        if (diff_negative != drift_negative) {
            // We only have positive iterations, so differences with different
            // signs are impossible to reach
            continue;
        }
        if (diff == 0.f) {
            // We are comparing the number to itself
            continue;
        }

        // Calculate the number of iterations needed to make up for the
        // difference between two base frequencies
        float iterations = diff / drift_f;
        assert(iterations >= 0);
        int iterations_int = static_cast<int>(iterations);

        // Check if iteration is an integer and smaller than min_iter
        if (iterations == iterations_int and iterations_int < min_iter) {
            min_iter = iterations_int;
            min_i = i;
        }
    }

    // Get the base frequency by dividing its position in the combinations
    // vector by the number of combinations per frequency
    int freq_index = min_i / num_changes;
    int freq_base = base_freqs[freq_index];
    // Calculate the final frequency by adding the drift amount for each
    // iteration
    int freq_final = freq_base + min_iter * drift;

    return freq_final;
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

