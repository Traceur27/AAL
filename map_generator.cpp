#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;


int main(int argc, char** argv)
{
	ofstream myfile;
	int rows = 0;
	int columns = 0;
	int height = 0;
	char *mode;

	if (argc != 5)
	{
		cout << "Sposob uzycia: generator rzedy kolumny wysokosc tryb(r - losowy, n - przypadek pesymistyczny" << endl;
		exit(0);
	}

	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	height = atoi(argv[3]);
	mode = argv[4];

	if (*mode != 'n' && *mode != 'r')
	{
		cout << "Sposob uzycia: generator rzedy kolumny wysokosc" << endl;
		exit(0);
	}

	myfile.open("data.txt");

	if (*mode == 'n')
	{
		////fill
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				if (i == 0 || i == rows - 1) //first and last rows - all have hight value
				{
					myfile << height;
				}
				else if (j == 0 || j == columns - 1) //first and last rows - all have hight value
				{
					myfile << height;
				}
				else
				{
					myfile << 0;
				}


				if (j == columns - 1)
				{
					myfile << endl;
				}
				else
					myfile << " ";
			}
		}
	}
	else
	{
		//generate radom
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				myfile << rand() % 10 << " ";
			}
			myfile << endl;
		}
	}

	myfile.close();


	return 0;
}