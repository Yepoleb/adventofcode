#include <fstream>
#include <string>
#include <cstdlib>
#include <numeric>
#include <vector>
#include <cassert>



constexpr int MAX_DISTANCE = 10000;

struct Vector2i
{
    Vector2i operator+(Vector2i rhs) const
    {
        return Vector2i{x + rhs.x, y + rhs.y};
    }

    Vector2i operator-(Vector2i rhs) const
    {
        return Vector2i{x - rhs.x, y - rhs.y};
    }

    Vector2i operator/(int rhs) const
    {
        return Vector2i{x / rhs, y / rhs};
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

int total_distance(Vector2i origin, const std::vector<Vector2i>& points)
{
    int dist = 0;
    for (Vector2i target : points) {
        dist += distance(origin, target);
    }
    return dist;
}

// Counts the area in x direction
int walk_x(Vector2i pos, int direction, const std::vector<Vector2i>& points)
{
    int area = 0;
    while (true) {
        int dist = total_distance(pos, points);
        if (dist < MAX_DISTANCE) {
            area++;
        } else {
            break;
        }

        pos.x += direction;
    }
    return area;
}

// Counts the area in y direction and calls walk_x for each y position
int walk_y(Vector2i pos, int direction, const std::vector<Vector2i>& points)
{
    int area = 0;
    while (true) {
        int dist = total_distance(pos, points);
        if (dist < MAX_DISTANCE) {
            area++;
        } else {
            break;
        }
        // Count the area to the right (x+)
        int area_r = walk_x({pos.x + 1, pos.y}, +1, points);
        // Count the area to the left (x-)
        int area_l = walk_x({pos.x - 1, pos.y}, -1, points);
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

    Vector2i origin = std::accumulate(
        points.begin(), points.end(), Vector2i{0, 0}) / points.size();

    // Count the area above (y+) and at the same level as the origin
    int area_u = walk_y({origin.x, origin.y + 0}, +1, points);
    // Count the area below
    int area_d = walk_y({origin.x, origin.y - 1}, -1, points);
    int total_area = area_u + area_d;

    std::printf("Total area: %d\n", total_area);

    return 0;
}
