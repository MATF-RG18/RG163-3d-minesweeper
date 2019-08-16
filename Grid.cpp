#include "Grid.hpp"
#include "Field.hpp"
#include "Mine.hpp"

Grid::Grid(int x, int y)
    : x(x), y(y), selected(false) {}

void Grid::drawGrid() {
    glColor3f(0.360, 0.741, 0.941);
    glBegin(GL_POLYGON); /* Oduzimanje sa 0.5 zbog kocke koja se crta u sredini */
        glVertex3f(this->x - 0.5f, -0.5f, this->y - 0.5f);
        glVertex3f(this->x - 0.5f, -0.5f, this->y + 0.5f);
        glVertex3f(this->x + 0.5f, -0.5f, this->y + 0.5f);
        glVertex3f(this->x + 0.5f, -0.5f, this->y - 0.5f);
    glEnd();
    float y1;
    if(this->isSelected()) {
        glColor3f(0, 0, 0);
        y1 = -0.49f;
    }
    else {
        glColor3f(0.184, 0.505, 0.674);
        y1 = -0.5f;
    }
    glBegin(GL_LINE_LOOP);
        glVertex3f(this->x - 0.5f, y1, this->y - 0.5f);
        glVertex3f(this->x - 0.5f, y1, this->y + 0.5f);
        glVertex3f(this->x + 0.5f, y1, this->y + 0.5f);
        glVertex3f(this->x + 0.5f, y1, this->y - 0.5f);
    glEnd();
}

int Grid::getX() const {
    return x;
}

int Grid::getY() const {
    return y;
}

bool Grid::isSelected() const {
    return selected;
}

void Grid::setSelected(bool selected) {
    Grid::selected = selected;
}
