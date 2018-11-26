#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

/* Dimenzije prozora */
static int window_width, window_height;

/* Deklaracija funkcije za OpenGL inicijalizaciju  */
static void init(void);

/* Deklaracija callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void coordinate_system();

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Minesweeper");

    /* Registruju se callback funkcije. */
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);

    init();

    /* Ulazimo u glavnu petlju */
    glutMainLoop();
    return 0;
}

static void init(void)
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    /* Promena debljine linije */
    glLineWidth(3);
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27:
            /*U slucaju esc izlazimo iz programa*/
            exit(0);
            break;
    }
}

static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height;
}

static void on_display(void)
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, window_width/(float)window_height, 1, 200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 10,
              0, 0, 0,
              0, 1, 0);

    coordinate_system();

    glutSwapBuffers();
}

static void coordinate_system()
{
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(100, 0, 0);
        glVertex3f(0, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 100, 0);
        glVertex3f(0, 0, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 100);
        glVertex3f(0, 0, 0);
    glEnd();
}