#pragma once
#include "Point2D.h"
#include "AStar.h"
#include <thread>

const int MIN_RUN = 30;

class Pacman
{
public:
	Pacman(Point2D* pos);
	~Pacman();
	void setGhosts(int numberOfGhosts, Point2D**& ghosts);
	void setCoins(int numberOfCoins, Point2D**& coins);
	void run();
private:
	bool win;

	Point2D* pos;

	vector<Point2D> coins;
	vector<AStar*> aStarCoins;
	Point2D* lastPosCoin;
	int indexCoin;

	vector<Point2D*> ghosts;
	AStar* runGhost;
	Point2D* lastPosRunGhost;
	bool runFromGhost();
	void newFromGhostPoint(Point2D*& ghost);

	void searchCoins();
};

