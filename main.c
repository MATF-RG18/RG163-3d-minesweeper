#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define boardSize (64)

/* Dimenzije prozora */
static int window_width, window_height;
/* Koordinate pokazivaca misa */
static int mouse_x, mouse_y;
/* Kumulativna matrica rotacije */
static float matrix[16];

typedef struct {
    float x;
    float y;
    bool has_mine;
}FIELD;

/* Deklaracija funkcije za OpenGL inicijalizaciju  */
static void init(void);

/* Deklaracija callback funkcija */
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_display(void);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);

/* Implementirane f-je */
static void coordinate_system();
static void draw_cube();
static void init_fields();

/* Funkcije nezavisne od OpenGL-a */
int** initMatrix(int **matrix, int size);

int **board;

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
    glutMouseFunc(on_mouse);
    glutMotionFunc(on_motion);

    /* Inicijalizacija koordinata misa */
    mouse_x = 0;
    mouse_y = 0;

    /* Inicijalizacija matrice rotacije */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);

    init();

    board = initMatrix(board, boardSize);

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
    gluLookAt(10, 15, 13,
              0, 0, 0,
              0, 1, 0);

    coordinate_system();

    /* Primena matrice rotacije */
    glMultMatrixf(matrix);

    int i;
    int m = 0;
    for(i = 0; i < boardSize; i++)
    {
        glPushMatrix();
        if(i != 0 && i % (int)sqrt(boardSize) == 0)
            m++;
        glTranslatef(i % (int)sqrt(boardSize), 0, m);
        draw_cube();
        glPopMatrix();
    }

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

static void draw_cube()
{
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(1);
    glColor3f(0.5, 0.5, 0.5);
    glutWireCube(1);
}

int** initMatrix(int **matrix, int size){
    matrix = (int**)calloc(size, sizeof(int*));
    for(int i = 0; i < size; i++)
        matrix[i] = (int*)calloc(size, sizeof(int));
    return matrix;
}

void on_mouse(int button, int state, int x, int y) {
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN) {
                printf("%d, %d\n", x, y);
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            break;
        case GLUT_RIGHT_BUTTON:
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

    /* Izracunavanje promena pozicije */
    delta_x = x - mouse_x;
    delta_y = y - mouse_y;

    /* Pamcenje nove pozicije */
    mouse_x = x;
    mouse_y = y;
}

void init_fields() {
    FIELD fields[boardSize];

    for(int i = 0; i < boardSize; i++)
    {
        fields[i].has_mine = false;
        /* TODO: dodati za x i y */
    }
}
