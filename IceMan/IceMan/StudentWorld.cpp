#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <queue>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::setDisplay()
{
	stringstream stringOutput;
	stringOutput << "Lvl:" << setw(2) << getLevel() << " ";
	setGameStatText(stringOutput.str());
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
	firstTick = true;
	lives = getLives();
	level = getLevel();
	world = this;
	player = nullptr;
	protesterLives = 0;
	ticks = 0;

	//string assetDir = assetDir;

	for (int column = 0; column < VIEW_HEIGHT; column++)
	{
		for (int row = 0; row < VIEW_WIDTH; row++) {
			iceField[column][row] = nullptr;
		}
	}

}

StudentWorld::~StudentWorld()
{
	cleanUp();
}

int StudentWorld::init()
{

	bool flag = true;
	protesterLives = 0;
	player = new IceMan(this);

	for (int i = 0; i < VIEW_HEIGHT; i++)
	{
		for (int j = 0; j < VIEW_WIDTH - SPRITE_HEIGHT; j++)
		{
			if (i >= (VIEW_WIDTH / 2) - 2 && i <= ((VIEW_WIDTH / SPRITE_WIDTH) + (VIEW_HEIGHT / SPRITE_HEIGHT) + 1) && j >= SPRITE_WIDTH)
			{
				continue;
			}
			iceField[i][j] = new Ice(i, j, this);
		}
	}
	//BOULDER
	int numBoulders = min((int)getLevel() / 2 + 2, 9);
	int x, y;
	randomCoordinates(x, y, 'B');
	for (int i = 0; i < numBoulders; i++)
	{
		randomCoordinates(x, y, 'B');
		
		while (x > (30 - SPRITE_WIDTH) && x < 34 && y > 0)
		{
			randomCoordinates(x, y, '.');
		}
		diggingIce(x, y, flag);
		addActors(new Boulder(this, x, y));
	}
	//BARREL
	this->barrel = min(2+(int)getLevel(), 21);
	randomCoordinates(x, y, 'O');
	for (int i = 0; i < barrel; i++)
	{
		randomCoordinates(x, y, 'O');
		addActors(new Barrels(this, x, y));
	}

	//GOLD
	x, y = 0;
	int gold = max(5 - (int)getLevel() / 2, 2);
	randomCoordinates(x, y, 'G');
	for (int i = 0; i < gold; i++)
	{
		randomCoordinates(x, y, 'G');
		addActors(new Gold(this, x, y, false));
	}
	return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::diggingIce(int x, int y, bool flag = false)
{
	for (int i = x; i < x + 4; i++)
	{
		for (int j = y; j < y + 4; j++)
		{
			if (iceField[i][j] != nullptr)
			{
				delete iceField[i][j];
				iceField[i][j] = nullptr;
				flag = true;
			}
		}
	}
	return flag;
}


int StudentWorld::move()
{
	while (player->isAlive())
	{
		setDisplay();
		actorsDoSomething();
		addProtester();
		int x, y;
		int G = ((int)getLevel()*25) + 300;
		if (int(rand() % G) + 1 == 1) {
			if (int(rand() % 5) + 1 == 1) {
				addActors(new Sonar(this, 0, 60));
			}
			else {
				do {
					//spawns
					x = rand() % 60 + 1; //0-60
					y = rand() % 60 + 1;
				} while (isThereAnyIceInBothDirections(x, y));
				addActors(new WaterRefills(this, x, y));
			}
		}
		clearDeadActors();

		if (barrel == 0)
			return GWSTATUS_FINISHED_LEVEL;

		return GWSTATUS_CONTINUE_GAME;
	}

	decLives();
	playSound(SOUND_PLAYER_GIVE_UP);
	return GWSTATUS_PLAYER_DIED;
	

}

void StudentWorld::clearDeadActors()
{
	std::vector<Actor*>::iterator it = gameObj.begin();

	while (it != gameObj.end()) {
		if (!((*it)->isAlive())) {
			(*it)->setVisible(false);
			(*it) = nullptr;
			gameObj.erase(it);
			it = gameObj.begin();
		}
		else
			++it;
	}
}

void StudentWorld::cleanUp()
{
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			delete iceField[x][y];
		}
	}
	vector<Actor*>::iterator it;
	for (it = gameObj.begin();it != gameObj.end();it++)
	{
		delete* it;
	}
	gameObj.clear();
	delete player;
}

void StudentWorld::addActors(Actor *actor)
{
	gameObj.push_back(actor);
}


bool StudentWorld::isThereAnyIce(int x, int y)
{
	for (int i = x; i < x + 4; i++)
	{
		if (iceField[i][y] != nullptr)
		{
			return true;
		}
	 }
	return false;
}

bool StudentWorld::isThereAnyIceInBothDirections(int x, int y)
{
	for (int i = x; i < x + 4; i++)
	{
		for (int j = y; j < y + 4; j++)
		{
			if (iceField[i][j] != nullptr)
			{
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::randomCoordinates(int& x, int& y, char option) const
{
	switch (option)
	{
		case 'B':
		{
			x = rand() % (VIEW_HEIGHT - SPRITE_HEIGHT + 1);
			y = rand() % ((37) + 20);
			break;
		}
		default:
		{
			x = rand() % (VIEW_HEIGHT - SPRITE_HEIGHT) + 1;
			y = rand() % ((VIEW_HEIGHT - SPRITE_HEIGHT) - (SPRITE_WIDTH + 1));
			break;
		}
	}
}

bool StudentWorld::isNotBoundary(int x, int y, GraphObject::Direction d)
{
	switch (d)
	{
		case GraphObject::left:
			return (x != 0 && !isThereAnyIce(x - 1, y) && !isBoulder(x - 1, y, 0));
		case GraphObject::right:
			return (x != 60 && !isThereAnyIce(x + 1, y) && !isBoulder(x + 1, y, 0));
		case GraphObject::down:
			return (y != 0 && !isThereAnyIce(x, y-1) && !isBoulder(x, y-1, 0));
		case GraphObject::up:
			return (y != 60 && !isThereAnyIce(x, y + 1) && !isBoulder(x, y + 1, 0));
		case GraphObject::none:
			return false;
	}
	return false;
}

bool StudentWorld::distanceRadius(int x1, int y1, int x2, int y2, int radius)
{
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < radius * radius;
}

bool StudentWorld::clientRadius(Actor* actor, int radius)
{
	return distanceRadius(actor->getX(), actor->getY(), player->getX(), player->getY(), radius);
}

bool StudentWorld::isBoulder(int x, int y, int radius)
{
	vector<Actor*>::iterator it;
	for (it = gameObj.begin(); it != gameObj.end(); it++)
	{
		if ((*it)->getID() == IID_BOULDER && distanceRadius(x, y, (*it)->getX(), (*it)->getY(), radius))
		{
			return true;
		}
	}
	return false;
}

void StudentWorld::actorsDoSomething()
{
	player->doSomething();
	for (std::vector<Actor*>::iterator it = gameObj.begin(); it != gameObj.end(); ++it) {
		(*it)->doSomething();
	}
}

void StudentWorld::addInventory(int add)
{
	if (add == IID_GOLD)
		player->updateGold();
	if (add == IID_SONAR)
		player->updateSonar();
	if (add == IID_WATER_POOL)
		player->updateSquirt();
}

void StudentWorld::addProtester()
{
	int T = max(25, 200 - (int)getLevel());
	int P = min(15, 2 + (int)getLevel() * (int)1.5);
	int probabilityHardcore = min(90, (int)getLevel() * 10 + 30);
		if (firstTick || (ticks > T && protesterLives < P))
		{
			if (!(rand() % 100 + 1 < probabilityHardcore))
			/*{
				addActors(new HardcoreProtester(this));
			}*/
			//else
			{
				addActors(new RegularProtester(this));
				ticks = 0;
				protesterLives++;
				firstTick = false;
			}

		}
		ticks++;
}
void StudentWorld::dropItem(Gold* gold)
{
	gold->updateTick();
	addActors(gold);
}

void StudentWorld::appearNearby(int x, int y, int radius)
{
	int a, b;
	vector<Actor*>::iterator it;
	for (it = gameObj.begin(); it != gameObj.end(); it++)
	{
		if ((*it)->getID() == IID_BARREL || (*it)->getID() == IID_GOLD)
		{
			a = (*it)->getX(); b = (*it)->getY();
			if ((x - a) * (x - a) + (y - b) * (y - b) <= radius * radius)
				(*it)->setVisible(true);
		}
	}
}

void StudentWorld::decBarrel()
{
	this->barrel--;
}

void StudentWorld::decProtester()
{
	this->protesterLives--;
}

IceMan* StudentWorld::getIceMan() const
{
	return player;
}

void StudentWorld::exitBFS(AI *bot)
{
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			fieldBFS[i][j] = 0;
		}
	}

	int protesterGetX = bot->getX();
	int protesterGetY = bot->getY();
	queue<Arena> queue;
	queue.push(Arena(60, 60));
	fieldBFS[60][60] = 1; //starting to mark
	while (!queue.empty())
	{
		Arena collect = queue.front();
		queue.pop();
		int xAxis = collect.x;
		int yAxis = collect.y;
		if (isNotBoundary(xAxis, yAxis, GraphObject::left) && fieldBFS[xAxis - 1] [yAxis] == 0) {
			queue.push(Arena(xAxis - 1, yAxis));
			fieldBFS[xAxis - 1][yAxis] = fieldBFS[xAxis][yAxis] + 1;
		}
		if (isNotBoundary(xAxis, yAxis, GraphObject::right) && fieldBFS[xAxis + 1][yAxis] == 0) {
			queue.push(Arena(xAxis + 1, yAxis));
			fieldBFS[xAxis + 1][yAxis] = fieldBFS[xAxis][yAxis] + 1;
		}
		if (isNotBoundary(xAxis, yAxis, GraphObject::up) && fieldBFS[xAxis][yAxis + 1] == 0) {
			queue.push(Arena(xAxis, yAxis + 1));
			fieldBFS[xAxis][yAxis + 1] = fieldBFS[xAxis][yAxis] + 1;
		}
		if (isNotBoundary(xAxis, yAxis, GraphObject::down) && fieldBFS[xAxis][yAxis - 1] == 0) {
			queue.push(Arena(xAxis, yAxis - 1));
			fieldBFS[xAxis][yAxis - 1] = fieldBFS[xAxis][yAxis] + 1;
		}
	}
	if (isNotBoundary(protesterGetX, protesterGetY, GraphObject::left) && fieldBFS[protesterGetX - 1][protesterGetY] < fieldBFS[protesterGetX][protesterGetY])
	{
		bot->moveDirection(GraphObject::left);
	}
	if (isNotBoundary(protesterGetX, protesterGetY, GraphObject::right) && fieldBFS[protesterGetX + 1][protesterGetY] < fieldBFS[protesterGetX][protesterGetY])
	{
		bot->moveDirection(GraphObject::right);
	}
	if (isNotBoundary(protesterGetX, protesterGetY, GraphObject::up) && fieldBFS[protesterGetX][protesterGetY + 1] < fieldBFS[protesterGetX][protesterGetY])
	{
		bot->moveDirection(GraphObject::up);
	}
	if (isNotBoundary(protesterGetX, protesterGetY, GraphObject::down) && fieldBFS[protesterGetX][protesterGetY - 1] < fieldBFS[protesterGetX][protesterGetY])
	{
		bot->moveDirection(GraphObject::down);
	}
	return;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
