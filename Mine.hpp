#ifndef INC_3DMINESWEEPER_MINE_HPP
#define INC_3DMINESWEEPER_MINE_HPP

#include <GL/glut.h>

class Mine {
private:
    int x;
    int y;
    float radius;
public:
    Mine(int x, int y);

    void drawMine();
};


#endif //INC_3DMINESWEEPER_MINE_HPP
