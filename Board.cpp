#include <cstdio>
#include "Board.hpp"
#include "Mine.hpp"

Board::Board(int boardHeight, int boardWidth, int numberOfMines)
        : boardHeight(boardHeight),
          boardWidth(boardWidth),
          numberOfMines(numberOfMines) {

    for(int i = 0; i < boardWidth; i++) {
        for(int j = 0; j < boardHeight; j++) {
            Grid* grid = new Grid(i, j);
            Field* field = new Field(i, j);
            if(i == 0 && j == 0)
                field->setSelected(true);
            this->addField(field);
            this->addGrid(grid);
        }
    }
}

void Board::addField(Field* field) {
    fields.push_back(field);
}

void Board::drawBoard() {
    for(Field* field : fields) {
        if(!field->isVisited())
            field->drawField();
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
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::vector<std::pair<int, int>> all;
    while(all.size() < numberOfMines) {
        int randomX = (rand() % this->boardWidth);
        int randomY = (rand() % this->boardHeight);
        std::pair <int, int> pairXY;
        pairXY.first = randomX;
        pairXY.second = randomY;
        bool unique = true; // sluzi za proveru jedinstvenosti elemenata zbog f-je rand()
        for(unsigned j = 0; j < all.size(); j++) {
            if(all[j].first == pairXY.first && all[j].second == pairXY.second)
                unique = false;
        }
        if(unique) {
            all.push_back(pairXY);
            Field *field = findSpecifiedField(randomX, randomY);
            field->setMine(true);
            Mine* mine = new Mine(randomX, randomY, 0.33);
            this->addMine(mine);
            /*
            Mine mine = Mine(randomX, randomY, 0.33);
            mine.drawMine();*/
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

/* Ispis broja suseda (iskljucivo za proveru dok ne ispisemo na tabli za igru) */
void Board::printValues() {
    for(Field* field : fields) {
        printf("%d %d : %d\n", field->getX(), field->getY(), field->getValue());
    }
}

void Board::addMine(Mine *mine) {
    mines.push_back(mine);
}
