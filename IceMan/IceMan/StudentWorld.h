#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
//class StudentWorld : public GameWorld
//{
//public:
//	StudentWorld(std::string assetDir)
//		: GameWorld(assetDir)
//	{
//	}
//
//	virtual int init()
//	{
//		return GWSTATUS_CONTINUE_GAME;
//	}
//
//	virtual int move()
//	{
//		// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
//		// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//		decLives();
//		return GWSTATUS_PLAYER_DIED;
//	}
//
//	virtual void cleanUp()
//	{
//	}
//
//private:
//};

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Ice;
class CharacterBase;
class IceMan;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	~StudentWorld();
	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void updateDisplayText();

	void removeObjects();

	void addProtestors();

	bool checkBoulder(int x, int y);



private:
	int ticks;
	int lives;
	int level;
	GameWorld* world;
	Ice* iceField[VIEW_WIDTH][VIEW_HEIGHT];
	IceMan* player;
	std::vector<Actor*> gameObj;
};


#endif // STUDENTWORLD_H_

