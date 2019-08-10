#ifndef INC_3DMINESWEEPER_GRID_HPP
#define INC_3DMINESWEEPER_GRID_HPP

#include <GL/glut.h>

class Grid {
private:
    int x;
    int y;
public:
    Grid(int x, int y);

    void drawGrid();

    int getX() const;

    int getY() const;
};


#endif //INC_3DMINESWEEPER_GRID_HPP
