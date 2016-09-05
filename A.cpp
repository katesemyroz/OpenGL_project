
#include <gl/glut.h>
#include <math.h>
#include <iostream>
using namespace std;


#define DETLIZING 50
#define R 5
#define r 5
#define PI 3.1415
#define STEP_OF_ZOOM 5
#define STEP_OF_ANGLE_LIGHT 1
#define STEP_OF_MOVE 5
#define STEP_OF_ANGLE 5
#define NUM_OR_ROWS_COLUMNS 100
#define KOEF 10 / NUM_OR_ROWS_COLUMNS 


int sign(double D)
{
	if (D == 0)
	{
		return 0;
	}
	else if (D > 0)
	{
		return 1;
	}
	return -1;
}
class Point
{
public:
	double x, y, z;
	Point()
	{
		x = y = z = 1;
	}
	Point(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Point* sum(Point* P)
	{
		return new Point(this->x + P->x, this->y + P->y, this->z + P->z);
	}

	Point* minus(Point* P)
	{
		return new Point(this->x - P->x, this->y - P->y, this->z - P->z);
	}
};

Point * LOOK_POINT = new Point(0, 0, 0);
Point * LOOK_AT_POINT = new Point(0, 0, 0);
Point * UP_VECTOR = new Point(0, 1, 0);
double RADIUS_OF_LOOK_POINT = 100;
int THETA = 0;
int PHI = 0;
double ratio = 0;

int THETA_LIGHT = 0;
int PHI_LIGHT = 0;
int RADIUS_LIGHT = 1000000;
int delay = 50;


GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[] = { 0.0, 0.0, 0.0, 2.0 };
GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightAmbient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightShines[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightSpotDir[] = { -1.0, -1.0, -1.0 };



double cords[3 * NUM_OR_ROWS_COLUMNS * NUM_OR_ROWS_COLUMNS];
int indexes[4 * (NUM_OR_ROWS_COLUMNS - 1) * (NUM_OR_ROWS_COLUMNS - 1)];
Point * MaxColor = new Point(10, 10, 0);
Point * MinColor = new Point(0, 0, 0);
Point * DeltaColorPoint = new Point(0, 0, 0);



void drawSquare(int numOfSquads)
{
	int itter = 0;
	DeltaColorPoint->x = MaxColor->x - MinColor->x;
	DeltaColorPoint->y = MaxColor->y - MinColor->y;
	DeltaColorPoint->z = MaxColor->z - MinColor->z;
	for (int i = 0; i < numOfSquads; i++)
	{

		glBegin(GL_QUADS);
		for (int j = 0; j < 4; j++)
		{
			glColor3f((cords[3 * indexes[itter]] - MinColor->x) / DeltaColorPoint->x, (cords[3 * indexes[itter] + 1] - MinColor->y) / DeltaColorPoint->y, (cords[3 * indexes[itter] + 2] - MinColor->z) / DeltaColorPoint->z);
			glVertex3d(cords[3 * indexes[itter]], cords[3 * indexes[itter] + 1], cords[3 * indexes[itter] + 2]);
			itter++;
		}
		glEnd();
	}
}


void changeSize(int w, int h) {
	if (h == 0)
		h = 1;
	ratio = w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, 1, -1);
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, 1.0f / ratio, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightSpotDir);
	glMaterialfv(GL_FRONT, GL_SPECULAR, lightShines);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0);


	LOOK_POINT->x = RADIUS_OF_LOOK_POINT * cos(PHI / 180.0 * PI) * sin(THETA / 180.0 * PI) + LOOK_AT_POINT->x;
	LOOK_POINT->y = RADIUS_OF_LOOK_POINT * sin(PHI / 180.0 * PI) * sin(THETA / 180.0 * PI) + LOOK_AT_POINT->y;
	LOOK_POINT->z = RADIUS_OF_LOOK_POINT * cos(THETA / 180.0 * PI) + LOOK_AT_POINT->z;
	gluLookAt(LOOK_POINT->x, LOOK_POINT->y, LOOK_POINT->z,
		LOOK_AT_POINT->x, LOOK_AT_POINT->y, LOOK_AT_POINT->z,
		0.0f, 1.0f, 0.0f);


	glColor3f(1, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTranslated(0, 30, 0);
	glutSolidTorus(5, 10, 100, 100);

	glColor3f(0, 1, 0);
	glTranslated(20, -30, 0);
	glutSolidCone(5, 10, 100, 100);

	glTranslated(-20, 0, 50);
	drawSquare((NUM_OR_ROWS_COLUMNS - 1) * (NUM_OR_ROWS_COLUMNS - 1));



	glutSwapBuffers();
}

void move(unsigned char key, int w, int h)
{
	switch (key)
	{
	case 49/*to reduce distance between camera and object - press 1*/:
		if (RADIUS_OF_LOOK_POINT != 0)
		{
			RADIUS_OF_LOOK_POINT = RADIUS_OF_LOOK_POINT - STEP_OF_ZOOM;
		}
		break;
	case 50/*to increase distance between camera and object - press 2*/:
		RADIUS_OF_LOOK_POINT = RADIUS_OF_LOOK_POINT + STEP_OF_ZOOM;
		break;
	case 51/*to move camera around the objects (x-axis) - press 3*/:
		THETA = (THETA - STEP_OF_ANGLE) % 360;
		break;
	case 52/*to move camera around the objects (x-axis) - press 4*/:
		PHI = (PHI - STEP_OF_ANGLE) % 360;
		break;
	case 53/*to move camera around the objects (y-axis) - press 5*/:
		PHI = (PHI + STEP_OF_ANGLE) % 360;
		break;
	case 54/*to move camera around the objects (y-axis)  - press 6*/:
		THETA = (THETA + STEP_OF_ANGLE) % 360;
		break;
	case 113/*to move camera on the right  - press q*/:
		LOOK_AT_POINT->x += STEP_OF_MOVE;
		break;
	case 119/*to move camera on the left  - press w*/:
		LOOK_AT_POINT->x -= STEP_OF_MOVE;
		break;
	case 97/*to move camera up  - press a*/:
		LOOK_AT_POINT->y += STEP_OF_MOVE;
		break;
	case 115/*to move camera down  - press s*/:
		LOOK_AT_POINT->y -= STEP_OF_MOVE;
		break;
	case 122/*to move camera forward - press z*/:
		LOOK_AT_POINT->z += STEP_OF_MOVE;
		break;
	case 120/*to move camera backward - press x*/:
		LOOK_AT_POINT->z -= STEP_OF_MOVE;
		break;
	default:
		break;
	}	
	glutPostRedisplay();
}


void timerFunc(int n)
{
	THETA_LIGHT = (THETA_LIGHT + n) % 360;
	lightPosition[0] = RADIUS_LIGHT * cos(THETA_LIGHT / 180.0 * PI) * cos(PHI_LIGHT / 180.0 * PI);
	lightPosition[1] = RADIUS_LIGHT * sin(THETA_LIGHT / 180.0 * PI) * cos(PHI_LIGHT / 180.0 * PI);
	lightPosition[3] = RADIUS_LIGHT * sin(long double(PHI_LIGHT));
	glutPostRedisplay();
	glutTimerFunc(delay, timerFunc, STEP_OF_ANGLE_LIGHT);
}


int main(int argc, char **argv) 
{

	cout << "to reduce distance between camera and object - press 1\n";
	cout << "to increase distance between camera and object - press 2\n";
	cout << "to move camera around the objects (x-axis) - press 3\n";
	cout << "to move camera around the objects (x-axis) - press 4\n";
	cout << "to move camera around the objects (y-axis) - press 5\n";
	cout << "to move camera around the objects (y-axis)  - press 6\n";
	cout << "to move camera on the right  - press q\n";
	cout << "to move camera on the left  - press w\n";
	cout << "to move camera up  - press a\n";
	cout << "to move camera down  - press s\n";
	cout << "to move camera forward - press z\n";
	cout << "to move camera backward - press x\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glEnable(GL_NORMALIZE);
	glutCreateWindow("");

	glEnable(GLUT_MULTISAMPLE);
	// регистрация
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(move);
	glutTimerFunc(delay, timerFunc, STEP_OF_ANGLE_LIGHT);

	int coords = 0;
	for (int i = 0; i < NUM_OR_ROWS_COLUMNS - 1; i++)
	{
		for (int j = 0; j < NUM_OR_ROWS_COLUMNS - 1; j++)
		{
			indexes[coords] = i * NUM_OR_ROWS_COLUMNS + j;
			++coords;
			indexes[coords] = i * NUM_OR_ROWS_COLUMNS + j + 1;
			++coords;
			indexes[coords] = (i + 1) * NUM_OR_ROWS_COLUMNS + j + 1;
			++coords;
			indexes[coords] = (i + 1) * NUM_OR_ROWS_COLUMNS + j;
			++coords;

		}
	}
	coords = 0;
	for (int i = 0; i < NUM_OR_ROWS_COLUMNS; i++)
	{
		for (int j = 0; j < NUM_OR_ROWS_COLUMNS; j++)
		{
			cords[coords] = (double)(i)* KOEF;
			++coords;
			cords[coords] = (double)(j)* KOEF;
			++coords;
			cords[coords] = sin((double)(i)* KOEF)*sqrt((double)(j)* KOEF);
			if (cords[coords] < MinColor->z)
			{
				MinColor->z = cords[coords];
			}
			if (cords[coords] > MaxColor->z)
			{
				MaxColor->z = cords[coords];
			}
			++coords;
		}
	}

	// основной цикл
	glutMainLoop();

	return 1;
}
