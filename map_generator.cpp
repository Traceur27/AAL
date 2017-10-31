#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char** argv)
{
	ofstream output_file;
	int rows = 0;
	int columns = 0;
	int height = 0;
	char *mode;

	if (argc != 5)
	{
		cout << "Usage: ./Flood <number_of_rows> <number_of_columns> <height> <mode>(r - random, n - pessimistic case)" << endl;
		exit(0);
	}

	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	height = atoi(argv[3]);
	mode = argv[4];

	if (*mode != 'n' && *mode != 'r')
	{
		cout << "Usage: ./Flood <number_of_rows> <number_of_columns> <height> <mode>(r - random, n - pessimistic case)" << endl;
		exit(0);
	}

	output_file.open("data.txt");

	if (*mode == 'n')
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				if (i == 0 || i == rows - 1) //first and last rows - all have hight value
					output_file << height;
				else if (j == 0 || j == columns - 1) //first and last rows - all have hight value
					output_file << height;
				else
					output_file << 0;

				if (j == columns - 1)
					output_file << endl;
				else
					output_file << " ";
			}
		}
	}
	else
	{
		//generate radom
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
				output_file << rand() % 10 << " ";
			output_file << endl;
		}
	}

	output_file.close();
	return 0;
}
