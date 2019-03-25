bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	//see if the coordinates match up
	if (sr == er && sc == ec) {
		return true;
	}

	//mark as visited
	maze[sr][sc] = 'X';
	int maze_r = sr;
	int maze_c = sc;

	if (maze[maze_r][maze_c + 1] == '.') {
		bool checkE = pathExists(maze, nRows, nCols, maze_r, maze_c + 1, er, ec);
		if (checkE) {
			return true;
		}
	}
	if (maze[maze_r][maze_c - 1] == '.') {
		bool checkW = pathExists(maze, nRows, nCols, maze_r, maze_c - 1, er, ec);
		if (checkW) {
			return true;
		}
	}
	if (maze[maze_r + 1][maze_c] == '.') {
		bool checkS = pathExists(maze, nRows, nCols, maze_r + 1, maze_c, er, ec);
		if (checkS) {
			return true;
		}
	}
	if (maze[maze_r - 1][maze_c] == '.') {
		bool checkN = pathExists(maze, nRows, nCols, maze_r - 1, maze_c, er, ec);
		if (checkN) {
			return true;
		}
	}
	return false;
}