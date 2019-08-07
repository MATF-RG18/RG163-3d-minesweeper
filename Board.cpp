#include "Board.hpp"

Board::Board(int boardHeight, int boardWidth, int numberOfMines)
        : boardHeight(boardHeight),
          boardWidth(boardWidth),
          numberOfMines(numberOfMines) {

    for(int i = 0; i < boardWidth; i++) {
        for(int j = 0; j < boardHeight; j++) {
            Field* field = new Field(i, j);
            if(i == 0 && j == 0)
                field->setSelected(true);
            this->add_field(field);
        }
    }
}

void Board::add_field(Field* field) {
    fields.push_back(field);
}

void Board::draw_board() {
    for(Field* field : fields) {
        if(!field->isVisited())
            field->drawField();
    }
}

Field* Board::find_selected_field() {
    for(Field* field : fields) {
        if(field->isSelected())
            return field;
    }
}

Field *Board::find_specified_field(int x, int y) {
    for(Field* field : fields) {
        if(x == field->getX() && y == field->getY())
            return field;
    }
}
