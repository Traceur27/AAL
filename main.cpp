#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <iostream>
#include <fstream>
#include <vector>

#define filename "data1.txt"

using namespace std;


int capacity = 0;


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
	int width = 0;
	int height = 0;

	countRows(&width, &input);
	getDataFromFile(&input, table);
	height = table->size() / width;

	markEstruaryFields(table, estruaryFields, &width, &height);

	flood(table, estruaryFields, checkedFields, &width, &height);

	cout << capacity << endl;
	
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
