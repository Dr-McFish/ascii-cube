#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "drawline.h"

void draw_line(int x0, int y0, int x1, int y1, char grid[GRID_HEIGHT][GRID_WIDTH])
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;


    grid[y0][x0] = '#';

    do {
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }

        grid[y0][x0] = '#';
    } while (!(x0 == x1 && y0 == y1));

}


