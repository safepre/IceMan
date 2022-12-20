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
class Items;
class Gold;
class Sonar;
class WaterPool;
class Barrels;
class Boulder;
class Squirt;
class AI;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	~StudentWorld();

	IceMan* getIceMan() const;

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	bool isNotBoundary(int x, int y, GraphObject::Direction d);

	bool isBoulder(int x, int y, int radius);

	bool isThereAnyIce(int x, int y);

	bool isThereAnyIceInBothDirections(int x, int y);

	bool isAboveEarth(int x, int y);

	bool distanceRadius(int x, int y1, int x2, int y2, int radius);

	bool clientRadius(Actor* actor, int radius);

	bool diggingIce(int x, int y, bool);

	void updateDisplayText();

	void removeObjects();

	void addActors(Actor *actor);

	void addProtester();

	void addInventory(int add);

	void randomCoordinates(int& x, int& y, char option) const;

	void actorsDoSomething();

	void clearDeadActors();
	
	void setDisplay();

	void decBarrel();

	void decProtester();

	void dropItem(Gold* gold);

	void exitBFS(AI* bot);

	void appearNearby(int x, int y, int radius);

private:
	bool firstTick;
	int barrel;
	int helper;
	int ticks;
	int protesterLives;
	int lives;
	int level;
	GameWorld* world;
	Ice* iceField[VIEW_WIDTH][VIEW_HEIGHT];
	IceMan* player;
	std::vector<Actor*> gameObj;
	int fieldBFS[64][64];
	struct Arena {
		int x;
		int y;
		Arena(int a, int b) : x(a), y(b) {}
	};

};


#endif // STUDENTWORLD_H_

