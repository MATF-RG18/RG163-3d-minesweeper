#ifndef INC_3DMINESWEEPER_FLAG_HPP
#define INC_3DMINESWEEPER_FLAG_HPP

#include <GL/glut.h>

class Flag {
private:
    int x;
    int y;
public:
    Flag(int x, int y);

    void drawFlag();
};


#endif //INC_3DMINESWEEPER_FLAG_HPP
