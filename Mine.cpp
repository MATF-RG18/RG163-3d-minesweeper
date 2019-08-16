#include "Mine.hpp"

void Mine::drawMine() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();
        glTranslatef(this->x, 0.0, this->y);
        float radius = 0.28;
        float angle = 0;
        glColor3f(0.2, 0.2, 0.2);
        glutSolidSphere(radius, 20, 20);
    while(angle < 360) {
        glPushMatrix();
            glColor3f(0.2, 0.2, 0.2);
            glRotatef(angle, 0.0, 1.0, 0.0);
            glutSolidCone(radius*0.8, radius*1.3, 30, 30);
            glColor3f(0, 0, 0);
            glutWireCone(radius*0.8, radius*1.3, 30, 30);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.2, 0.2, 0.2);
            glRotatef(angle, 1.0, 1.0, 0.0);
            glutSolidCone(radius*0.8, radius*1.3, 30, 30);
            glColor3f(0, 0, 0);
            glutWireCone(radius*0.8, radius*1.3, 30, 30);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.2, 0.2, 0.2);
            glRotatef(angle, 1.0, -1.0f, 0.0);
            glutSolidCone(radius*0.8, radius*1.3, 30, 30);
            glColor3f(0, 0, 0);
            glutWireCone(radius*0.8, radius*1.3, 30, 30);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.2, 0.2, 0.2);
            glRotatef(angle, 1.0, 0.0, 0.0);
            glutSolidCone(radius*0.8, radius*1.3, 30, 30);
            glColor3f(0, 0, 0);
            glutWireCone(radius*0.8, radius*1.3, 30, 30);
        glPopMatrix();
        angle += 45;
    }
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
}

Mine::Mine(int x, int y, float r)
    : x(x), y(y), radius(r) {}
