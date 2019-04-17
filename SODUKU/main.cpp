#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <utility>

using namespace std;

#define DIM 9
#define NEW_ROW "-------------------------------------"
#define BLANK 0
#define SPACE " "
#define LINE "|"
#define GRID_FULL make_pair(9,9)

int dc = 0; // difficulty number, counts the number of recursions
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
	return (!usedin_row(grid, row, num) && !usedin_col(grid, col, num) && !usedin_box(grid, row - row % 3, col - col % 3, num));
}

// this function searches for unassigned entries in the sodoku. The reference parameter is set to
// this location and true is returned. If every entry is already assigned we return GRID FULL which is a pair greater than anything is the sodoku
pair <int, int > get_unassigned_loc(int grid[DIM][DIM])
{
	for (int row = 0; row < DIM; row++)
	{
		for (int col = 0; col < DIM; col++)
		{
			if (grid[row][col] == BLANK)
			{
				return make_pair(row, col);
			}
		}
	}
	return GRID_FULL;
}

// implementation of the DFS algorithm to find solution to the sodoku "grid" return true if a solution is found
/*
Finds an entry in the grid which is unassgined, and assgins a number to it. Than it goes on unmtil every entry is filled or 
one entry cannot be filled as required by the SODOKU rules, we backtrack and try another number in the last entry. 
*/

/*
EXPLANATION

1) Find an empty  space (if there are no empty spaces we are solved the sodoku -> RETURN TRUE)
2) Try to fit numbers into the place with a loop (PUT IN NUMBER)
	This can fail in two main ways
	A) The number cannot be assigned to the entry because of sodoku rules [ISSAFE in FOR LOOP] -> try the next number in the loop
	B) the number seems good but every variation to fill the sodoku using this number here fails
	

A) is easy to address 
B) is handled by recursion:
	We put the number into the grid than we call this function again. If we do this just as a call it creates an infinite recursion. This is stopped
	by putting the call into an IF statement which is executed if one of the recursions detect a full grid and sends a true. Than we exit with a true.
	RECURSION IS STOPPED WHEN FULL GRID IS DETECTED, THIS IS A SOLUTION

	The normal way of the recursions to stop is that at some point none of numbers can be assigned to an entry because of the rules. This is case A) 
	and this means that an ISSAFE is failed for all numbers in a FOR LOOP in a recursion.  This is where we have to  BACKTRACK.
	This realized in a way, that the SOLVE_SODOKU function returns false when no number can be fitted (ISSAFE failed for all steps in the LOOP)
	This stops the recursions since all possibilities are tried. This happens at the level where the SOLVE_SODOKU just failed was called. The IF STATEMENT is skipped
	and we reset the entry to empty. The for LOOP of the caller SOLVE_SODOKU function than proceeds to try another number. 

	So since a number can be wrong in two ways in an entry, the for loop has two if statements to test it. First we check the rules, and if this was cleared, we check
	the consequences of putting the number in. THe order is important since checking the rules is faster than recursion so we check that first. The B) case is really
	the A) case because the only point when we return false is when all numbers tried by LOOP and FAILED by ISSAFE. So we are searching for a case where the number we put in
	produces a situation where no good substituion exist. 

*/
bool solve_sodoku(int grid[DIM][DIM])
{
	// if the sodoku is filled we are done, thie is the point where we detect completion
	if (get_unassigned_loc(grid) == GRID_FULL)
	{
		return true;
	}

	// count the number of function calls
	dc++;

	// get a new unassgined entry location
	pair<int, int> rnc = get_unassigned_loc(grid);   // rnc stores the row and column of the new entry
	int row = rnc.first;
	int col = rnc.second;

	// we try out numbers between 1-9
	for (int i = 1; i  <= 9; i++)
	{
		// Checking placemenet validity of the current number
		if (issafe(grid, row, col, i))
		{
			// we put the valid number into the grid
			grid[row][col] = i;

			// call the function recursively until a full grid is detected in one of the recursions
			if (solve_sodoku(grid))
			{
				// if the program gets to this point all of the function calls were right so we solved the sodoku  because we only return true when the grid is full

				return true;

			}
			
			// If the progam gets to this point the number is wrong because we already tried to fill in the whole sosoku using this number. WE DELETE IT
			grid[row][col] = BLANK;

		}

		
	}
	return false;

}

// this fucntion reads the soduku from the user line by line
// the function takes the array by reference because we have to change it. THis is like taking it with pointer just safer
void  input_grid(int (&grid)[DIM][DIM])
{
	char curline[256];
	cout << "Enter the sodoku line by line. Treat one line as a 9 digit number, use zeros to represent empty entries. After a line is eneterd press enter." << endl;
	for (int i = 0; i < 9; i++)
	{
		cout << "line  " << i+1 << " : ";
		cin.getline(curline,256);
		
		for (int j = 0; j < 9; j++)
		{
			grid[i][j] = curline[j]-'0';  
			/*
			this forces conversation of char into int because of "-". This converts both char and '0' into their ascii code decimal. 
			Problem is that the number's ascii code are  not themselves, they start at 48 by zero. SO to finish the conversion we have to decreasae the converted int value.
			0..9 are after each other in the ascii so by subtracting 0 ascii code we convert the into numbers correctly.
			Example: char 8 -> 56; 0-> 48; : 56- 48 = 8, is the correct number. 
			a more verbose solution is forced conversion
			((int)curline[j])-48;
			*/
		}
	}
}



int main()
{
	int grid[9][9] = 
	{
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0} ,
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0}
	};
	

	
	input_grid(grid);

	print_grid(grid);

	cout << endl << endl;

	if (true == solve_sodoku(grid))
	{
		// we print the grid if we found a solution
		print_grid(grid);
		cout << endl << endl << "Number of recursions : " << dc;

	}
	else
	{
		cout << "NO SOLUTION";
	}



	string s;
	getline(cin,s);

	return 0;
}