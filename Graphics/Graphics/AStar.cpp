#include "AStar.h"

extern int maze[MSIZE][MSIZE];

/*Short Explanation:
* The run method executes the A* algorithm to find a path from the initial position to a given target point in the maze.
  It also accepts a maximum cost limit (maxG) which can terminate the search early if reached.
  The algorithm uses a priority queue to manage the open set of nodes (points in the maze not yet fully processed),
  leveraging the Point2D_hg class which calculating position and cost data.
  Two vectors, black and gray, track nodes that have been fully processed and those that are still to be fully processed.
  The method iteratively expands the frontier of explored nodes by examining adjacent nodes (left, right, up, down) and adding them to the priority queue if they are not walls (WALL) and have not been processed yet.
  If the target point is reached or the cost exceeds maxG, the method constructs the path by backtracking from the target to the initial point using parent pointers in Point2D_hg instances and returns true.
  If no path is found (e.g., the priority queue is emptied without reaching the target), the method returns false.
*/

AStar::AStar(Point2D*& pos)
{
	this->last = NULL;
	this->pos = pos;
	this->lastPos = Point2D(*pos);
}


AStar::~AStar()
{
	delete last;
}

int AStar::getColor(Point2D& point) {
	return maze[point.getY()][point.getX()];
}

bool AStar::run(Point2D targetPoint, int maxG) {

	if (maxG == -1 && getColor(targetPoint) == WALL)
		return false;

	if (last != NULL)
	{
		if (targetPoint == *last && *pos == lastPos)
			return true;
		delete last;
		solution.clear();
	}

	last = new Point2D(targetPoint);
	Point2D_hg bestPoint;
	priority_queue <Point2D_hg, vector <Point2D_hg>, ComparePoints> pq; //priority queue to determine the best distance
	vector <Point2D_hg> black; //fully proccessed points
	vector <Point2D_hg> gray; //waiting to be proccessed points

	bestPoint = Point2D_hg(*pos, targetPoint);
	pq.emplace(bestPoint);
	gray.push_back(bestPoint);
	Point2D_hg* bestPointAsParent, neighborPos_hg;
	Point2D bestPointPos, neighborPos;
	vector<Point2D_hg>::iterator black_iterator;
	vector<Point2D_hg>::iterator gray_iterator;

	//visiting neighbors to find the best point for path
	do {
		if (pq.empty())
			return false;

		bestPoint = pq.top();
		pq.pop();
		gray_iterator = find(gray.begin(), gray.end(), bestPoint);
		gray.erase(gray_iterator);
		black.push_back(bestPoint);
		bestPointAsParent = new Point2D_hg(bestPoint);
		bestPointPos = bestPointAsParent->getPoint();

		if (bestPointPos == targetPoint || maxG == bestPoint.getG()) {
			while (bestPointAsParent->getParent() != NULL)
			{
				solution.push_back(*bestPointAsParent);
				bestPointAsParent = bestPointAsParent->getParent();
			}
			return true;
		}

		//searching right
		neighborPos = Point2D(bestPointPos.getX() + 1, bestPointPos.getY());
		if (getColor(neighborPos) != WALL) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		//searching left
		neighborPos = Point2D(bestPointPos.getX() - 1, bestPointPos.getY());
		if (getColor(neighborPos) != WALL) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		//searching up
		neighborPos = Point2D(bestPointPos.getX(), bestPointPos.getY() + 1);
		if (getColor(neighborPos) != WALL) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		//searching down
		neighborPos = Point2D(bestPointPos.getX(), bestPointPos.getY() - 1);
		if (getColor(neighborPos) != WALL) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}
	} while (true);
	return false;
}

Point2D* AStar::getTop()
{
	if (!solution.empty())
	{
		Point2D_hg best = solution.back();
		solution.pop_back();
		lastPos = Point2D(best.getPoint());
		return new Point2D(lastPos);
	}
	return NULL;
}

int AStar::getLen()
{
	return solution.size();
}