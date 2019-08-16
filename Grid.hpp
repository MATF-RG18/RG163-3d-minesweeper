#ifndef INC_3DMINESWEEPER_GRID_HPP
#define INC_3DMINESWEEPER_GRID_HPP

#include <GL/glut.h>

class Grid {
private:
    int x;
    int y;
    bool selected;
public:
    Grid(int x, int y);

    void drawGrid();

    int getX() const;

    int getY() const;

    bool isSelected() const;

    void setSelected(bool selected);
};


#endif //INC_3DMINESWEEPER_GRID_HPP
