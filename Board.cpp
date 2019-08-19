#include <cstdio>
#include "Board.hpp"
#include "Mine.hpp"

Board::Board(int boardHeight, int boardWidth, int numberOfMines)
        : boardHeight(boardHeight),
          boardWidth(boardWidth),
          numberOfMines(numberOfMines) {}

void Board::addField(Field* field) {
    fields.push_back(field);
}

void Board::drawBoard() {
    for(Field* field : fields) {
        if(!field->isVisited()) {
            field->drawField();
            if(field->isMarked()) {
                Flag *flag = new Flag(field->getX(), field->getY());
                flag->drawFlag();
            }
        }
    }
    for(Grid* grid : grids) {
        grid->drawGrid();
    }
    for(Mine* mine : mines) {
        mine->drawMine();
    }
}

Field* Board::findSelectedField() {
    for(Field* field : fields) {
        if(field->isSelected())
            return field;
    }
}

Field* Board::findSpecifiedField(int x, int y) {
    for(Field* field : fields) {
        if(x == field->getX() && y == field->getY())
            return field;
    }
}

void Board::addGrid(Grid *grid) {
    grids.push_back(grid);
}

void Board::initBoard() {
    fields.clear();
    grids.clear();
    mines.clear();
    for(int i = 0; i < boardWidth; i++) {
        for (int j = 0; j < boardHeight; j++) {
            Grid *grid = new Grid(i, j);
            Field *field = new Field(i, j);
            if (i == 0 && j == 0) {
                field->setSelected(true);
                grid->setSelected(true);
            }
            this->addField(field);
            this->addGrid(grid);
        }
    }
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::pair<int, int>> all;
    while(all.size() < numberOfMines) {
        int randomX = (rand() % this->boardWidth);
        int randomY = (rand() % this->boardHeight);
        std::pair <int, int> pairXY;
        pairXY.first = randomX;
        pairXY.second = randomY;
        bool unique = true; // sluzi za proveru jedinstvenosti elemenata zbog f-je rand()
        for (auto &j : all) {
            if(j.first == pairXY.first && j.second == pairXY.second)
                unique = false;
        }
        if(unique) {
            all.push_back(pairXY);
            Field *field = findSpecifiedField(randomX, randomY);
            field->setMine(true);
            Mine* mine = new Mine(randomX, randomY);
            this->addMine(mine);
        }
    }
    calculateValues();
}

Grid *Board::findSpecifiedGrid(int x, int y) {
    for(Grid* grid : grids) {
        if(x == grid->getX() && y == grid->getY())
            return grid;
    }
}

int Board::calculateValues() {
    for(Field* field : fields) {
        if(field->isMine()) {
            field->setValue(-1);
        }
        else {
            int count = 0;
            int x = field->getX();
            int y = field->getY();
            for (int i = x - 1; i <= x + 1; i++) {
                if (i < 0 || i >= boardWidth) continue;

                for (int j = y - 1; j <= y + 1; j++) {
                    if (j < 0 || j >= boardHeight) continue;

                    Field *neighbour = findSpecifiedField(i, j);
                    if (neighbour->isMine())
                        count++;
                }
            }
            field->setValue(count);
        }
    }
}

void Board::printValues(GLuint *names) {
    for(Field* field : fields) {
        int value = field->getValue();
        if(value == -1 || value == 0) continue; // ukoliko je bomba ili nema suseda
        Grid *grid = findSpecifiedGrid(field->getX(), field->getY());
        glBindTexture(GL_TEXTURE_2D, names[value]);
        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            /* Umesto 0.5 stavljeno je 0.49 kako bi se videle linije izmedju kvadratica na podlozi
             * odnosno da bi se videla tekstura (y koordinata)
             * */
            glTexCoord2f(0.3, 0);
            glVertex3f(grid->getX() - 0.49f, -0.49f, grid->getY() - 0.49f);

            glTexCoord2f(1, 0);
            glVertex3f(grid->getX() - 0.49f, -0.49f, grid->getY() + 0.49f);

            glTexCoord2f(1, 0.9);
            glVertex3f(grid->getX() + 0.49f, -0.49f, grid->getY() + 0.49f);

            glTexCoord2f(0.3, 0.9);
            glVertex3f(grid->getX() + 0.49f, -0.49f, grid->getY() - 0.49f);
        glEnd();
    }
}

void Board::addMine(Mine *mine) {
    mines.push_back(mine);
}

Grid *Board::findSelectedGrid() {
    for(Grid *grid : grids){
        if(grid->isSelected())
            return grid;
    }
}

void Board::visitAllZeroValueNeighbours(Field *field) {
    int x = field->getX();
    int y = field->getY();
    for (int i = x - 1; i <= x + 1; i++) {
        if (i < 0 || i >= boardWidth) continue;
        for (int j = y - 1; j <= y + 1; j++) {
            if (j < 0 || j >= boardHeight) continue;
            Field *neighbour = findSpecifiedField(i, j);
            if (!neighbour->isVisited()) {
                neighbour->setVisited(true);
                if (neighbour->getValue() == 0)
                    visitAllZeroValueNeighbours(neighbour);
            }
        }
    }
}

void Board::change(int boardHeight, int boardWidth, int numberOfMines) {
    this->boardHeight = boardHeight;
    this->boardWidth = boardWidth;
    this->numberOfMines = numberOfMines;
}

void Board::show() {
    for(Field *field : fields)
        field->setVisited(true);
}

void Board::visitAllNeighbours(Field *field) {
    int x = field->getX();
    int y = field->getY();
    for (int i = x - 1; i <= x + 1; i++) {
        if (i < 0 || i >= boardWidth) continue;
        for (int j = y - 1; j <= y + 1; j++) {
            if (j < 0 || j >= boardHeight) continue;
            Field *neighbour = findSpecifiedField(i, j);
            if (!neighbour->isVisited() && !neighbour->isMarked()) {
                neighbour->setVisited(true);
                if(neighbour->getValue() == -1) // ukoliko je bomba
                    this->show(); // GAME OVER / prikazi sve
                if(neighbour->getValue() == 0)
                    visitAllZeroValueNeighbours(neighbour);
            }
        }
    }
}
