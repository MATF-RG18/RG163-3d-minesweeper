#include "Field.hpp"

Field::Field(int x, int y)
    : x(x),
      y(y),
      visited(false),
      mine(false),
      marked(false),
      value(0),
      selected(false) {}

void Field::drawField() {
    glPushMatrix();
        glTranslatef(this->x, 0, this->y);
        if(this->isSelected())
            glColor3f(0.5, 0.5, 0.5);
        else
            glColor3f(0.7, 0.7, 0.7);
        glutSolidCube(1);
        glColor3f(0.5, 0.5, 0.5);
        glutWireCube(1);
    glPopMatrix();
}

bool Field::isVisited() const {
    return visited;
}

void Field::setVisited(bool visited) {
    Field::visited = visited;
}

bool Field::isMine() const {
    return mine;
}

void Field::setMine(bool mine) {
    Field::mine = mine;
}

bool Field::isMarked() const {
    return marked;
}

void Field::setMarked(bool marked) {
    Field::marked = marked;
}

int Field::getValue() const {
    return value;
}

void Field::setValue(int value) {
    Field::value = value;
}

bool Field::isSelected() const {
    return selected;
}

void Field::setSelected(bool selected) {
    Field::selected = selected;
}

float Field::getX() const {
    return x;
}

float Field::getY() const {
    return y;
}
