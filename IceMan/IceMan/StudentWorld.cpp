#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
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

int StudentWorld::init()
{
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

	return GWSTATUS_CONTINUE_GAME;

}

int StudentWorld::move()
{
	while (player->isAlive())
	{
		player->doSomething();
		++ticks;
		return GWSTATUS_CONTINUE_GAME;
	}
	decLives();
	playSound(SOUND_PLAYER_GIVE_UP);
	return GWSTATUS_PLAYER_DIED;

}

void StudentWorld::cleanUp()
{

	delete player;
	for (auto deleteObj : gameObj)
	{
		delete deleteObj;
		gameObj.pop_back();
	}
}

StudentWorld::~StudentWorld()
{
	gameObj.clear();
	for (int column = 0; column < VIEW_HEIGHT; column++)
	{
		for (int row = 0; row < VIEW_WIDTH; row++) {
			delete iceField[column][row];
		}
	}
}


// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
