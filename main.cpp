#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>

#define filename "data.txt"

using namespace std;


int capacity = 0;
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
GLdouble left1 = 0;
GLdouble right1 = 0;
GLdouble bottom = 0;
GLdouble top = 0;
GLdouble near1 = 1;
GLdouble far1 = 0;
GLdouble x = 0;
GLdouble y = 0;
GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat scale = 1.0;
vector<int> * fieldsToDraw;
vector<int> * fieldsAfterFlood;
bool beforeFlood = true;
char * mode;



void drawFields()
{
	//move to the left upper corner
	int i = 0;
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(-x / 2 + 0.5, y / 2 - 0.5, 0.5);

	vector<int>::iterator it, end;
	if (beforeFlood)
	{
		it = fieldsToDraw->begin();
		end = fieldsToDraw->end();
	}
	else
	{
		it = fieldsAfterFlood->begin();
		end = fieldsAfterFlood->end();
	}

	for (; it != end; ++it, ++i)
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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//moving coordinate system to the middle of z axis
	glTranslatef(0, 0, -11);

	// object rotation
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// object scaling
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
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	if (width < height && width > 0)
		glOrtho(left1, right1, bottom * height / width, top * height / width, near1, far1);
	else
		if (width >= height && height > 0)
			glOrtho(left1 * width / height, right1 * width / height, bottom, top, 1, 21);

	Display();
}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == '+')
		scale += 0.1;
	else if (key == '-' && scale > 0.1)
		scale -= 0.1;
	else if (key == 'q')
		beforeFlood = !beforeFlood;

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

	glutKeyboardFunc(Keyboard);

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

void markEstruaryFieldsNaive(vector<int> * tab, vector<int> * eFields, int * w, int * h)
{
	for (int i = 0; i < *h; ++i)
	{
		for (int j = 0; j < *w; ++j)
		{
			if (i == 0 || i == *h - 1) //first and last row
			{
				eFields->push_back(i*(*w) + j); //saving number of fields
			}

			else if (j == 0 || j == *w - 1) //first and last column
			{
				eFields->push_back(i*(*w) + j);
			}
		}
	}
}


void markEstruaryFields(vector<int> * tab, vector<int> * eFields, vector<int> * vFields, int * w, int * h)
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
			else
			{
				vFields->push_back(i*(*w) + j);
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


bool tryFloodNaive(vector<int> * table, vector<int> * estruaryFields, vector<int> * checkedFields, int x, int y, int * w, int * h, int fieldHeight = -1)
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

	//left field's height is lower - mark this field as checked and go left
	if ((*table)[leftFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x - 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFloodNaive(table, estruaryFields, checkedFields, x - 1, y, w, h, fieldHeight))
			return false;
	}

	//right field's height is lower - mark this field as checked and go right
	if ((*table)[rightFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x + 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFloodNaive(table, estruaryFields, checkedFields, x + 1, y, w, h, fieldHeight))
			return false;
	}


	//up field's height is lower - mark this field as checked and go up
	if ((*table)[upFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y - 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFloodNaive(table, estruaryFields, checkedFields, x, y - 1, w, h, fieldHeight))
			return false;
	}

	//down field's height is lower - mark this field as checked and go down
	if ((*table)[downFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y + 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFloodNaive(table, estruaryFields, checkedFields, x, y + 1, w, h, fieldHeight))
			return false;
	}
	return true;
}



bool tryFlood(vector<int> * table, vector<int> * estruaryFields, vector<int> * checkedFields, int x, int y, int * w, int * h)
{
	if (isFieldInside(estruaryFields, x, y, w, h))
		return false;


	int fieldIndex = y * (*w) + x;
	int	fieldHeight = (*table)[fieldIndex];

	int upFieldIndex = fieldIndex - *w;
	int downFieldIndex = fieldIndex + *w;
	int rightFieldIndex = fieldIndex + 1;
	int leftFieldIndex = fieldIndex - 1;

	//all fields around are higher
	if (fieldHeight < (*table)[upFieldIndex] && fieldHeight < (*table)[downFieldIndex] &&
		fieldHeight < (*table)[leftFieldIndex] && fieldHeight < (*table)[rightFieldIndex])
		return true;

	//left field's height is lower - mark this field as checked and go left
	if ((*table)[leftFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x - 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x - 1, y, w, h))
			return false;
	}

	//right field's height is lower - mark this field as checked and go right
	if ((*table)[rightFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x + 1, y, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x + 1, y, w, h))
			return false;
	}

	//up field's height is lower - mark this field as checked and go up
	if ((*table)[upFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y - 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x, y - 1, w, h))
			return false;
	}

	//down field's height is lower - mark this field as checked and go down
	if ((*table)[downFieldIndex] <= fieldHeight && !isFieldInside(checkedFields, x, y + 1, w, h))
	{
		checkedFields->push_back(fieldIndex);
		if (!tryFlood(table, estruaryFields, checkedFields, x, y + 1, w, h))
			return false;
	}

	return true;
}


void floodNaive(vector<int> * table, vector<int> * estruaryFields, vector<int> * checkedFields, int * width, int * height)
{
	//for all fields
	for (int i = 0; i < *height; ++i)
	{
		for (int j = 0; j < *width; ++j)
		{
			while (tryFloodNaive(table, estruaryFields, checkedFields, j, i, width, height))
			{
				(*table)[i * (*width) + j] += 1; //increase field's height
				capacity += 1;
				checkedFields->clear();
			}
			estruaryFields->push_back(i*(*width) + j);
			checkedFields->clear();
		}
	}
}


void flood(vector<int> * table, vector<int> * estruaryFields, vector<int> * fieldsToVisit, vector<int> * checkedFields, int * width, int * height)
{
	int i = 0;
	int j = 0;
	int h = 0;

	//for all fields left
	while (fieldsToVisit->size() != 0)
	{
		vector<int>::iterator it = fieldsToVisit->begin();
		while (it != fieldsToVisit->end())
		{
			i = *it / (*width);
			j = *it - i*(*width);
			if ((*table)[i * (*width) + j] == h)
			{
				if (tryFlood(table, estruaryFields, checkedFields, j, i, width, height))
				{
					(*table)[i * (*width) + j] += 1; //increase field's height
					capacity += 1;
					++it;
				}
				else
				{
					estruaryFields->push_back(i*(*width) + j);
					it = fieldsToVisit->erase(it);
				}
				checkedFields->clear();
			}
			else
				++it;
		}
		++h;
	}
}


int main(int argc, char** argv)
{
	ifstream input;//(filename, ios::in);
	vector<int> * table = new vector<int>();
	vector<int> * estruaryFields = new vector<int>();
	vector<int> * fieldsToVisit = new vector<int>();
	vector<int> * checkedFields = new vector<int>();
	fieldsToDraw = new vector<int>();
	fieldsAfterFlood = new vector<int>();
	int width = 0;
	int height = 0;
	mode = new char();
	char *name = new char();

	if (argc != 3)
	{
		cout << "Uzycie programu: WaterFlooding dane.txt tryb(n lub u)" << endl;
		exit(1);
	}

	name = argv[1];
	mode = argv[2];

	input.open(name, ios::in);
	
	if (*mode != 'u' && *mode != 'n')
	{
		*mode = 'u';
	}


	countRows(&width, &input);
	getDataFromFile(&input, table);
	*fieldsToDraw = *table;
	height = table->size() / width;

	clock_t t = clock();

	if (*mode == 'n')
	{
		markEstruaryFieldsNaive(table, estruaryFields, &width, &height);
		floodNaive(table, estruaryFields, checkedFields, &width, &height);
	}
	else if (*mode == 'u')
	{
		markEstruaryFields(table, estruaryFields, fieldsToVisit, &width, &height);
		flood(table, estruaryFields, fieldsToVisit, checkedFields, &width, &height);
	}

	t = clock() - t;

	*fieldsAfterFlood = *table;
	cout << "Computed capacity: " << capacity << endl;
	cout << "Calculation took " << (float)t / CLOCKS_PER_SEC << " seconds" << endl;
	//cout << "q = " << ((float)t / CLOCKS_PER_SEC * 32768) / (1.141f * pow(width, 5)) << endl;

	initGL(width, height);
	
	cin.get();

	return 0;
}
