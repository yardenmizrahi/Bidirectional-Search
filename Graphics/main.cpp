#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <queue>
#include <vector>
#include "Cell.h"
#include <iostream>

using namespace std;

const int MSZ = 100;

const int SPACE = 0;
const int WALL = 1;
const int START = 2;
const int TARGET = 3;
const int GRAY = 4;
const int BLACK = 5;
const int PATH = 6;
const int DARK_GRAY = 7;


int maze[MSZ][MSZ] = {0}; // space
bool runBFS = false, runDFS = false;

queue <Cell*> grays; // queue of gray cells
queue <Cell*> reverse_dark_grays; // stack of gray cells



void InitMaze();

void init()
{
	glClearColor(0.5,0.5,0.5,0);// color of window background
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // set the coordinates system

	srand(time(0));

	InitMaze();
}

void InitMaze()
{
	int i, j;

	for (i = 0;i < MSZ;i++)
	{
		maze[0][i] = WALL;
		maze[MSZ-1][i] = WALL;
		maze[i][0] = WALL;
		maze[i][MSZ - 1] = WALL;
	}

	for(i=1;i<MSZ-1;i++)
		for (j = 1;j < MSZ - 1;j++)
		{
			if (i % 2 == 1) // mostly spaces
			{
				if (rand() % 10 < 2) // WALL
					maze[i][j] = WALL;
				else maze[i][j] = SPACE;
			}
			else // even lines
			{
				if (rand() % 10 < 4) // space
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
		}
	maze[MSZ / 2][MSZ / 2] = START;
	int rand_num = rand() % MSZ;
	maze[rand_num][rand_num] = TARGET;

	Cell* pc = new Cell(MSZ / 2, MSZ / 2,nullptr);  // pointer to a start cell
	Cell* pc_rev = new Cell(rand_num, rand_num, nullptr);  // pointer to a target cell
	grays.push(pc); // insert first cell to grays
	reverse_dark_grays.push(pc_rev); // insert first cell to reverse grays

}

void DrawMaze()
{
	int i, j;

	for(i=0;i<MSZ;i++)
		for (j = 0;j < MSZ;j++)
		{
			switch (maze[i][j]) // set convinient color of a cell
			{
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case WALL:
				glColor3d(0,0, 0); // black
				break;
			case START:
				glColor3d(0.5, 0.5, 1); // blue
				break;
			case TARGET:
				glColor3d(1, 0, 0); // red
				break;
			case GRAY:
				glColor3d(1, 0.7, 0); // orange
				break;
			case BLACK:
				glColor3d(0.8, 1, 0.8); // green
				break;
			case PATH:
				glColor3d(1, 0, 1); // magenta
				break;
			case DARK_GRAY:
				glColor3d(1, 0.8, 0); // dark grey
				break;

			}// switch

			glBegin(GL_POLYGON);
			glVertex2d(j, i);
			glVertex2d(j, i + 1);
			glVertex2d(j+1, i + 1);
			glVertex2d(j+1 , i );
			glEnd();
		}
}

Cell* findMatchingPointer(queue <Cell*> x, int row, int col) {
	// Search in queue x
	while (!x.empty()) {
		Cell* currentPointer = x.front();
		x.pop();
		if (currentPointer->getRow() == row && currentPointer->getCol() == col) {
			return currentPointer;
		}
	}
	// Pointer not found in either queue
	return nullptr;
}


void RestorePath(Cell* pc)
{
	while (pc != nullptr)
	{
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();
	}

}


// BFS a cell on row,col can be either WHITE or DARK GRAY
bool CheckNeighbour(int row, int col, Cell* pCurrent)
{
	Cell* pn = nullptr; 
	if (maze[row][col] == DARK_GRAY)
	{
		runBFS = false;
		RestorePath(pCurrent);
		Cell* rev_pc = findMatchingPointer(reverse_dark_grays, row, col);
		RestorePath(rev_pc);
		return false;
	}
	else // must be "White Neighbour" - SPACE
	{
		pn = new Cell(row, col, pCurrent); // create new Cell
		grays.push(pn);
		maze[row][col] = GRAY;
		return true; // go_on = true
	}
}

// BFS a cell on row,col can be either WHITE or GRAY
bool CheckNeighbourReverse(int row, int col, Cell* pCurrent)
{
	Cell* pn = nullptr;
	if (maze[row][col] == GRAY)
	{
		runBFS = false;
		RestorePath(pCurrent);
		Cell* rev_pc = findMatchingPointer(grays, row, col);
		RestorePath(rev_pc);
		return false;
	}
	else // must be "White Neighbour" - SPACE
	{
		pn = new Cell(row, col, pCurrent); // create new Cell
		reverse_dark_grays.push(pn);
		maze[row][col] = DARK_GRAY;
		return true; // go_on = true
	}
}

void RunBiBFSIteration()
{
	Cell* pc;
	int row, col;
	bool go_on = true;

	if (grays.empty())
	{
		runBFS = false;
		cout << "Grays are empty. There is no solution\n";
		return;
	}
	else // grays is not empty
	{
		// pick the first Cell from grays
		pc = grays.front();
		grays.pop();
		// paint it black
		if(maze[pc->getRow()][pc->getCol()]!= START) maze[pc->getRow()][pc->getCol()] = BLACK;

		row = pc->getRow();
		col = pc->getCol();
		// now check all neighbors of pc and if they are white add them to grays
		// but if one of them is target then stop BFS because target has been found
		
		// up
		if (maze[row + 1][col] == SPACE || maze[row + 1][col] == DARK_GRAY) 
			go_on = CheckNeighbour(row + 1, col, pc);
		// down
		if (go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == DARK_GRAY))
			go_on = CheckNeighbour(row - 1, col, pc);
		// left
		if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == DARK_GRAY))
			go_on = CheckNeighbour(row, col - 1, pc);
		// right
		if (go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == DARK_GRAY))
			go_on = CheckNeighbour(row, col + 1, pc);

	}
}

void RunBiBFSIterationReversed()
{
	Cell* pc;
	int row, col;
	bool go_on = true;

	if (reverse_dark_grays.empty())
	{
		runBFS = false;
		cout << "Grays are empty. There is no solution\n";
		return;
	}
	else // grays is not empty
	{
		// pick the first Cell from grays
		pc = reverse_dark_grays.front();
		reverse_dark_grays.pop();
		// paint it black
		if (maze[pc->getRow()][pc->getCol()] != TARGET) maze[pc->getRow()][pc->getCol()] = BLACK;

		row = pc->getRow();
		col = pc->getCol();
		// now check all neighbors of pc and if they are white add them to grays
		// but if one of them is target then stop BFS because target has been found

		// up
		if (maze[row + 1][col] == SPACE || maze[row + 1][col] == GRAY)
			go_on = CheckNeighbourReverse(row + 1, col, pc);
		// down
		if (go_on && (maze[row - 1][col] == SPACE || maze[row - 1][col] == GRAY))
			go_on = CheckNeighbourReverse(row - 1, col, pc);
		// left
		if (go_on && (maze[row][col - 1] == SPACE || maze[row][col - 1] == GRAY))
			go_on = CheckNeighbourReverse(row, col - 1, pc);
		// right
		if (go_on && (maze[row][col + 1] == SPACE || maze[row][col + 1] == GRAY))
			go_on = CheckNeighbourReverse(row, col + 1, pc);

	}
}



// drawings are here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer with background color

	DrawMaze();

	glutSwapBuffers(); // show all
}

void idle() 
{

	if (runBFS) {
		RunBiBFSIteration();
		RunBiBFSIterationReversed();
	}

	glutPostRedisplay(); // call to display indirectly
}

void menu(int choice)
{
	if (choice == 1) // BFS
		runBFS = true;
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("BFS Example");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle);  // updates function
	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("run BFS", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}

