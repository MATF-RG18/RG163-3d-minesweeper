#include "Flag.hpp"

Flag::Flag(int x, int y)
    : x(x), y(y) {}

void Flag::drawFlag() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPushMatrix();
        glTranslatef(this->x, 0.0, this->y);
        glPushMatrix();
        glColor3f(0, 0, 0);
        /* 6 -> 0.2 * 6 = 1.2 */
        glTranslatef(0, 0.56, 0);
        glScalef(1, 0.2, 1);
        glutSolidCube(0.6);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0, 0.62, 0);
        glRotatef(270, 1, 0, 0);
        GLUquadricObj *obj = gluNewQuadric();
        gluCylinder(obj, 0.05, 0.05, 0.6, 64, 64);
        glPopMatrix();

        glColor3f(1, 0, 0);
        glBegin(GL_TRIANGLES);
        glVertex3f(0, 1.2, 0);
        glVertex3f(0, 0.8, 0);
        glVertex3f(0, 1, 0.3f);
        glEnd();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}
