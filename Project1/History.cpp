#include "History.h"

#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
	//Initialize variables
	m_col = nCols;
	m_row = nRows;

	//Initialize grid
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			m_grid[i][j] = '.';
		}
	}
}

bool History::record(int r, int c) {
	//Ensure it's within the city
	if (r < 0 || c < 0 || r > MAXROWS || c > MAXCOLS)
	{
		return false;
	}

	//Update grid
	if (m_grid[r - 1][c - 1] == '.') {
		m_grid[r - 1][c - 1] = 'A';
	}
	else if (m_grid[r - 1][c - 1] == 'Z') {
	}
	else {
		m_grid[r - 1][c - 1]++;
	}

	//return true if values are legal
	return true;
}

void History::display() const {
	//Clear the screen
	clearScreen();

	//Print out grid
	for (int i = 0; i < m_row; i++) {
		for (int j = 0; j < m_col; j++) {
			cout << m_grid[i][j];
		}
		cout << endl;
	}

	cout << endl;
}