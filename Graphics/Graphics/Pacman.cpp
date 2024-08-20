#include "Pacman.h"

extern int maze[MSIZE][MSIZE];
extern bool done;

Pacman::Pacman(Point2D* pos)
{
	//init Pacman
	this->pos = pos;
	this->lastPosCoin = NULL;
	this->win = false;
	this->runGhost = new AStar(pos);
	this->lastPosRunGhost = NULL;
}


Pacman::~Pacman()
{
	//destroy Pacman
	coins.clear();
	for (int i = 0; i < (int)aStarCoins.size(); i++)
		delete aStarCoins[i];
	aStarCoins.clear();
	delete lastPosCoin;
	delete pos;
	delete runGhost;
	delete lastPosRunGhost;
}

void Pacman::setGhosts(int numberOfGhosts, Point2D**& ghosts)
{
	//setting ghosts to chase Pacman
	AStar astar = AStar(pos);
	for (int i = 0; i < numberOfGhosts; i++)
	{
		if (astar.run(*(ghosts[i])))
			this->ghosts.push_back(ghosts[i]);
	}
}

void Pacman::setCoins(int numberOfCoins, Point2D**& coins)
{
	//setting coins for Pacman to collect
	AStar* astar;
	for (int i = 0; i < numberOfCoins; i++)
	{
		astar = new AStar(pos);
		if (astar->run(*(coins[i]))) {
			this->coins.push_back(*(coins[i]));
			this->aStarCoins.push_back(astar);
		}
		else
			delete astar;
	}
	if (this->coins.size() == 0)
		done = true;
}

void Pacman::run() {
	//trigerring Pacman run
	int minH = MIN_RUN, temp, ghostIndex, numberOfUse = 0;
	
	while (!done)
	{
		//calculating distances of ghosts
		for (int i = 0; i < (int)ghosts.size(); i++)
		{
			temp = abs(pos->getX() - ghosts.at(i)->getX()) + abs(pos->getY() - ghosts.at(i)->getY());// Manhattan Distance
			
			if (i == 0 || minH > temp)
			{
				minH = temp;
				ghostIndex = i;
			}
		}

		if (minH < MIN_RUN)
		{
			if (numberOfUse <= 0 || lastPosRunGhost == NULL || !(*pos == *lastPosRunGhost))
			{
				if (lastPosRunGhost != NULL)
				{
					delete lastPosRunGhost;
					lastPosRunGhost = NULL;
				}
				numberOfUse = USE_TIME;
				newFromGhostPoint(ghosts.at(ghostIndex));
			}
			if (runFromGhost())
				numberOfUse--;
			else
				numberOfUse = 0;
			lastPosRunGhost = new Point2D(*pos);
		}
		else
		{
			searchCoins();
		}
		(maze)[pos->getY()][pos->getX()] = PACMAN;
	}
	if (win)
		maze[pos->getY()][pos->getX()] = PACMAN_WIN;
	else
		maze[pos->getY()][pos->getX()] = PACMAN_LOSE;
}

bool Pacman::runFromGhost()
{
	Point2D* helper = runGhost->getTop();

	if (helper != NULL)
	{
		this_thread::sleep_for(chrono::microseconds(PACMAN_SLEEP));
		(maze)[pos->getY()][pos->getX()] = SPACE;
		pos->copy(*helper);
		delete helper;
		return true;
	}
	return false;
}

void Pacman::newFromGhostPoint(Point2D*& ghost)
{
	int mX, mY;
	mX = ghost->getX();
	mY = ghost->getY();
	int posX, posY;
	posX = pos->getX() - mX;
	if (posX != 0)
		posX = posX / abs(posX);
	posY = pos->getY() - mY;
	if (posY != 0)
		posY = posY / abs(posY);
	Point2D pos = Point2D((MSIZE - mX + posX * 10) % MSIZE, (MSIZE - mY + posY * 10) % MSIZE);
	runGhost->run(pos, RUN);
}

void Pacman::searchCoins()
{
	if (lastPosCoin == NULL || !(*pos == *lastPosCoin))
	{
		int minLen, temp;
		for (int i = 0; i < (int)coins.size(); i++)
		{
			aStarCoins[i]->run(coins[i]);
			temp = aStarCoins[i]->getLen();
			if (i == 0 || minLen > temp) {
				indexCoin = i;
				minLen = temp;
			}
		}
	}

	Point2D* helper = aStarCoins[indexCoin]->getTop();
	this_thread::sleep_for(chrono::microseconds(PACMAN_SLEEP));
	(maze)[pos->getY()][pos->getX()] = SPACE;
	pos->copy(*helper);
	delete helper;
	delete lastPosCoin;

	if ((maze)[pos->getY()][pos->getX()] == COIN) {
		coins.erase(coins.begin() + indexCoin);
		delete aStarCoins[indexCoin];
		lastPosCoin = NULL;
		aStarCoins.erase(aStarCoins.begin() + indexCoin);
		if (coins.size() == 0) {
			done = true;
			this->win = true;
		}
	}
	else
		lastPosCoin = new Point2D(*pos);
}