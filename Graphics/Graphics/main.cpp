#include "Const.h"
#include "glut.h"
#include "Ghost.h"
#include "Pacman.h"
#include <thread>
#include <time.h>

const int W = 750; // window width
const int H = 750; // window height

int maze[MSIZE][MSIZE];
bool done = false;
const int NUMBER_OF_GHOSTS = 3;
const int NUMBER_OF_COINS = 3;

Ghost** ghosts;
Pacman* pacman;
Point2D** coins;

vector<thread> thread_pool;

void setupMaze()
{
	int i, j;

	//set frame
	for (i = 0; i < MSIZE; i++)
	{
		maze[i][0] = maze[i][MSIZE - 1] = WALL;
		maze[0][i] = maze[MSIZE - 1][i] = WALL;
	}

	// set walls
	for (i = 1; i < MSIZE - 1; i++)
		for (j = 1; j < MSIZE - 1; j++)
		{
			if (i % 2 == 0) // mostly walls
			{
				if (rand() % 100 < 50) maze[i][j] = WALL;
			}
			else // mostly spaces
			{
				if (rand() % 100 < 25) maze[i][j] = WALL;
			}
		}
}

void clearMemory()
{
	for (int i = 0; i < (int)thread_pool.size(); i++)
		thread_pool[i].join();
	thread_pool.clear();
	for (int i = 0; i < NUMBER_OF_GHOSTS; i++)
		delete ghosts[i];
	delete[]ghosts;
	for (int i = 0; i < NUMBER_OF_COINS; i++)
		delete coins[i];
	delete[]coins;
	delete pacman;
}

void clean()
{
	done = false;
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			if (maze[i][j] != PACMAN && maze[i][j] != GHOST && maze[i][j] != WALL)
				maze[i][j] = SPACE;
		}
	}
}

void init()
{
	clean();
	int i, j;

	srand(time(0));

	// clean up the maze
	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
			maze[i][j] = SPACE;

	setupMaze();

	Point2D* pacmanPos;
	pacmanPos = new Point2D(MSIZE / 2, MSIZE / 2);
	pacman = new Pacman(pacmanPos);
	maze[pacmanPos->getY()][pacmanPos->getX()] = PACMAN;

	ghosts = new Ghost * [NUMBER_OF_GHOSTS];
	Point2D** pos;
	pos = new Point2D * [NUMBER_OF_GHOSTS];
	int x, y;

	for (int i = 0; i < NUMBER_OF_GHOSTS; i++)
	{
		x = rand() % MSIZE;
		y = rand() % MSIZE;
		pos[i] = new Point2D(x, y);
		ghosts[i] = new Ghost(pos[i], pacmanPos);
		maze[y][x] = GHOST;
	}

	pacman->setGhosts(NUMBER_OF_GHOSTS, pos);
	delete[] pos;

	coins = new Point2D * [NUMBER_OF_COINS];
	for (int i = 0; i < NUMBER_OF_COINS; i++)
	{
		x = rand() % MSIZE;
		y = rand() % MSIZE;
		coins[i] = new Point2D(x, y);
		maze[y][x] = COIN;
	}
	pacman->setCoins(NUMBER_OF_COINS, coins);

	glOrtho(-1, 1, -1, 1, -1, 1);
}

void drawMaze()
{
	int i, j;

	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
		{
			switch (maze[i][j])
			{
			case WALL:
				glColor3d(0, 0, 0); // black
				break;
			case SPACE:
				glColor3d(1, 1, 1); // white
				break;
			case PACMAN:
				glColor3d(0, 0, 1); // blue
				break;
			case GHOST:
				glColor3d(1, 0, 0); // red
				break;
			case COIN:
				glColor3d(1, 1, 0); // yellow
				break;
			case PACMAN_LOSE:
				glColor3d(1, 0.5, 0); // orange
				break;
			case PACMAN_WIN:
				glColor3d(0, 1, 0); // green
				break;
			}
			// draw square
			glBegin(GL_POLYGON);
			glVertex2d(j * SQSIZE - 1 - SQSIZE / 2, i * SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j * SQSIZE - 1 + SQSIZE / 2, i * SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j * SQSIZE - 1 + SQSIZE / 2, i * SQSIZE - 1 - SQSIZE / 2);
			glVertex2d(j * SQSIZE - 1 - SQSIZE / 2, i * SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	drawMaze();

	glutSwapBuffers();
}

void idle()
{
	display();
	glutPostRedisplay();
}

void menu(int choice)
{
	switch (choice)
	{
	case 1: //Play - triggering Pacman and ghosts to start "running" in the maze
		thread_pool.push_back(std::thread(&Pacman::run, pacman));
		for (int i = 0; i < NUMBER_OF_GHOSTS; i++)
			thread_pool.push_back(std::thread(&Ghost::run, ghosts[i]));
		break;
	case 2: //Restart Game
		clearMemory();
		init();
		break;
	}
}

void main(int argc, char* argv[])
{
	atexit(clearMemory);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(250, 0);
	glutCreateWindow("PacMan AI");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	init();

	glutCreateMenu(menu);
	glutAddMenuEntry("Play", 1);
	glutAddMenuEntry("Restart Game", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}