#include <time.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#endif



using namespace std;

class Pixel
{
public:
  Pixel();
  Pixel(int, int, float, float, float);
  ~Pixel();

  int getX();
  int getY();
  float getR();
  float getG();
  float getB();

  void setPosition(int, int);
  void setColour(float, float, float);

private:
  int newX;
  int newY;
  float newR;
  float newG;
  float newB;
};


Pixel::Pixel(){

}

Pixel::Pixel(int x, int y, float r, float g, float b){
    newX = x;
    newY = y;
    newR = r;
    newG = g;
    newB = b;
}

Pixel::~Pixel(){

}
int Pixel::getX(){
    return newX;
}

int Pixel::getY(){
    return newY;
}

float Pixel::getR(){
    return newR;
}

float Pixel::getG(){
    return newG;
}

float Pixel::getB(){
    return newB;
}

void Pixel::setPosition(int x, int y){
    newX = x;
    newY = y;
}
void Pixel::setColour(float r, float g, float b){
    newR = r;
    newG = g;
    newB = b;
}



float red = 1.0, green = 0.0, blue = 0.0;
int tmpx, tmpy; // for storing 4 point in the case of rectangle or circle
int brushSize = 4;
int eraserSize = 1;
bool isSecond = false;
bool isRandom = false;
bool isEraser = false;
bool isRadial = false;
float window_w = 1536;
float window_h = 801;

int shape = 1; // 1:point, 2:line, 3:rectangle, 4:circle, 5:brush

 vector<Pixel> Pixels;		// store all the points until clear
 list<int> undoHistory; // record for undo, maximum 20 shapes in history
 list<int> redoHistory; // record for redo, maximum 20 shapes in history
 vector<Pixel> redoPixels;  // store the Pixels after undo
void drawString (void * font, char *s, float x, float y){
    unsigned int i;
    glRasterPos2f(x, y);
    for (i = 0; i < strlen (s); i++)
        glutBitmapCharacter (font, s[i]);
}




float cp[14][3]={{1,0,0},//color combinations used
             {0,1,0},
             {0,0,1},
             {1,1,0},
             {0,1,1},
             {1,0,1},
             {0,0,0},
             {1,1,1},
             {1,0.5,0},
             {0,0.5,0},
             {0.18,0.18,0.34},
             {0.6,0.19,0.84},
             {0.85,0.85,0.10},
             {0.53,0.12,0.47},
             };

bool isgrid=false;
vector <int> v;
void necess_draw(){
glColor3f(1,1,0);
drawString(GLUT_BITMAP_HELVETICA_18, "BASIC PAINT   -- IN OPENGL BY Madhukar and Mukul",540, 760);

glColor3f(1, 1,1);
glBegin(GL_POLYGON);
glVertex2f(100,80);
glVertex2f(1435,80);
glVertex2f(1435,740);
glVertex2f(100,740);
glEnd();

if(isgrid){
  for(int i=100;i<=1435;i+=10){
glColor3f(0.65,0.65,0.65);
glBegin(GL_LINES);
glVertex2f(i,80);
glVertex2f(i,740);
glEnd();

}

for(int i=80;i<=740;i+=10){
glColor3f(0.65,0.65,0.65);
glBegin(GL_LINES);
glVertex2f(100,i);
glVertex2f(1435,i);
glEnd();

}
}


glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(650,20);
glVertex2f(720,20);
glVertex2f(720,70);
glVertex2f(650,70);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "GRID", 660, 40);





int x=0;
for(int i=0;i<14;i++){
glColor3f(cp[i][0],cp[i][1],cp[i][2]);
glBegin(GL_POLYGON);
glVertex2f(750+x,20);
glVertex2f(790+x,20);
glVertex2f(790+x,70);
glVertex2f(750+x,70);
glEnd();
x+=40;
}

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(1440,730);
glVertex2f(1520,730);
glVertex2f(1520,680);
glVertex2f(1440,680);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "UNDO", 1450, 700);

int y=0;
y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(1440,730+y);
glVertex2f(1520,730+y);
glVertex2f(1520,680+y);
glVertex2f(1440,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "REDO", 1450, 700+y);

y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(1440,730+y);
glVertex2f(1520,730+y);
glVertex2f(1520,680+y);
glVertex2f(1440,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "CLEAR", 1450, 700+y);

y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(1440,730+y);
glVertex2f(1520,730+y);
glVertex2f(1520,680+y);
glVertex2f(1440,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "QUIT", 1450, 700+y);


glColor3f(1,1,0);
glBegin(GL_POLYGON);
glVertex2f(10,770);
glVertex2f(40,770);
glVertex2f(40,740);
glVertex2f(10,740);
glEnd();

glBegin(GL_POLYGON);
glVertex2f(45,770);
glVertex2f(80,770);
glVertex2f(80,740);
glVertex2f(45,740);
glEnd();

glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "+", 20, 750);
drawString(GLUT_BITMAP_HELVETICA_18, "-", 55, 750);


glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730);
glVertex2f(90,730);
glVertex2f(90,680);
glVertex2f(10,680);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "POINT", 12, 700);

y=0;
y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "LINE", 12, 700+y);

y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "CIRCLE", 12, 700+y);

y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_12, "RECTANGLE", 12, 700+y);


y-=60;

glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_10, "MAGIC BRUSH", 12, 700+y);

y-=100;
glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "ERASER", 12, 700+y);

y-=100;
glColor3f(0.65,0.65,0.65);
glBegin(GL_POLYGON);
glVertex2f(10,730+y);
glVertex2f(90,730+y);
glVertex2f(90,680+y);
glVertex2f(10,680+y);
glEnd();
glColor3f(0,0,0);
drawString(GLUT_BITMAP_HELVETICA_18, "SPRAY", 12, 700+y);




}


bool shoulddraw=0;
float pointsiz=4;
void display(void)
{
	glClearColor(0.32,0.32,0.32,1);
	glClear(GL_COLOR_BUFFER_BIT);
	necess_draw();
	glPointSize(pointsiz);

       glBegin(GL_POINTS);
	for (unsigned int i = 0; i < Pixels.size(); i++)
	{
		glColor3f(Pixels[i].getR(), Pixels[i].getG(), Pixels[i].getB());
		glVertex2i(Pixels[i].getX(), Pixels[i].getY());
	}


	glEnd();
	glutSwapBuffers();
}

void clear()
{
	Pixels.clear();
	undoHistory.clear();
	redoPixels.clear();
	redoHistory.clear();
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();

}

void quit()
{
	 cout << "EXITING" <<  endl;
	exit(0);
}
void undo()
{
	if (undoHistory.size() > 0)
	{
		if (undoHistory.back() != Pixels.size() && redoHistory.back() != Pixels.size())
		{
			redoHistory.push_back(Pixels.size());
		}
		int numRemove = Pixels.size() - undoHistory.back();
		for (int i = 0; i < numRemove; i++)
		{
			redoPixels.push_back(Pixels.back());
			Pixels.pop_back();
		}
		redoHistory.push_back(undoHistory.back());
		undoHistory.pop_back();
	}

}

void redo()
{
	if (redoHistory.size() > 1)
	{
		undoHistory.push_back(redoHistory.back());
		redoHistory.pop_back();
		int numRemove = redoHistory.back() - Pixels.size();
		for (int i = 0; i < numRemove; i++)
		{
			Pixels.push_back(redoPixels.back());
			redoPixels.pop_back();
		}
	}

}

void drawPixel(int mousex, int mousey)
{
	Pixel newPixel(mousex, window_h - mousey, isEraser ? 1.0 : red, isEraser ? 1.0 : green, isEraser ? 1.0 : blue);
	Pixels.push_back(newPixel);
}

void drawBrush(int x, int y)
{
	for (int i = 0; i < brushSize; i++)
	{
		int randX = rand() % (brushSize + 1) - brushSize / 2 + x;
		int randY = rand() % (brushSize + 1) - brushSize / 2 + y;
		drawPixel(randX, randY);
	}
}

//modified bresnan
void drawLine(int x1, int y1, int x2, int y2)
{
	bool changed = false;
	if (abs(x2 - x1) < abs(y2 - y1))
	{
		int tmp1 = x1;
		x1 = y1;
		y1 = tmp1;
		int tmp2 = x2;
		x2 = y2;
		y2 = tmp2;
		changed = true;
	}
	int dx = x2 - x1;
	int dy = y2 - y1;
	int yi = 1;
	int xi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int d = 2 * dy - dx;
	int incrE = dy * 2;
	int incrNE = 2 * dy - 2 * dx;

	int x = x1, y = y1;
	if (changed)
		drawPixel(y, x);
	else
		drawPixel(x, y);
	while (x != x2)
	{
		if (d <= 0)
			d += incrE;
		else
		{
			d += incrNE;
			y += yi;
		}
		x += xi;
		if (changed)
			drawPixel(y, x);
		else
			drawPixel(x, y);
	}
}


void drawRectangle(int x1, int y1, int x2, int y2)
{
	if (x1 < x2 && y1 < y2)
	{
		drawLine(x1, y1, x2, y1);
		drawLine(x2, y1, x2, y2);
		drawLine(x2, y2, x1, y2);
		drawLine(x1, y2, x1, y1);
	}

}


//Midpoint circle algorithm

void drawCircle(int x1, int y1, int x2, int y2)
{
	int r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	double d;
	int x, y;

	x = 0;
	y = r;
	d = 1.25 - r;

	while (x <= y)
	{
		drawPixel(x1 + x, y1 + y);
		drawPixel(x1 - x, y1 + y);
		drawPixel(x1 + x, y1 - y);
		drawPixel(x1 - x, y1 - y);
		drawPixel(x1 + y, y1 + x);
		drawPixel(x1 - y, y1 + x);
		drawPixel(x1 + y, y1 - x);
		drawPixel(x1 - y, y1 - x);
		x++;
		if (d < 0)
		{
			d += 2 * x + 3;
		}
		else
		{
			y--;
			d += 2 * (x - y) + 5;
		}
	}
}


void magicbrush(int x, int y)
{
	int xc = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int yc = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	int dx, dy;

	dx = xc - x;
	dy = yc - y;

	drawPixel(xc + dx, yc + dy);
	drawPixel(xc - dx, yc + dy);
	drawPixel(xc + dx, yc - dy);
	drawPixel(xc - dx, yc - dy);
	drawPixel(xc + dy, yc + dx);
	drawPixel(xc - dy, yc + dx);
	drawPixel(xc + dy, yc - dx);
	drawPixel(xc - dy, yc - dx);
}


void erase(int x, int y)
{
	for (int i = -eraserSize; i <= eraserSize; i++)
	{
		for (int j = -eraserSize; j <= eraserSize; j++)
		{
			drawPixel(x + i, y + j);
		}
	}
}

void keyboard(unsigned char key, int xIn, int yIn)
{
	isSecond = false;
	switch (key)
	{
	case 'q':
	case 27: // 27 is the esc key
		quit();
		break;
	case 'c':
		clear();
		break;
	case '+':
		if (shape == 5 && !isEraser)
		{
			if (brushSize < 16)
				brushSize += 4;

		}
		else if (isEraser)
		{
			if (eraserSize < 10)
				eraserSize += 4;

		}
		break;
	case '-':
		if (shape == 5 && !isEraser)
		{
			if (brushSize > 4)
				brushSize -= 4;

		}
		else if (isEraser)
		{
			if (eraserSize > 2)
				eraserSize -= 4;

		}
		break;
	case 'u':
		undo();
		break;
	case 'r':
		redo();
		break;
	}
}



void mouse(int bin, int state, int x, int y)
{
	if (bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

	    if(x<=1435 && x>=100){
            if(y<=740 && y>=80){
                shoulddraw=true;
            }else{
                shoulddraw=false;
            }
	    }else{
	        shoulddraw=false;
	    }
	    y=801-y;
         if(y<=70 && y>=20){
              if(x<=720 && x>=620){
                if(isgrid){
                    isgrid=false;
                }else{
                isgrid=true;
                }
            }
            else if(x<=790 && x>=750){
                red=1;blue=0;green=0;
            }else if(x<=830 && x>=790){
                red=0;green=1;blue=0;
            }else if(x<=870 && x>=830){
                red=0;green=0;blue=1;
            }else if(x<=910 && x>=870){
                red=1;green=1;blue=0;
            }else if(x<=950 && x>=910){
                red=0;green=1;blue=1;
            }else if(x<=990 && x>=950){
                red=1;green=0;blue=1;
            }else if(x<=1030 && x>=990){
                red=0;green=0;blue=0;
            }
            else if(x<=1070 && x>=1030){
                red=1;green=1;blue=1;
            }

            else if(x<=1100 && x>=1070){
                red=1;green=0.5;blue=0;
            }
            else if(x<=1150 && x>=1110){
                red=0;green=0.5;blue=0;
            }
            else if(x<=1190 && x>=1150){
                red=0.18;green=0.18;blue=0.34;
            }
            else if(x<=1230 && x>=1190){
                red=0.6;green=0.19;blue=0.84;
            }
            else if(x<=1270 && x>=1230){
                red=0.85;green=0.85;blue=0.10;
            }
            else if(x<=1310 && x>=1270){
                red=0.53;green=0.12;blue=0.47;
            }

        }else if(x>=1430 && x<=1520){
            if(y<=730 && y>=680){
                undo();
            }else if(y<=670 && y>=630){
                redo();
            }else if(y<=610 && y>=570){
                clear();
            }else if(y<=550 && y>=510){
                quit();
            }
        }else if(x>=10 && x<=90){
            if(y<=770 && y>=740){
                if(x<=50){
                   pointsiz+=2;
                }else{
                    pointsiz-=2;
                }
            }
            if(y<=730 && y>=680){

                glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
                isEraser=false;
                shape=1;
            }else if(y<=670 && y>=630){
                glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
                isEraser=false;
                shape=2;
            }else if(y<=610 && y>=570){
                isEraser=false;
                shape=4;
                glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            }else if(y<=550 && y>=510){
                isEraser=false;
                shape=3;
                glutSetCursor(GLUT_CURSOR_CROSSHAIR);
            }else if(y<=490 && y>=450){
                if(isRadial){
                    isRadial=false;
                }else{
                    isRadial=true;
                }
            }else if(y<=390 && y>=350){
                if(isEraser){
                    isEraser=false;
                }else{
                eraserSize=12;
                shape=5;
                glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
                isEraser = true;
                }


            }else if(y<=290 && y>=250){
                	shape = 5;
        isEraser = false;
        brushSize = 12;
            glutSetCursor(GLUT_CURSOR_CROSSHAIR);
             }
        }else{
            y=801-y;
            if (isRandom)
		{
			srand(time(NULL));
			red = float(rand()) / float(RAND_MAX);
			green = float(rand()) / float(RAND_MAX);
			blue = float(rand()) / float(RAND_MAX);
		}
		if (isEraser)
		{
			undoHistory.push_back(Pixels.size());
			erase(x, y);
		}
		else
		{
			if (shape == 1)
			{

				undoHistory.push_back(Pixels.size());
				if (isRadial)
					magicbrush(x, y);
				else
					drawPixel(x, y);
			}
			else if (shape == 5)
			{
				undoHistory.push_back(Pixels.size());
				drawBrush(x, y);
			}
			else
			{
				if (!isSecond)
				{
					tmpx = x;
					tmpy = y;
					isSecond = true;
				}
				else
				{
					if (undoHistory.back() != Pixels.size())
						undoHistory.push_back(Pixels.size());
					if (shape == 2)
						drawLine(tmpx, tmpy, x, y);
					else if (shape == 3)
						drawRectangle(tmpx, tmpy, x, y);
					else if (shape == 4)
						drawCircle(tmpx, tmpy, x, y);

					isSecond = false;
				}
			}
		}
		if (undoHistory.size() > 20)
		{
			undoHistory.pop_front();
		}
	}
}

}

void motion(int x, int y)//Actions when mouse is pressed
{
    if(shoulddraw){
      if (isEraser)
		erase(x, y);
	else
	{
		if (shape == 1)
		{
			if (isRadial)
				magicbrush(x, y);
			else
				drawPixel(x, y);
		}
		if (shape == 5)
			drawBrush(x, y);
	}
    }

}

void reshape(int w, int h)
{
	window_w = w;
	window_h = h;
 	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}


void processMainMenu(int value)
{
	switch (value)
	{
	case 0:
		quit();
		break;


	}
}

void processBrushSizeMenu(int value)
{
	shape = 5;
	isEraser = false;
	brushSize = value;
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
}



void mousemenu()
{

	int sizeMenu = glutCreateMenu(processBrushSizeMenu);
	glutAddMenuEntry("4px", 4);
 	glutAddMenuEntry("12px", 12);
	glutAddMenuEntry("16px", 16);

	int main_id = glutCreateMenu(processMainMenu);
	glutAddSubMenu("SPRAY SIZE", sizeMenu);;
	glutAddMenuEntry("Quit", 0);
 	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(void)
{
 	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(red, green, blue);

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, window_w, 0.0, window_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FPS(int val)
{
	glutPostRedisplay();
	glutTimerFunc(0, FPS, 0);
}

void callbackInit()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(17, FPS, 0);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(" BASIC PAINT");
 	callbackInit();
	init();
 	mousemenu();
	glutMainLoop();
	return (0);
}
