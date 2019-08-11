#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Field.hpp"
#include "Board.hpp"
#include "Grid.hpp"
#include "Mine.hpp"
#include "BMPLoader.hpp"

#define TIMER_ID (0)
#define boardWidth (9)
#define boardHeight (9)
#define numOfMines (10)
#define FILENAME "../Numbers/"

/* Dimenzije prozora */
static int window_width, window_height;
/* Koordinate pokazivaca misa */
static int mouse_x, mouse_y;
/* Kumulativna matrica rotacije */
static float matrix[16];
/* Faktor za zumiranje */
static float zoom_x = 1.0;
static float zoom_y = 1.0;
/* Konstanta pi. */
const static float pi = 3.141592653589793;
unsigned int ID;

/* Deklaracija funkcije za OpenGL inicijalizaciju  */
static void init();

/* Deklaracija callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display();
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);
static void on_timer(int id);

/* Implementirane f-je */
static void coordinate_system();

/* Funkcije nezavisne od OpenGL-a
int** initMatrix(int **matrix, int size); */

static void add_texture(int i);

static GLuint names[10];

void createMenu();

Board board = Board(boardWidth, boardHeight, numOfMines);

BMPLoader image = BMPLoader(0, 0);

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    /* Kreira se prozor. */
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Minesweeper");

    /* TODO:Kreiranje menija */

    /* Registruju se callback funkcije. */
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);

    /* Inicijalizacija koordinata misa */
    mouse_x = 0;
    mouse_y = 0;

    init();
    board.initBoard();

    /* Ulazimo u glavnu petlju */
    glutMainLoop();

    return 0;
}

static void init()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(1.0, 1.0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);

    /* Promena debljine linije */
    glLineWidth(3);

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(10, names);

    for(int i = 1; i < 9; i++) {
        char iStr[2];
        sprintf(iStr, "%d", i);
        char str[30];
        strcpy(str, FILENAME);
        strcat(str, iStr);
        strcat(str, ".bmp");
        image.BMPRead(str);
        add_texture(i);
    }

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Inicijalizacija matrice rotacije */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
}

static void on_display()
{
    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(13, 20, 8,
              0, 0, 0,
              0, 1, 0);

    /* Primena matrice rotacije */
    glMultMatrixf(matrix);

    //coordinate_system();

    board.drawBoard();
    board.printValues(names); /* samo za proveru dok ne ispisemo brojeve na tabli */

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
}

void on_keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: { /*U slucaju esc izlazi se iz programa */
            exit(0);
            break;
        }
        case ' ': {
            Field *field = board.findSelectedField();
            field->setVisited(true);
            glutPostRedisplay();
            break;
        }
        case 'W':
        case 'w': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            if(field->getY() == 0) { /* Ukoliko je trenutno polje na gornjoj ivici table za igru */
                Field *pom = board.findSpecifiedField(field->getX(), boardHeight - 1);
                pom->setSelected(true);
            }
            else {
                Field *pom = board.findSpecifiedField(field->getX(), field->getY() - 1);
                pom->setSelected(true);
            }
            glutPostRedisplay();
            break;
        }
        case 'A':
        case 'a': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            if (field->getX() == 0) { /* Ukoliko je trenutno polje na levoj ivici table za igru */
                Field *pom = board.findSpecifiedField(boardWidth - 1, field->getY());
                pom->setSelected(true);
            } else {
                Field *pom = board.findSpecifiedField(field->getX() - 1, field->getY());
                pom->setSelected(true);
            }
            glutPostRedisplay();
            break;
        }
        case 'S':
        case 's': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            if(field->getY() == boardHeight - 1) { /* Ukoliko je trenutno polje na donjoj ivici table za igru */
                Field *pom = board.findSpecifiedField(field->getX(), 0);
                pom->setSelected(true);
            }
            else {
                Field *pom = board.findSpecifiedField(field->getX(), field->getY() + 1);
                pom->setSelected(true);
            }
            glutPostRedisplay();
            break;
        }
        case 'D':
        case 'd': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            if(field->getX() == boardWidth - 1) { /* Ukoliko je trenutno polje na desnoj ivici table za igru */
                Field *pom = board.findSpecifiedField(0, field->getY());
                pom->setSelected(true);
            }
            else {
                Field *pom = board.findSpecifiedField(field->getX() + 1, field->getY());
                pom->setSelected(true);
            }
            glutPostRedisplay();
            break;
        }
        default:
            break;
    }
}

static void on_reshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    window_width = width;
    window_height = height;

    /* Podesava se viewport. */
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, window_width/(float)window_height, 1, 200);
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

void on_mouse(int button, int state, int x, int y) {
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            break;
        case GLUT_RIGHT_BUTTON:
            break;
        case 3: /* Mouse wheel scroll up */
            
            break;
        case 4: /* Mouse wheel scrool down */
            break;
        default:
            break;
    }
    mouse_x = x;
    mouse_y = y;
}

void on_motion(int x, int y) {
    /* Promene pozicija pokazivaca misa */
    int delta_x, delta_y;

    /* Racunanje promena pozicije */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamcenje nove pozicije */
    mouse_x = x;
    mouse_y = y;

    /* Izracunavanje nove matrice rotacije. */
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glRotatef(180 * (float) delta_x / window_width, 0, 1, 0);
    glRotatef(180 * (float) delta_y / window_height, 1, 0, 0);
    glMultMatrixf(matrix);

    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();

    glutPostRedisplay();
}

void on_timer(int id) {
    if(TIMER_ID != id)
        return;

    glutPostRedisplay();
}

void add_texture(int i) {
    glBindTexture(GL_TEXTURE_2D, names[i]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image.width, image.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image.pixels);
}

