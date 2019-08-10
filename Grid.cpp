#include "Grid.hpp"
#include "Field.hpp"
#include "Mine.hpp"

Grid::Grid(int x, int y)
    : x(x), y(y) {}

void Grid::drawGrid() {
    glColor3f(0.360, 0.741, 0.941);
    glBegin(GL_POLYGON); /* Oduzimanje sa 0.5 zbog kocke koja se crta u sredini */
        glVertex3f(this->x - 0.5, -0.5, this->y - 0.5);
        glVertex3f(this->x - 0.5, -0.5, this->y + 0.5);
        glVertex3f(this->x + 0.5, -0.5, this->y + 0.5);
        glVertex3f(this->x + 0.5, -0.5, this->y - 0.5);
    glEnd();
    glColor3f(0.184, 0.505, 0.674);
    glBegin(GL_LINE_LOOP);
        glVertex3f(this->x - 0.5, -0.5, this->y - 0.5);
        glVertex3f(this->x - 0.5, -0.5, this->y + 0.5);
        glVertex3f(this->x + 0.5, -0.5, this->y + 0.5);
        glVertex3f(this->x + 0.5, -0.5, this->y - 0.5);
    glEnd();
}

int Grid::getX() const {
    return x;
}

int Grid::getY() const {
    return y;
}
