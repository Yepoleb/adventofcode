#include <fstream>
#include <cassert>
#include <regex>
#include <string>
#include <vector>
#include <stddef.h>
#include <limits.h>
#include <cstdio>



constexpr size_t FABRIC_SIZE = 1000;

struct Claim
{
    int id;
    int x;
    int y;
    int w;
    int h;
};

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());
    std::regex input_re("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");

    std::vector<Claim> claims;
    while (true) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            break;
        }

        std::smatch line_match;
        std::regex_match(line, line_match, input_re);
        Claim claim;
        claim.id = std::stoi(line_match[1]);
        claim.x = std::stoi(line_match[2]);
        claim.y = std::stoi(line_match[3]);
        claim.w = std::stoi(line_match[4]);
        claim.h = std::stoi(line_match[5]);
        claims.push_back(claim);
    }

    std::vector<bool> claim_overlap(claims.back().id + 1, false);
    std::vector<short> fabric(FABRIC_SIZE*FABRIC_SIZE, 0);
    for (const Claim& claim : claims) {
        for (int x_pos = claim.x; x_pos < claim.x + claim.w; x_pos++) {
            for (int y_pos = claim.y; y_pos < claim.y + claim.h; y_pos++) {
                int fabric_i = y_pos * FABRIC_SIZE + x_pos;
                short prev = fabric[fabric_i];
                if (prev) {
                    fabric[fabric_i] = -1;
                    if (prev > 0) {
                        claim_overlap[prev] = true;
                    }
                    claim_overlap[claim.id] = true;
                } else {
                    fabric[fabric_i] = claim.id;
                }
            }
        }
    }

    int double_claimed = 0;
    for (short square : fabric) {
        if (square == -1) {
            double_claimed++;
        }
    }

    std::printf("Num double claimed: %d\n", double_claimed);

    for (int overlap_i = 1; overlap_i < claim_overlap.size(); overlap_i++) {
        if (!claim_overlap[overlap_i]) {
            std::printf("No overlap: %d\n", overlap_i);
        }
    }

    return 0;
}
