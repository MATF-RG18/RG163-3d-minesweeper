#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cmath>
#include <string>
#include "Field.hpp"
#include "Board.hpp"
#include "Grid.hpp"
#include "Mine.hpp"
#include "Flag.hpp"
#include "BMPLoader.hpp"

#define FILENAME "../Numbers/"
/* Minimalni i maksimalni poluprecnik sfere */
#define MAX_R (20)
#define MIN_R (4)

static int boardWidth = 9;
static int boardHeight = 9;
static int numOfMines = 10;

/* Dimenzije prozora */
static int windowWidth = 1280, windowHeight = 720;
/* Uglovi u sfernom koordinatnom sistemu koji
   odredjuju polozaj vidne tacke i njihovi inkrementi. */
static double phi, theta;
static double deltaPhi, deltaTheta;
/* Konstanta pi. */
const static float pi = 3.141592653589793;

/* Promenljive za podesavanja / izmene svetla*/
static float centerX;
static float centerY = 0;
static float centerZ;
static float r = 7;

/* Deklaracija funkcije za OpenGL inicijalizaciju  */
static void init();

/* Deklaracija callback funkcija */
static void onKeyboard(unsigned char key, int x, int y);
static void onReshape(int width, int height);
static void onDisplay();
static void onMouse(int button, int state, int x, int y);
static void onSpecialKey(int key, int x, int y);

static void coordinateSystem();

/* 1   pomeranje na gore   W
 * 2   pomeranje na dole   S
 * 3   pomeranje na levo   A
 * 4   pomeranje na desno  D
 *
 * -1  gornja ivica table
 * -2  donja ivica table
 * -3  leva ivica table
 * -4  desna ivica table
 *
 * 0   default / poziva se initCamera()
 * -5  identifikator da se ne pomera kamera niti inicijalizuje
 *
 *        (-1)
 *         |
 *         |
 *         |
 * (-3)---------(-4)
 *         |
 *         |
 *         |
 *        (-2)
 *
 * */
static int moved = 0;

static void initCamera();
static void moveCamera();

static void addTexture(int i);
static GLuint names[9];

/* Pravljenje objekta table za igru i objekta za ocitavanje bmp fajlova */
static Board board = Board(boardWidth, boardHeight, numOfMines);
static BMPLoader image = BMPLoader(0, 0);

int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Minesweeper");

    /* Registruju se callback funkcije. */
    glutDisplayFunc(onDisplay);
    glutKeyboardFunc(onKeyboard);
    glutReshapeFunc(onReshape);
    glutMouseFunc(onMouse);
    glutSpecialFunc(onSpecialKey);

    /* Inicijalizacija uglova za sferne koordinate
     * koje su koriscene za pogled */
    phi = pi / 5;
    theta = 5 * pi / 6;
    deltaPhi = deltaTheta = pi / 90;

    /* Inicijalizacija teksta, teksture i sl. u zasebnoj funkciji
     * kao i inicijalizacija table za igru */
    init();
    board.initBoard();

    /* Ulazimo u glavnu petlju */
    glutMainLoop();

    return 0;
}

static void init()
{
    // Koeficijenti potrebni za postavljanje svetla
    GLfloat lightAmbient[] = {0, 0, 0, 1};
    GLfloat lightDiffuse[] = {0.8, 0.8, 0.8, 1};
    GLfloat lightSpecular[] = {1, 1, 1, 1};

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
        char iStr[3];
        sprintf(iStr, "%d", i);
        char str[30];
        strcpy(str, FILENAME);
        strcat(str, iStr);
        strcat(str, ".bmp");
        image.BMPRead(str);
        addTexture(i);
    }

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Ukljucivanje svetla */
    glEnable(GL_LIGHTING);
    /* Postavljanje svojstva prvog svetla */
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glShadeModel(GL_SMOOTH);
}

static void onDisplay()
{
    GLfloat lightPosition[] = {0.5, 0.5, 0.5, 1};

    // Koeficijenti potrebni za postavljanje materijala
    GLfloat coeffsAmbient[] = {1.0, 0.1, 0.1, 1};
    GLfloat coeffsDiffuse[] = {0.0, 0.0, 0.8, 1};
    GLfloat coeffsSpecular[] = {1, 1, 1, 1};
    GLfloat coefShine = 30;

    /* Brise se prethodni sadrzaj prozora. */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Ukoliko je moved = 0 inicijalizujemo kameru,
     * a za moved = -5 ne menjamo kameru
     * u ostalim slucajevima pomeramo kameru u zavisnosti
     * od pritisnutog tastera */
    if(!moved)
        initCamera();
    else if (moved != -5)
        moveCamera();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    lightPosition[0] = 0;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);

    /* Koriscenje sfernih koordinata za pogled */
    gluLookAt(r * cos(theta) * cos(phi),
              r * sin(theta),
              r * cos(theta) * sin(phi),
              0, 0, 0,
              0, 1, 0);

    /* Iskljucivanje osvetljenja */
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    //coordinateSystem();

    /* Podesavaju se parametri materijala. */
    glMaterialfv(GL_FRONT, GL_AMBIENT, coeffsAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, coeffsDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, coeffsSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, coefShine);

    glPushMatrix();
        glTranslatef(centerX, centerY, centerZ);
        board.drawBoard();
        board.printValues(names);
    glPopMatrix();

    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 27: { /*U slucaju esc izlazi se iz programa */
            exit(0);
            break;
        }
        case 'x': { /* Obelezavanje polja na kom mislio da se nalazi mina*/
            Field *field = board.findSelectedField();
            if(!field->isVisited())
                field->setMarked(!field->isMarked());
            moved = -5;
            glutPostRedisplay();
            break;
        }
        case ' ': { // space
            Field *field = board.findSelectedField();
            if(!field->isVisited()) { /* Klik na neposeceno polje */
                field->setVisited(true);
                if (field->getValue() == -1) { // ako smo kliknuli na bombu
                    board.show(); // GAME OVER - prikazi sve
                }
                /* Ukoliko nema bombi u okolini pritisnutog polja
                 * otvori sve njegove susede koje takodje nemaju mine u okolini */
                if (field->getValue() == 0) {
                    board.visitAllZeroValueNeighbours(field);
                }
            }
            /* Klik na poseceno polje.
             * Sluzi za otvaranje svih okolnih polja.
             * Uz pretpostavku  da u okolini nema mina, ubrzava resavanje
             * U suprotnom (pogresna pretpostavka) igra se zavrsava
             * Ukoliko se u okolini nalazi polje bez suseda
             * Otvaraju se i svi njegovi susedi sa funkcijom odozgo
             * visitAllZeroValueNeighbours */
            else {
                board.visitAllNeighbours(field);
            }
            moved = -5; /* postavljen identifikator da se ne treba pomerati kamera */
            glutPostRedisplay();
            break;
        }
        case 'W':
        case 'w': {
            /* 1. Trazenje trenutno selektovanog polja i njemu odgovarajuceg polja na skrivenoj tabli
             * 2. Postavljanje selektovanog na false za oba
             * 3. U odnosu na polozaj trenutnog polja nalazi se koje polje je iznad njega
             * (pomeranje na gore)
             * 4. Obelezavanje selektovano na true
             * Slicno i za ostala pomeranja
             * * * * * * * * */
            Field *field = board.findSelectedField();
            field->setSelected(false);
            Grid *grid = board.findSelectedGrid();
            grid->setSelected(false);
            /* Ukoliko je trenutno polje na donjoj ivici table za igru */
            if(field->getX() == boardHeight - 1 && grid->getX() == boardHeight - 1) {
                Field *field1 = board.findSpecifiedField(0, field->getY());
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(0, grid->getY());
                grid1->setSelected(true);
                moved = -1;
            }
            else {
                Field *field1 = board.findSpecifiedField(field->getX() + 1, field->getY());
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX() + 1, grid->getY());
                grid1->setSelected(true);
                moved = 1;
            }
            glutPostRedisplay();
            break;
        }
        case 'S':
        case 's': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            Grid *grid = board.findSelectedGrid();
            grid->setSelected(false);
            /* Ukoliko je trenutno polje na gornjoj ivici table za igru */
            if (field->getX() == 0 && grid->getX() == 0) {
                Field *field1 = board.findSpecifiedField(boardHeight - 1, field->getY());
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(boardHeight - 1, grid->getY());
                grid1->setSelected(true);
                moved = -2;
            } else {
                Field *field1 = board.findSpecifiedField(field->getX() - 1, field->getY());
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX() - 1, grid->getY());
                grid1->setSelected(true);
                moved = 2;
            }
            glutPostRedisplay();
            break;
        }
        case 'A':
        case 'a': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            Grid *grid = board.findSelectedGrid();
            grid->setSelected(false);
            /* Ukoliko je trenutno polje na desnoj ivici table za igru */
            if(field->getY() == 0 && grid->getY() == 0) {
                Field *field1 = board.findSpecifiedField(field->getX(), boardWidth - 1);
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX(), boardWidth - 1);
                grid1->setSelected(true);
                moved = -3;
            }
            else {
                Field *field1 = board.findSpecifiedField(field->getX(), field->getY() - 1);
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX(), grid->getY() - 1);
                grid1->setSelected(true);
                moved = 3;
            }
            glutPostRedisplay();
            break;
        }
        case 'D':
        case 'd': {
            Field *field = board.findSelectedField();
            field->setSelected(false);
            Grid *grid = board.findSelectedGrid();
            grid->setSelected(false);
            /* Ukoliko je trenutno polje na levoj ivici table za igru */
            if(field->getY() == boardWidth - 1 && grid->getY() == boardWidth - 1) {
                Field *field1 = board.findSpecifiedField(field->getX(), 0);
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX(), 0);
                grid1->setSelected(true);
                moved = -4;
            }
            else {
                Field *field1 = board.findSpecifiedField(field->getX(), field->getY() + 1);
                field1->setSelected(true);
                Grid *grid1 = board.findSpecifiedGrid(grid->getX(), grid->getY() + 1);
                grid1->setSelected(true);
                moved = 4;
            }
            glutPostRedisplay();
            break;
        }
        case 'R': // restart
        case 'r': {
            board.initBoard();
            glutPostRedisplay();
            moved = 0; // vracanje na default kako bi se pozvala funkcija initCamera()
            break;
        }
        case '1': { // Beginner
            boardWidth = 9;
            boardHeight = 9;
            numOfMines = 10;
            moved = 0; // vracanje na default kako bi se pozvala funkcija initCamera()
            board.change(boardWidth, boardHeight, numOfMines);
            board.initBoard();
            glutPostRedisplay();
            break;
        }
        case '2': { // Intermediate
            boardWidth = 16;
            boardHeight = 16;
            numOfMines = 40;
            moved = 0; // vracanje na default kako bi se pozvala funkcija initCamera()
            board.change(boardWidth, boardHeight, numOfMines);
            board.initBoard();
            glutPostRedisplay();
            break;
        }
        case '3': { // Expert
            boardWidth = 30;
            boardHeight = 16;
            numOfMines = 99;
            moved = 0; // vracanje na default kako bi se pozvala funkcija initCamera()
            board.change(boardWidth, boardHeight, numOfMines);
            board.initBoard();
            glutPostRedisplay();
            break;
        }
        case 'I':
        case 'i': // Zoom in
            r--;
            if(r < MIN_R)
                r = MIN_R;
            moved = -5;
            glutPostRedisplay();
            break;
        case 'O':
        case 'o': // Zoom out
            r++;
            if(r > MAX_R)
                r = MAX_R;
            moved = -5;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

static void onReshape(int width, int height)
{
    /* Pamte se sirina i visina prozora. */
    windowWidth = width;
    windowHeight = height;

    /* Podesava se viewport. */
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, windowWidth/(float)windowHeight, 1, 200);
}

/* Pomocna funkcija za iscrtavanje koordinatnog sistema
 * X - Red
 * Y - Green
 * Z - Blue
 * */
static void coordinateSystem()
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

void onMouse(int button, int state, int x, int y) {
    switch(button)
    {
        /* Mouse wheel scroll up
         * Zoom in */
        case 3:
            r--;
            if(r < MIN_R)
                r = MIN_R;
            break;
        /* Mouse wheel scrool down
         * Zoom out */
        case 4:
            r++;
            if(r > MAX_R)
                r = MAX_R;
            break;
        default:
            break;
    }
    moved = -5; // identifikator da nije bilo pomeranja na tabli
    glutPostRedisplay();
}

void addTexture(int i) {
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

void initCamera() {
    centerX = 0;
    centerZ = 0;
    phi = pi / 5;
    theta = 5 * pi / 6;
}

void moveCamera() {
    switch(moved) {
        case 1:
            centerX -= 1;
            break;
        case 2:
            centerX += 1;
            break;
        case 3:
            centerZ += 1;
            break;
        case 4:
            centerZ -= 1;
            break;
        case -1:
            centerX += boardHeight - 1;
            break;
        case -2:
            centerX -= boardHeight - 1;
            break;
        case -3:
            centerZ -= boardWidth - 1;
            break;
        case -4:
            centerZ += boardWidth - 1;
            break;
        default:
            break;
    }
}

void onSpecialKey(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_RIGHT:
            /* Dekrementiranje ugla phi. */
            phi -= deltaPhi;
            if (phi > 2 * pi) {
                phi -= 2 * pi;
            } else if (phi < 0) {
                phi += 2 * pi;
            }
            break;
        case GLUT_KEY_LEFT:
            /* Inkrementiranje ugla phi. */
            phi += deltaPhi;
            if (phi > 2 * pi) {
                phi -= 2 * pi;
            } else if (phi < 0) {
                phi += 2 * pi;
            }
            break;
        case GLUT_KEY_UP:
            /*
            * Dekrementiranje ugla theta.
            * Ugao pripada intervalu [90, 150] stepeni.
            */
            theta -= deltaTheta;
            if (theta < pi / 2) {
                theta = pi / 2;
            }
            break;
        case GLUT_KEY_DOWN:
            /*
            * Inkrementiranje ugla theta.
            * Ugao pripada intervalu [90, 150] stepeni.
            */
            theta += deltaTheta;
            if (theta > 5 * pi / 6) {
                theta = 5 * pi / 6;
            }
            break;
        default:
            break;
    }
    /* U svim slucajevima postavljamo moved na -5 sto oznacava nije bilo pomeranja
     * na tabli i ponovo iscrtavamo scenu */
    moved = -5;
    glutPostRedisplay();
}