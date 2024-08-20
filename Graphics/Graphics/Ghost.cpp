#include "Ghost.h"

extern int maze[MSIZE][MSIZE];
extern bool done;

Ghost::Ghost(Point2D* pos, Point2D* target)
{
	this->pos = pos;
	this->pacMan = target;
	aStar = new AStar(pos);
	lastPosColor = SPACE;
}


Ghost::~Ghost()
{
	delete pos;
	delete aStar;
}

void Ghost::run() {
	Point2D* helper = NULL;

	while (!done)
	{
		if (aStar->run(*pacMan, -1)) {
			delete helper;
			helper = aStar->getTop();
			if (helper != NULL && !(*helper == *pacMan)) {
				this_thread::sleep_for(chrono::microseconds(GHOST_SLEEP));
				(maze)[pos->getY()][pos->getX()] = lastPosColor;
				pos->copy(*helper);
				lastPosColor = (maze)[pos->getY()][pos->getX()];
				(maze)[pos->getY()][pos->getX()] = GHOST;
			}
			else
				done = true;
		}
		else
			break;
	}
	delete helper;
}