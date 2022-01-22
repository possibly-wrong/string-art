#ifndef GRAPH_BRESENHAM_H
#define GRAPH_BRESENHAM_H

#include <cmath>

namespace graph
{
    // Call plot(_, _) to draw pixels on line from (x, y) inclusive to
    // (x2, y2) exclusive.
    void draw_line(int x, int y, int x2, int y2, void(*plot)(int, int))
    {
        int dx = std::abs(x2 - x);
        int dy = -std::abs(y2 - y);
        int sx = (x < x2 ? 1 : -1);
        int sy = (y < y2 ? 1 : -1);
        int error = dx + dy;
        while (x != x2 || y != y2)
        {
            plot(x, y);
            int error2 = 2 * error;
            if (error2 > dy)
            {
                x += sx;
                error += dy;
            }
            if (error2 < dx)
            {
                y += sy;
                error += dx;
            }
        }
    }

    // Call plot(_, _) to draw pixels on circle at origin with given radius.
    void draw_circle(int radius, void(*plot)(int, int))
    {
        int x = radius;
        int y = 0;
        int error = 3 - 2 * radius;
        while (x >= y)
        {
            plot(x, y); plot(x, -y); plot(-x, y); plot(-x, -y);
            plot(y, x); plot(y, -x); plot(-y, x); plot(-y, -x);
            if (error > 0)
            {
                error -= 4 * (--x);
            }
            error += 4 * (++y) + 2;
        }
    }
} // namespace graph

#endif // GRAPH_BRESENHAM_H
