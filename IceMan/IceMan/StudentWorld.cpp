#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
	lives = getLives();
	level = getLevel();
	world = this;
	player = nullptr;
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
		actorsDoSomething();
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
			y = rand() % ((37)+ 20);
			break;
		}


		default:
		{
			//'.'
			x = rand() % (VIEW_HEIGHT - SPRITE_HEIGHT) + 1;
			y = rand() % ((VIEW_HEIGHT - SPRITE_HEIGHT) - (SPRITE_WIDTH + 1));
			break;
		}
	}

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

//void StudentWorld::setDisplay()
//{
//	stringstream s;
//	s << "Lvl: " << setw(2) << getLevel() << " ";
//	s << "Lives: " << getLives() << " ";
//	s << "Hlth: " << setw(3) << player->getHealth() * 10 << "% ";
//	s << "Wtr: " << setw(2) << player->getSquirt() << " ";
//	s << "Gld: " << setw(2) << player->getGold() << " ";
//	s << "Oil Left: " << setw(2) << this << " ";
//	s << "Sonar: " << setw(2) << player->getSonar() << " ";
//	s << "Scr: " << setw(6) << setfill('0') << getScore();
//
//	setGameStatText(s.str());
//}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
