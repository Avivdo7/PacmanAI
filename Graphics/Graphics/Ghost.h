#pragma once
#include "Const.h"
#include "AStar.h"
#include <thread>

class Ghost
{
public:
	Ghost(Point2D* pos, Point2D* target);
	~Ghost();
	void run();
private:
	Point2D* pos;
	Point2D* pacMan;
	AStar* aStar;
	int lastPosColor;
};

