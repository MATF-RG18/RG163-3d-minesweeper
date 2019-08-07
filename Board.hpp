#ifndef INC_3DMINESWEEPER_BOARD_H
#define INC_3DMINESWEEPER_BOARD_H


#include <vector>
#include "Field.hpp"

class Board {
private:
    int boardWidth;
    int boardHeight;
    int numberOfMines;
    std::vector<Field*> fields;
public:
    Board(int boardHeight, int boardWidth, int numberOfMines);

    void add_field(Field* field);

    void draw_board();

    Field* find_selected_field();

    Field* find_specified_field(int x, int y);
};


#endif //INC_3DMINESWEEPER_BOARD_H
