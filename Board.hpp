#ifndef INC_3DMINESWEEPER_BOARD_HPP
#define INC_3DMINESWEEPER_BOARD_HPP


#include <vector>
#include <ctime>
#include <utility>
#include "Field.hpp"
#include "Grid.hpp"
#include "Mine.hpp"

class Board {
private:
    int boardWidth;
    int boardHeight;
    int numberOfMines;
    std::vector<Field*> fields;
    std::vector<Grid*> grids;
    std::vector<Mine*> mines;
public:
    Board(int boardHeight, int boardWidth, int numberOfMines);

    void addField(Field* field);

    void drawBoard();

    Field* findSelectedField();

    Field* findSpecifiedField(int x, int y);

    void addGrid(Grid* grid);

    void initBoard();

    Grid* findSpecifiedGrid(int x, int y);

    int calculateValues();

    void printValues(GLuint *names);

    void addMine(Mine* mine);
};


#endif //INC_3DMINESWEEPER_BOARD_HPP
