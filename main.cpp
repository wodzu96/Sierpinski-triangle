/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi  
//  układu współrzednych
/*************************************************************************************/
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

typedef float point3[3];
static GLfloat camera[]= {0.0, 0.0, 0.0};
static GLfloat viewer[]= {0.0, 0.0, 0.0};
static GLfloat distance[] = {0,0,0};
const int lPoziomow = 1;
static GLfloat fi = 1.0;   // kąt obrotu obiektu
static GLfloat jota = 1.0;   // kąt obrotu obiektu
static GLfloat zoom = 1.0;
static GLfloat pix2angle;
bool w,s,a,d, up, down = false;
float randFloat(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
struct point
{
    float x;
    float y;
    float z;
};
/*************************************************************************************/
// Funkcja rysująca osie układu współrzędnych
void refreshScreen()
{
    if(d) {
        jota += 0.03;
    }
    if(a)
        jota-=0.03;
    if(s){
        fi-=0.03;
      }
    if(w)
        fi+=0.03;
    float R = 100;
    camera[0] = R*cos(jota)*cos(fi);
    camera[1]=R*sin(fi);
    camera[2]=R*sin(jota)*cos(fi);
    if(up) {
        for(int i = 0; i<3; ++i){
            distance[i] = camera[i] - viewer[i];
            viewer[i] += distance[i] / 1000;

        }
    }
    if(down){
        for(int i = 0; i<3; ++i){
            distance[i] = camera[i] - viewer[i];
            viewer[i] -= distance[i] / 1000;

        }
    }

    glutPostRedisplay(); //odświeżenie zawartości aktualnego okna
}


void Axes(void)
{
    point3  x_min = {-5.0, 0.0, 0.0};
    point3  x_max = { 5.0, 0.0, 0.0};
// początek i koniec obrazu osi x

    point3  y_min = {0.0, -5.0, 0.0};
    point3  y_max = {0.0,  5.0, 0.0};
// początek i koniec obrazu osi y

    point3  z_min = {0.0, 0.0, -5.0};
    point3  z_max = {0.0, 0.0,  5.0};
//  początek i koniec obrazu osi y
    glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
    glBegin(GL_LINES); // rysowanie osi x
    glVertex3fv(x_min);
    glVertex3fv(x_max);
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
    glBegin(GL_LINES);  // rysowanie osi y
    glVertex3fv(y_min);
    glVertex3fv(y_max);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
    glBegin(GL_LINES); // rysowanie osi z
    glVertex3fv(z_min);
    glVertex3fv(z_max);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();
}
/*************************************************************************************/


/*Rysuję prostopadłościan poprzez złożenie 4 trójkątów i jednego kwadratu */
void drawPyramid(point top, float height){
    glBegin(GL_TRIANGLES);
    glTexCoord2f(top.x+top.z, top.y);
    glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(top.x-(height/2)+top.z+(height/2), top.y-height);
    glVertex3f(top.x-(height/2), top.y-height, top.z+(height/2));
    glTexCoord2f(top.x+(height/2)+top.z+(height/2), top.y-height);
    glVertex3f(top.x+(height/2), top.y-height, top.z+(height/2));
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(top.x+top.z, top.y);
    glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(top.x+(height/2)+top.z+(height/2), top.y-height);
    glVertex3f(top.x+(height/2), top.y-height, top.z+(height/2));
    glTexCoord2f(top.x+(height/2)+top.z-(height/2), top.y-height);
    glVertex3f(top.x+(height/2), top.y-height, top.z-(height/2));
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(top.x+ top.z, top.y);
    glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(top.x+(height/2)+top.z-(height/2), top.y-height);
    glVertex3f(top.x+(height/2), top.y-height, top.z-(height/2));
    glTexCoord2f(top.x-(height/2)+top.z-(height/2), top.y-height);
    glVertex3f(top.x-(height/2), top.y-height, top.z-(height/2));
    glEnd();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(top.x+ top.z, top.y);
    glVertex3f(top.x, top.y, top.z);
    glTexCoord2f(top.x-(height/2)+top.z-(height/2), top.y-height);
    glVertex3f(top.x-(height/2), top.y-height, top.z-(height/2));
    glTexCoord2f(top.x-(height/2)+top.z+(height/2), top.y-height);
    glVertex3f(top.x-(height/2), top.y-height, top.z+(height/2));

    glEnd();

    glBegin(GL_POLYGON);
    glTexCoord2f(top.x-(height/2),top.z+(height/2));
    glVertex3f(top.x-(height/2), top.y-height, top.z+(height/2));
    glTexCoord2f(top.x-(height/2), top.z-(height/2));
    glVertex3f(top.x-(height/2), top.y-height, top.z-(height/2));
    glTexCoord2f(top.x+(height/2), top.z-(height/2));
    glVertex3f(top.x+(height/2), top.y-height, top.z-(height/2));
    glTexCoord2f(top.x+(height/2),top.z+(height/2));
    glVertex3f(top.x+(height/2), top.y-height, top.z+(height/2));
    glEnd();


}

void drawFivePyramids(point top, float height, int level){
    point floorLeft, floorLeftDeep, floorRight, floorRightDeep;
    /*wyliczam współrzędne 4 punktów podstawy prostopadłościanu,
     * którego wysokoś jest dwa razy mniejsza a punkt startowy (top) ten sam */
    floorLeft.x = top.x - height/4;
    floorLeft.y = top.y - height/2;
    floorLeft.z = top.z - height/4;

    floorLeftDeep.x = top.x - height/4;
    floorLeftDeep.y = top.y - height/2;
    floorLeftDeep.z = top.z + height/4;

    floorRight.x = top.x + height/4;
    floorRight.y = top.y - height/2;
    floorRight.z = top.z - height/4;

    floorRightDeep.x = top.x + height/4;
    floorRightDeep.y = top.y - height/2;
    floorRightDeep.z = top.z + height/4;

    if(level>0){
        //jeżeli level jest większy od 0 rekurencyjnie ta funkcja wywołuje samą siebie
        drawFivePyramids(top, height/2, level-1);
        drawFivePyramids(floorLeft, height/2, level-1);
        drawFivePyramids(floorRight, height/2, level-1);
        drawFivePyramids(floorLeftDeep, height/2, level-1);
        drawFivePyramids(floorRightDeep, height/2, level-1);
    }
    else {
        drawPyramid(top, height/2);
        drawPyramid(floorLeft, height / 2);
        drawPyramid(floorRight, height / 2);
        drawPyramid(floorLeftDeep, height / 2);
        drawPyramid(floorRightDeep, height / 2);
    }
}

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
/*************************************************************************************/
// Struktura dla nagłówka pliku  TGA

#pragma pack(1)
    typedef struct
    {
        GLbyte    idlength;
        GLbyte    colormaptype;
        GLbyte    datatypecode;
        unsigned short    colormapstart;
        unsigned short    colormaplength;
        unsigned char     colormapdepth;
        unsigned short    x_orgin;
        unsigned short    y_orgin;
        unsigned short    width;
        unsigned short    height;
        GLbyte    bitsperpixel;
        GLbyte    descriptor;
    }TGAHEADER;
#pragma pack(8)

    FILE *pFile;
    TGAHEADER tgaHeader;
    unsigned long lImageSize;
    short sDepth;
    GLbyte    *pbitsperpixel = NULL;

/*************************************************************************************/
// Wartości domyślne zwracane w przypadku błędu

    *ImWidth = 0;
    *ImHeight = 0;
    *ImFormat = GL_BGR_EXT;
    *ImComponents = GL_RGB8;

    pFile = fopen(FileName, "rb");
    if(pFile == NULL)
        return NULL;
/*************************************************************************************/
// Przeczytanie nagłówka pliku

    fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

/*************************************************************************************/
// Odczytanie szerokości, wysokości i głębi obrazu

    *ImWidth = tgaHeader.width;
    *ImHeight = tgaHeader.height;
    sDepth = tgaHeader.bitsperpixel / 8;

/*************************************************************************************/
// Sprawdzenie, czy głębia spełnia założone warunki (8, 24, lub 32 bity)

    if(tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
        return NULL;

/*************************************************************************************/
// Obliczenie rozmiaru bufora w pamięci

    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

/*************************************************************************************/
// Alokacja pamięci dla danych obrazu

    pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

    if(pbitsperpixel == NULL)
        return NULL;

    if(fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
    {
        free(pbitsperpixel);
        return NULL;
    }

/*************************************************************************************/
// Ustawienie formatu OpenGL

    switch(sDepth)
    {
        case 3:
            *ImFormat = GL_BGR_EXT;
            *ImComponents = GL_RGB8;
            break;
        case 4:
            *ImFormat = GL_BGRA_EXT;
            *ImComponents = GL_RGBA8;
            break;
        case 1:
            *ImFormat = GL_LUMINANCE;
            *ImComponents = GL_LUMINANCE8;
            break;
    };

    fclose(pFile);

    return pbitsperpixel;
}


void RenderScene(void)
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
// Czyszczenie macierzy bieżącej
    gluLookAt(viewer[0],viewer[1],viewer[2], camera[0], camera[1], camera[2], 0.0, 1.0, 0.0);
// Narysowanie osi przy pomocy funkcji zdefiniowanej wyżej
    Axes();
    point top;
    top.x = 0;
    top.y = 5;
    top.z = 0;
    drawFivePyramids(top, 10, lPoziomow);


    glFlush();
// Przekazanie poleceń rysujących do wykonania

    glutSwapBuffers();

//

}
void keys(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            w = true;
            break;
        case 's':
            s = true;
            break;
        case 'a':
            a = true;
            break;
        case 'd':
            d = true;
            break;
        case 'o':
            up = true;
            break;
        case 'l':
            down = true;
        default:
            break;
    }
}
void keyUp (unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            w = false;
            break;
        case 's':
            s = false;
            break;
        case 'a':
            a = false;
            break;
        case 'd':
            d = false;
            break;
        case 'o':
            up = false;
            break;
        case 'l':
            down = false;
        default:
            break;
    }
}
// Funkcja ustalająca stan renderowania

void MyInit(void)
{
    GLbyte *pBytes;
    GLint ImWidth, ImHeight, ImComponents;
    GLenum ImFormat;

    // ..................................
    //       Pozostała część funkcji MyInit()
    // ..................................

/*************************************************************************************/
// Teksturowanie będzie prowadzone tyko po jednej stronie ściany

    glEnable(GL_CULL_FACE);

/*************************************************************************************/
//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

    pBytes = LoadTGAImage("P5_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

    /*************************************************************************************/
// Zdefiniowanie tekstury 2-D

    glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

/*************************************************************************************/
// Zwolnienie pamięci

    free(pBytes);

/*************************************************************************************/
// Włączenie mechanizmu teksturowania

    glEnable(GL_TEXTURE_2D);

/*************************************************************************************/
// Ustalenie trybu teksturowania

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

/*************************************************************************************/
// Określenie sposobu nakładania tekstur

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
// Kolor czyszcący (wypełnienia okna) ustawiono na czarny
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są 
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical )
{
    pix2angle = 360.0/(float)horizontal;  // przeliczenie pikseli na stopnie
    glMatrixMode(GL_PROJECTION);
    // Przełączenie macierzy bieżącej na macierz projekcji

    glLoadIdentity();
    // Czyszcznie macierzy bieżącej

    gluPerspective(70, 1.0, 1.0, 30.0);
    // Ustawienie parametrów dla rzutu perspektywicznego


    if(horizontal <= vertical)
        glViewport(0, (vertical-horizontal)/2, horizontal, horizontal);

    else
        glViewport((horizontal-vertical)/2, 0, vertical, vertical);
    // Ustawienie wielkości okna okna widoku (viewport) w zależności
    // relacji pomiędzy wysokością i szerokością okna

    glMatrixMode(GL_MODELVIEW);
    // Przełączenie macierzy bieżącej na macierz widoku modelu

    glLoadIdentity();
    // Czyszczenie macierzy bieżącej

}
/*************************************************************************************/
// Główny punkt wejścia programu. Program działa w trybie konsoli

int main(int argc, char ** argv)
{
    srand( time( NULL ) );
    glutInit(&argc, argv);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);

    glutInitWindowSize(300, 300);

    glutCreateWindow("Układ współrzędnych 3-D");
    glutKeyboardFunc(keys);
    glutKeyboardUpFunc(keyUp);
    glutDisplayFunc(RenderScene);
// Określenie, że funkcja RenderScene będzie funkcją zwrotną
// (callback function).  Bedzie ona wywoływana za każdym razem 
// gdy zajdzie potrzba przeryswania okna 
    glutReshapeFunc(ChangeSize);
// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
// zazmiany rozmiaru okna      
    MyInit();
    glutIdleFunc(refreshScreen);
// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
// inicjalizacje konieczne  przed przystąpieniem do renderowania
    glEnable(GL_DEPTH_TEST);
// Włączenie mechanizmu usuwania powierzchni niewidocznych

    glutMainLoop();
// Funkcja uruchamia szkielet biblioteki GLUT
    return 0;
}
/*************************************************************************************/