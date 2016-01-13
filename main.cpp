#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define filename "data.txt"

using namespace std;


int capacity = 0;
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
GLdouble left1 = 0;//-21.0;
GLdouble right1 = 0;// 21.0;
GLdouble bottom = 0;// -21.0;
GLdouble top = 0;// 21.0;
GLdouble near1 = 1; //1.0;
GLdouble far1 = 0;// 41.0;
GLdouble x = 0;
GLdouble y = 0;
GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat scale = 1.0;
vector<int> * fieldsToDraw;



void drawFields()
{
	//move to the left upper corner
	int i = 0;
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(-x / 2 + 0.5, y / 2 - 0.5, 0.5);

	for (vector<int>::iterator it = fieldsToDraw->begin(); it != fieldsToDraw->end(); ++it, ++i)
	{
		glPushMatrix();
		for (int h = 0; h < *it; ++h)
		{
			if(h % 2)
				glColor3f(0, 0, 1);
			else
				glColor3f(0, 1, 0);

			glutSolidCube(1);
			glTranslatef(0, 0, 1);
		}
		glPopMatrix();

		if (i == x - 1)
		{
			glTranslatef(-x + 1, -1, 0);
			i = -1;
		}
		else
			glTranslatef(1, 0, 0);
	}


	glPopMatrix();
}


void Display()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	//Clear the color buffer and z buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW); //- potrzebne przy rysowaniu primitywuw
	glLoadIdentity();
	// przesuniêcie uk³adu wspó³rzêdnych obiektu do œrodka bry³y odcinania

	glTranslatef(0, 0, -11);//-y/2 - 1);

	// obroty obiektu - klawisze kursora
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);


	glColor3f(1.0, 0.0, 0.0);

	//floor
	glBegin(GL_QUADS);
	glVertex3f(x/2, -y/2, 0);
	glVertex3f(x/2, y/2, 0);
	glVertex3f(-x/2, y/2, 0);
	glVertex3f(-x/2, -y/2, 0);
	glEnd();

	drawFields();

	glFlush();
	glutSwapBuffers();
}


void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();


	if (width < height && width > 0)
		glFrustum(left1, right1, bottom * height / width, top * height / width, near1, far1);
	else
		// szerokoœæ okna wiêksza lub równa wysokoœci okna
		if (width >= height && height > 0)
			glOrtho(left1 * width / height, right1 * width / height, bottom, top, 1, 21);

	//glMatrixMode(GL_MODELVIEW);
	Display();
}


void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.1;
	else if (key == '-' && scale > 0.1)
		scale -= 0.1;

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


void initGL(int width, int height)
{
	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	GLdouble clip = 0;

	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	//Draw the window in the middle of the screen
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 600) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - 600) / 2);
	glutInitWindowSize(600, 600);

	glutCreateWindow("Water flooding");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);

	// do³¹czenie funkcji obs³ugi klawiatury
	glutKeyboardFunc(Keyboard);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
	glEnable(GL_COLOR_MATERIAL);

	
	clip = height > width ? height : width;
	//Set the cutting planes
	//far1 = clip + 1;
	top = clip / 2 + 1;
	bottom = -clip / 2 - 1;
	left1 = -clip / 2 - 1;
	right1 = clip / 2 + 1;
	x = width;
	y = height;

	glutMainLoop();
}


void countRows(int * w, ifstream * in)
{
	char c = (*in).get();
	while (c != '\n')
	{
		if (c != ' ')
			(*w)++; //counting characters in row
		c = (*in).get();
	}

	(*in).seekg(0, ios::beg);
}

void getDataFromFile(ifstream * in, vector<int> * tab)
{
	char c = (*in).get();

	while (c != EOF)
	{
		if (c != ' ' && c != '\n')
			tab->push_back(c - '0'); //converting chars to int
		c = (*in).get();
	}

}

void markEstruaryFields(vector<int> * tab, vector<int> * eFields, int * w, int * h)
{
	for (int i = 0; i < *h; ++i)
	{
		for (int j = 0; j < *w; ++j)
		{
			if (i == 0 || i == *h - 1) //first and last row
			{
				eFields->push_back(i*(*w) + j); //saving number of fields
			}

			else if (j == 0 || j == *w - 1) //first and lasc column
			{
				eFields->push_back(i*(*w) + j);
			}
		}
	}
}


bool isFieldInside(vector<int> * tab, int x, int y, int * w, int * h)
{
	if (x < 0 || x >= *w || y < 0 || y >= *h)
		return false;

	int number = y*(*w) + x;

	for (vector<int>::iterator it = tab->begin(); it != tab->end(); ++it)
	{
		if (number == *it)
			return true;
	}
	return false;
}


bool tryFlood(vector<int> * table, vector<int> * estruaryFields, vector<int> * checkedFields, int x, int y, int * w, int * h, int fieldHeight = -1)
{
	if (isFieldInside(estruaryFields, x, y, w, h))
		return false;

	
	int fieldIndex= y * (*w) + x;
	if (fieldHeight == -1)
		fieldHeight = (*table)[fieldIndex];

	int upFieldIndex = fieldIndex - *w;
	int downFieldIndex = fieldIndex + *w;
	int rightFieldIndex = fieldIndex + 1;
	int leftFieldIndex = fieldIndex - 1;

	//all fields around are higher
	if (fieldHeight < (*table)[upFieldIndex] && fieldHeight < (*table)[downFieldIndex] &&
		fieldHeight < (*table)[leftFieldIndex] && fieldHeight < (*table)[rightFieldIndex])
		return true;

	if ((*table)[leftFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x - 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x - 1, y, w, h, fieldHeight))
			return false;
	}

	if ((*table)[rightFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x + 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x + 1, y, w, h, fieldHeight))
			return false;
	}

	if ((*table)[upFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y - 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x, y - 1, w, h, fieldHeight))
			return false;
	}

	if ((*table)[downFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y + 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x, y + 1, w, h, fieldHeight))
			return false;
	}

	return true;
}


void flood(vector<int> * table, vector<int> * estruaryFields, vector<int> * checkedFields, int * width, int * height)
{
	//for all fields
	for (int i = 0; i < *height; ++i)
	{
		for (int j = 0; j < *width; ++j)
		{
			while (tryFlood(table, estruaryFields, checkedFields, j, i, width, height))
			{
				(*table)[i * (*width) + j] += 1; //increase field's height
				capacity += 1;
			}
			estruaryFields->push_back(i*(*width) + j);
			checkedFields->clear();
		}
	}
}


int main(int argc, char** argv)
{
	//get data from file, create table and start flood procedure
	//then present results
	ifstream input(filename, ios::in);
	vector<int> * table = new vector<int>();
	vector<int> * estruaryFields = new vector<int>();
	vector<int> * checkedFields = new vector<int>();
	fieldsToDraw = new vector<int>();
	int width = 0;
	int height = 0;
	clock_t t = clock();


	countRows(&width, &input);
	getDataFromFile(&input, table);
	*fieldsToDraw = *table;
	height = table->size() / width;

	markEstruaryFields(table, estruaryFields, &width, &height);

	flood(table, estruaryFields, checkedFields, &width, &height);
	t = clock() - t;

	cout << capacity << endl;
	cout << "Calculation took " << (float)t / CLOCKS_PER_SEC << " seconds" << endl;

	initGL(width, height);
	
	/*cout << isFieldInside(estruaryFields, 8, 3, &width, &height) << " ";
	for (vector<int>::iterator it = estruaryFields->begin(); it != estruaryFields->end(); ++it)
	{
		cout << (*it) << " ";
	}

	cout << "\n" << estruaryFields->size() << endl;

	cout << "Szerokosc: " << width << ", wysokosc: " << height << endl;
	
	for (vector<int>::iterator it = table->begin(); it != table->end(); ++it)
	{
		cout << (*it) << endl;
	}
	*/
	cin.get();

	return 0;
}
