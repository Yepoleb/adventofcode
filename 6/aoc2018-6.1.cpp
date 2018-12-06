#include <fstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <algorithm>
#include <vector>
#include <cassert>



struct Vector2i
{
    Vector2i operator-(Vector2i rhs) const
    {
        return Vector2i{x - rhs.x, y - rhs.y};
    }

    bool operator==(Vector2i rhs) const
    {
        return (x == rhs.x) && (y == rhs.y);
    }

    Vector2i abs() const
    {
        return Vector2i{std::abs(x), std::abs(y)};
    }

    int x;
    int y;
};

inline int distance(Vector2i p1, Vector2i p2)
{
    Vector2i dist_xy = (p1 - p2).abs();
    return dist_xy.x + dist_xy.y;
}

// Determines the nearest point to a position
// Returns a point if there is only one candidate for the position
// Returns {0, 0} if two or more points have the same distance
// Returns {-1, -1} if the distances from all points has only increased since
// the last check, to signal that the area in this direction is going to be
// infinite
Vector2i nearest_point(
    Vector2i origin, const std::vector<Vector2i>& points,
    std::vector<int>& last_distances)
{
    Vector2i nearest = {0, 0};
    int min_dist = INT_MAX;
    // Current min_dist has been matched by at least one other point
    bool matched = false;
    // Distance to all points has only been increased since last check
    bool only_increased = true;

    for (size_t target_i = 0; target_i < points.size(); target_i++) {
        Vector2i target = points[target_i];
        int dist = distance(origin, target);
        if (dist < min_dist) {
            nearest = target;
            min_dist = dist;
            matched = false;
        } else if (dist == min_dist) {
            matched = true;
        }
        if (dist < last_distances[target_i]) {
            only_increased = false;
        }
        last_distances[target_i] = dist;
    }

    if (only_increased) {
        return {-1, -1};
    }
    if (!matched) {
        return nearest;
    } else {
        return {0, 0};
    }
}

// Counts the area in x direction
int walk_x(
    Vector2i origin, Vector2i pos, int direction,
    const std::vector<Vector2i>& points, std::vector<int> last_distances)
{
    int area = 0;
    while (true) {
        Vector2i nearest = nearest_point(pos, points, last_distances);
        if (nearest.x == -1) {
            // Area is infinite
            return -1;
        }
        if (nearest == origin) {
            area++;
        } else {
            // Point no longer belongs to origin
            break;
        }

        pos.x += direction;
    }
    return area;
}

// Counts the area in y direction and calls walk_x for each y position
int walk_y(
    Vector2i origin, Vector2i pos, int direction,
    const std::vector<Vector2i>& points)
{
    int area = 0;
    std::vector<int> last_distances(points.size(), INT_MAX);
    while (true) {
        Vector2i nearest = nearest_point(pos, points, last_distances);
        if (nearest.x == -1) {
            // Area is infinite
            return -1;
        }
        if (nearest == origin) {
            area++;
        } else {
            // Point no longer belongs to origin
            break;
        }
        // Count the area to the right (x+)
        int area_r = walk_x(
            origin, {pos.x + 1, pos.y}, +1, points, last_distances);
        // Check if area is infinite
        if (area_r == -1) return -1;
        // Count the area to the left (x-)
        int area_l = walk_x(
            origin, {pos.x - 1, pos.y}, -1, points, last_distances);
        if (area_l == -1) return -1;
        area += area_r + area_l;

        pos.y += direction;
    }
    return area;
}

int main()
{
    std::ifstream file("input.txt");
    assert(file.good());

    std::vector<Vector2i> points;
    while (true) {
        Vector2i p = {0, 0};
        file >> p.x;
        if (p.x == 0) {
            break;
        }
        file.ignore(2);
        file >> p.y;
        points.push_back(p);
    }

    int max_area = 0;
    for (Vector2i origin : points) {
        // Count the area above (y+) and at the same level as the origin
        int area_u = walk_y(origin, {origin.x, origin.y + 0}, +1, points);
        // Check if area is infinite
        if (area_u == -1) continue;
        // Count the area below
        int area_d = walk_y(origin, {origin.x, origin.y - 1}, -1, points);
        if (area_d == -1) continue;

        int total_area = area_u + area_d;
        max_area = std::max(max_area, total_area);
    }

    std::printf("Max area: %d\n", max_area);

    return 0;
}
