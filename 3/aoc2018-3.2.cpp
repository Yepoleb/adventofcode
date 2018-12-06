#include <fstream>
#include <cassert>
#include <regex>
#include <string>
#include <vector>
#include <stddef.h>
#include <algorithm>



constexpr size_t FABRIC_SIZE = 1000;

struct Claim
{
    int id;
    int x;
    int y;
    int w;
    int h;
};

int no_overlap_rast(const std::vector<Claim>& claims)
{
    // Vector to store what overlaps
    std::vector<char> claim_overlap(claims.back().id + 1, false);
    std::vector<short> fabric(FABRIC_SIZE*FABRIC_SIZE, 0);
    for (const Claim& claim : claims) {
        for (int x_pos = claim.x; x_pos < claim.x + claim.w; x_pos++) {
            for (int y_pos = claim.y; y_pos < claim.y + claim.h; y_pos++) {
                int fabric_i = y_pos * FABRIC_SIZE + x_pos;
                short prev = fabric[fabric_i];
                if (prev) {
                    // Field is already claimed
                    fabric[fabric_i] = -1;
                    // Set previous and current claim to overlap
                    if (prev > 0) {
                        claim_overlap[prev] = true;
                    }
                    claim_overlap[claim.id] = true;
                } else {
                    // New claim
                    fabric[fabric_i] = claim.id;
                }
            }
        }
    }

    // Search for the non-overlapping claim
    for (int overlap_i = 1; overlap_i < claim_overlap.size(); overlap_i++) {
        if (!claim_overlap[overlap_i]) {
            return overlap_i;
        }
    }

    return 0;
}

struct Edge
{
    int id;
    int x;
    int y1;
    int y2;
    bool on;
};

constexpr bool overlaps(int a1, int b1, int a2, int b2)
{
    return ((a1 <= a2 && a2 < b1) || (a1 < b2 && b2 <= b1) ||
            (a2 <= a1 && a1 < b2) || (a2 < b1 && b1 <= b2));
}

int no_overlap_scan(const std::vector<Claim>& claims)
{
    // This algorithm moves in +x direction, scans the entire y axis for edges
    // and turns claims on or off, based on whether those edges are start or
    // end edges. If an edge falls in the same y-range as a turned-on claim,
    // it gets flagged as overlapping
    std::vector<Edge> edges;
    for (const Claim& claim : claims) {
        // Turn all claims into on and off edges
        Edge left_edge = {claim.id, claim.x, claim.y, claim.y + claim.h, true};
        Edge right_edge = {claim.id, claim.x + claim.w - 1, 0, 0, false};
        edges.push_back(left_edge);
        edges.push_back(right_edge);
    }
    // Sort the edges by their x coordinate and the on state as a secondary
    // condition
    std::sort(edges.begin(), edges.end(),
        [](const Edge& a, const Edge& b) {
            if (a.x != b.x) {
                return a.x < b.x;
            } else {
                return a.on > b.on;
            }
        }
    );

    // Vector for the edges that are currently active
    std::vector<Edge> active;
    // Vector to store what overlaps
    std::vector<char> claim_overlap(claims.back().id + 1, false);
    for (const Edge& edge : edges) {
        if (edge.on) {
            // Check whether the edge overlaps with any of the currently active
            // edges
            for (const Edge& act_edge : active) {
                if (overlaps(edge.y1, edge.y2, act_edge.y1, act_edge.y2)) {
                    // Set the overlap state for both parties
                    claim_overlap[edge.id] = true;
                    claim_overlap[act_edge.id] = true;
                }
            }
            active.push_back(edge);
        } else {
            // off edges are basically just delete commands
            auto del_iter = std::find_if(active.begin(), active.end(),
                [&edge](const Edge& v){return v.id == edge.id;});
            active.erase(del_iter);
        }
    }

    // Search for the non-overlapping claim
    for (int overlap_i = 1; overlap_i < claim_overlap.size(); overlap_i++) {
        if (!claim_overlap[overlap_i]) {
            return overlap_i;
        }
    }

    return 0;
}

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

    volatile int result;
    for (int i = 0; i < 1000; i++) {
        //result = no_overlap_rast(claims);
        result = no_overlap_scan(claims);
    }

    std::printf("No overlap: %d\n", result);

    return 0;
}
