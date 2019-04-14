#include <iostream>
#include <string>

using namespace std;

#define DIM 9
#define NEW_ROW "-------------------------------------"
#define BLANK 0
#define SPACE " "
#define LINE "|"


// Print Soduku grid
void print_grid(int grid[DIM][DIM])
{
	for (int i = 0; i < DIM; i++)
	{
		cout << SPACE << SPACE;
		cout << NEW_ROW << endl;
		cout << SPACE << SPACE << LINE;
		for (int j = 0; j < DIM; j++)
		{
			cout << SPACE;
			if (BLANK == grid[i][j])
			{
				cout << SPACE;
			}
			else
			{
				cout << grid[i][j];
			}
			cout << SPACE;
			cout << LINE;
		}
		cout << endl;

	}
	cout << SPACE << SPACE << NEW_ROW;
	}

// Checks that whether or not  number "num" is used as an entry in row "row" in sodoku "grid"
// returns true when  the number is used
bool usedin_row(int grid[DIM][DIM], int row, int num)
{
	for (int i = 0; i < DIM; i++)
	{
		if (grid[row][i] == num)
		{
			return true;
		}
	}
	return false;
}

// Checks that whether or not  number "num" is used as an entry in column "col" in sodoku "grid"
// returns true when  the number is used
bool usedin_col(int grid[DIM][DIM], int col, int num)
{
	for (int i = 0; i < DIM; i++)
	{
		if (grid[i][col] == num)
		{
			return true;
		}
	}
	return false;
}

// Checks that whether or not  number "num" is used as an entry in a 3x3 box which upper left corener coordinates are "str_row" "str_col" in sodoku "grid"
// returns true when  the number is used
bool usedin_box(int grid[DIM][DIM], int str_row, int str_col, int num)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (grid[str_row + i][str_col + j] == num)
			{
				return true;
			}
		}
	}
	return false;
}

// Checks that wheter or not is valid to place number "num" into the slot "row" "col"
bool issafe(int grid[DIM][DIM], int row, int col, int num)
{
	// only safe to place if the number is not already placed: means the cheking functions all return false
	/*
	when the box is checked there is a problem: the 3x3 square's upper left corner is given and that square can be anywhere
	in the game there boxes are given. if (0,4) is given, that number is in the (0,3) box.  We can get the boxe's "address" 
	if we subtract from numbers coordinates their coordinates respect to the small box. This can be 0,1,2. THe next box starts after 
	the first which is 3 row/column distance the coordinates respect to the small boxes thus repeat: in a row: 0,1,2,0,1,2,0,1,2. This is exactly the 
	modulo 3 of their original big grid coordinates. 
	THis in this example: (0,4) is in the ( 0 - (0 mod 3) , 4 - (4 mod 3) )
	*/
	return (usedin_row(grid, row, num) && usedin_col(grid, col, num) && usedin_box(grid, row - row % 3, col - col % 3, num));
}

int main()
{
	int grid[9][9] = 
	{
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,0,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,0,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,5,6,7,8,9}
	};
	
	print_grid(grid);

	cout << endl <<  issafe(grid, 2,3, 11);

	string s;
	getline(cin,s);

	return 0;
}