#ifndef INC_3DMINESWEEPER_FIELD_HPP
#define INC_3DMINESWEEPER_FIELD_HPP

#include <GL/glut.h>

/* Respresents one cube to be opened */
class Field {
private:
    int x;
    int y;
    bool visited; /* Otvoreno polje / pretisnut space */
    bool mine; /* Polje sa minom */
    bool marked; /* Polje obelezeno kao bomba */
    bool selected; /* Trenutno polje */
    int value; /* broj koji je upisan u polje / broj okolnih bombi */
public:
    Field(int x, int y);

    bool isVisited() const;

    void setVisited(bool visited);

    bool isMine() const;

    void setMine(bool mine);

    bool isMarked() const;

    void setMarked(bool marked);

    int getValue() const;

    void setValue(int value);

    bool isSelected() const;

    void setSelected(bool selected);

    void drawField();

    int getX() const;

    int getY() const;
};


#endif //INC_3DMINESWEEPER_FIELD_HPP
