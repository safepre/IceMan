#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <algorithm>
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
Actor::Actor(int imageID, int xAxis, int yAxis, Direction startDirection, float size, unsigned int depth, StudentWorld* world)
	:GraphObject(imageID, xAxis, yAxis, startDirection, size, depth), studentWorld(world)
{
	setVisible(true);
}
StudentWorld* Actor::getWorld() const
{
	return studentWorld;
}

bool Actor::isAlive()
{
	return stillAlive;
}

void Actor::notAlive()
{
	stillAlive = false;
}

void Actor::moveToModified(int x, int y) //Handles the bound whenever you try to get out of the IceField
{
	if (x < 0)
	{
		x = 0;
	}
	if (x > 60)
	{
		x = 60;
	}
	if (y < 0)
	{
		y = 0;
	}
	if (y > 60)
	{
		y = 60;
	}
	GraphObject::moveTo(x, y);
}


Actor::~Actor()
{
	studentWorld = nullptr;
}

Ice::Ice(int xAxis, int yAxis, StudentWorld* world) :Actor(IID_ICE, xAxis, yAxis, right, 0.25, 3, world)
{}

void Ice::doSomething()
{}


Ice::~Ice()
{}

CharacterBase::CharacterBase(int imageID, int xAxis, int yAxis, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) :Actor(imageID, xAxis, yAxis, startDirection, size, depth, tempWorld)
{
	this->health = 0;
}

void CharacterBase::setDamage(int hp)
{
	health -= hp;
}

void CharacterBase::setHealth(int hp)
{
	health = hp;
}

int CharacterBase::getHealth() const
{
	return this->health;
}

CharacterBase::~CharacterBase()
{}

IceMan::IceMan(StudentWorld* world) : CharacterBase(IID_PLAYER, 30, 60, right, 1.0, 0, world),water(5),sonar(1),gold(0)
{
	this->setHealth(10);
}

void IceMan::isAnnoyed(int hp)
{
	setDamage(hp);
	if (getHealth() <= 0) {
		notAlive();
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
	}
}

int IceMan::getSonar() const
{
	return sonar;
}

int IceMan::getSquirt() const
{
	return water;
}

int IceMan::getGold() const
{
	return gold;
}

void IceMan::updateSonar()
{
	sonar++;
}

void IceMan::updateSquirt()
{
	water += 5;
}

void IceMan::updateGold()
{
	gold++;
}


void IceMan::doSomething()
{
	bool flag = false;
	int ch;
	Direction dir = getDirection();
	if (getWorld()->diggingIce(getX(), getY(), flag))
		getWorld()->playSound(SOUND_DIG);
	if (getWorld()->getKey(ch) == true)
	{
		switch (ch) {
		case KEY_PRESS_ESCAPE:
			notAlive();
			break;
		case KEY_PRESS_LEFT:
			if (dir == left) {
				if (!getWorld()->isBoulder(getX() - 1, getY(), 3))
					moveToModified(getX() - 1, getY());
				else if (getWorld()->isBoulder(getX() - 1, getY(), 3))
					moveToModified(getX(), getY());
			}
			else setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (dir == right) {
				if (!getWorld()->isBoulder(getX() + 1, getY(), 3))
					moveToModified(getX() + 1, getY());
				else if (getWorld()->isBoulder(getX() + 1, getY(), 3))
					moveToModified(getX(), getY());
			}
			else setDirection(right);
			break;
		case KEY_PRESS_UP:   
			if (dir == up) {
				if (!getWorld()->isBoulder(getX(), getY() + 1, 3))
					moveToModified(getX(), getY() + 1);
				else if (getWorld()->isBoulder(getX(), getY() + 1, 3))
					moveToModified(getX(), getY());
			}
			else setDirection(up);
			break;
		case KEY_PRESS_DOWN: 
			if (dir == down) {
				if (!getWorld()->isBoulder(getX(), getY() - 1, 3))
					moveToModified(getX(), getY() - 1);
				else if (getWorld()->isBoulder(getX(), getY() - 1, 3))
					moveToModified(getX(), getY());
			}
			else setDirection(down);
			break;
		case KEY_PRESS_SPACE:
			if (water > 0)
			{
				water--;
				shoot();
			}
			break;
		}
	}
}

void IceMan::addGoodies(int add)
{
	if (add == IID_GOLD) gold++;
	if (add == IID_WATER_POOL) water += 5;
	if (add == IID_SONAR) sonar += 2;
}

void IceMan::shoot()
{
	switch (getDirection())
	{
	case left:
		if (!getWorld()->isThereAnyIceInBothDirections(getX() - 4, getY()) && !getWorld()->isBoulder(getX() - 4, getY(), 3))
			getWorld()->addActors(new Squirt(getX() - 4, getY(), getWorld(), getDirection()));
		break;
	case right:
		if (!getWorld()->isThereAnyIceInBothDirections(getX() + 4, getY()) && !getWorld()->isBoulder(getX() + 4, getY(), 3))
			getWorld()->addActors(new Squirt(getX() + 4, getY(), getWorld(), getDirection()));
		break;
	case down:
		if (!getWorld()->isThereAnyIceInBothDirections(getX(), getY()-4) && !getWorld()->isBoulder(getX(), getY()-4, 3))
			getWorld()->addActors(new Squirt(getX(), getY()-4, getWorld(), getDirection()));
		break;
	case up:
		if (!getWorld()->isThereAnyIceInBothDirections(getX(), getY() + 4) && !getWorld()->isBoulder(getX(), getY() + 4, 3))
			getWorld()->addActors(new Squirt(getX(), getY() + 4, getWorld(), getDirection()));
		break;
	case none: return;
	}
	getWorld()->playSound(SOUND_PLAYER_SQUIRT);
}

IceMan::~IceMan()
{}

Boulder::Boulder(StudentWorld* world, int x, int y) :Actor(IID_BOULDER, x, y, down, 1.0, 1, world),isStable(true)
{
	flagFall = false;
	this->ticks = 0;
}

void Boulder::doSomething()
{
	if (!isAlive()) return;
	if (isStable)
	{
		if (getWorld()->isThereAnyIce(getX(), getY() - 1))
		{
			return;
		}
		else
		{
			isStable = false;
		}
	}
	if (ticks == 30)
	{
		flagFall = true;
		getWorld()->playSound(SOUND_FALLING_ROCK);
	}
	ticks++;
	if (flagFall) {
		if (getWorld()->isThereAnyIce(getX(), getY() - 1) || getWorld()->isBoulder(getX(), getY() - 4, 0) || getY() == 0)
		{
			notAlive();
		}
		else
		{
			moveTo(getX(), getY() - 1);
		}
	}
}

Squirt::Squirt(int x, int y, StudentWorld* world, Direction dir):Actor(IID_WATER_SPURT, x, y, dir, 1.0, 1, world)
{}

void Squirt::doSomething()
{
	if(!isAlive()) return;
	switch (getDirection()) {
		case left:
			if (getWorld()->isThereAnyIce(getX() - 1, getY()) || getWorld()->isBoulder(getX() - 1, getY(),3)) {
				notAlive(); return;
			}
			else moveTo(getX() - 1, getY());
			break;
		case right:
			if (getWorld()->isThereAnyIce(getX() + 1, getY()) || getWorld()->isBoulder(getX() + 1, getY(), 3)) {
				notAlive(); return;
			}
			else moveTo(getX() + 1, getY());
			break;
		case up:
			if (getWorld()->isThereAnyIce(getX(), getY()+1) || getWorld()->isBoulder(getX(), getY()+1, 3)) {
				notAlive(); return;
			}
			else moveTo(getX(), getY()+1);
			break;
		case down:
			if (getWorld()->isThereAnyIce(getX(), getY() - 1) || getWorld()->isBoulder(getX(), getY() - 1, 3)) {
				notAlive(); return;
			}
			else moveTo(getX(), getY() - 1);
			break;
	}
}

Items::Items(StudentWorld* world, int imageID, int x, int y, int maxTick):Actor(imageID, x, y, right, 1.0, 2, world)
{}

void Items::increaseTick()
{
	itemTick++;
}

int Items::getTick() const
{
	return itemTick;
}
Gold::Gold(StudentWorld* world, int x, int y, bool drop):Items(world, IID_GOLD, x, y, 100),isGrabItem(drop)
{
	setVisible(true);
}

void Gold::doSomething()
{
	if (!isAlive()) return;

	if (!isVisible() && getWorld()->clientRadius(this, 4))
	{
		setVisible(true);
		return;
	}
	else if (!isGrabItem && getWorld()->clientRadius(this, 3))
	{
		notAlive();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->addInventory(IID_GOLD);
		getWorld()->increaseScore(10);
		return;
	}
	else
	{
		pickUpDisappear();
	}
	increaseTick();
}

void Gold::pickUpDisappear()
{
	if (getTick() == tickDrop + 100) notAlive();
}

void Gold::updateTick()
{
	tickDrop = getTick();}

Barrels::Barrels(StudentWorld* world, int x, int y) :Items(world, IID_BARREL, x, y, 0)
{
	setVisible(true);
}

void Barrels::doSomething()
{
	if (!isAlive()) return;
	if (!isVisible() && getWorld()->clientRadius(this, 4))
	{
		setVisible(true);
		return;
	}
	else if (getWorld()->clientRadius(this, 3))
	{
		notAlive();
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decBarrel();
		return;
	}
}

void Barrels::updateTick()
{
	return;
}

WaterRefills::WaterRefills(StudentWorld* world, int x, int y):Items(world,IID_WATER_POOL,x,y,0)
{}

void WaterRefills::doSomething()
{
	if (!isAlive()) return;

	int tick = std::max(100, 300 - 10 * (int)getWorld()->getLevel());

	if (getTick() == this->tickWater + tick)
	{
		notAlive();
	}

	else if (getWorld()->clientRadius(this, 3))
	{
		notAlive();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->addInventory(IID_WATER_POOL);
		getWorld()->increaseScore(75);
	}
	increaseTick();
}

void WaterRefills::updateTick()
{
	tickWater = getTick();
}

Sonar::Sonar(StudentWorld* world, int x, int y) :Items(world, IID_SONAR, x, y, 0)
{}

void Sonar::doSomething()
{
	if (!isAlive()) return;

	int tick = std::max(100, 300 - 10 * (int)getWorld()->getLevel());

	if (getTick() == this->tickSonar + tick)
	{
		notAlive();
	}

	else if (getWorld()->clientRadius(this, 3))
	{
		notAlive();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->addInventory(IID_SONAR);
		getWorld()->increaseScore(75);
	}
	increaseTick();
}

void Sonar::updateTick()
{
	tickSonar = getTick();
}

AI::AI(StudentWorld* world, int image, int hp):CharacterBase(image,60,60,left,1.0,0,world)
{
	leave = false;
	tickTurn = 200;
	tickWithoutWarning = 15;
	randomMove();
	tickRestState = std::max(0, 3 - (int)getWorld() / 4);
}
void AI::moveDirectionPro(Direction direction)
{
	switch (direction) {
	case left:
		if (getDirection() == left) {
			if (getX() == 0) setDirection(right);
			moveTo(getX() - 1, getY());
		}
		else setDirection(left);
		break;
	case right:
		if (getDirection() == right) {
			if (getX() == 60) setDirection(left);
			moveTo(getX() + 1, getY());
		}
		else setDirection(right);
		break;
	case up:
		if (getDirection() == up) {
			if (getY() == 60) setDirection(down);
			moveTo(getX(), getY() + 1);
		}
		else setDirection(up);
		break;
	case down:
		if (getDirection() == down) {
			if (getY() == 0) setDirection(up);
			moveTo(getX(), getY() - 1);
		}
		else setDirection(down);
		break;
	case none: return;
	}
}
void AI::moveDirection(Direction d)
{
	switch (d) 
	{
	case left:
		if (getDirection() == left) {
			if (!getWorld()->isBoulder(getX() - 1, getY(), 0))
			{
				moveTo(getX() - 1, getY());
			}
			else
			{
				moveTo(getX(), getY());
			}
		}
		else 
		{
			setDirection(left);
		}
		break;
	case right:
		if (getDirection() == right)
		{
			if (!getWorld()->isBoulder(getX() + 1, getY(), 0))
			{
				moveTo(getX() + 1, getY());
			}
			else
			{
				moveTo(getX(), getY());
			}
		}
		else
		{
			setDirection(right);
		}
		break;
	case up:
		if (getDirection() == up) 
		{
			if (!getWorld()->isBoulder(getX(), getY() + 1, 0))
			{
				moveTo(getX(), getY() + 1);
			}
			else 
			{
				moveTo(getX(), getY());
			}
		}
		else
		{
			setDirection(up);
		}
		break;
	case down:
		if (getDirection() == down) 
		{
			if (!getWorld()->isBoulder(getX(), getY() - 1, 0))
			{
				moveTo(getX(), getY() - 1);
			}
			else
			{
				moveTo(getX(), getY());
			}
		}
		else 
		{
			setDirection(down);
		}
		break;
	case none: 
		return;
	}
}

void AI::doSomething()
{
	if (!isAlive()) return;

	if (tickRestState > 0)
	{
		tickRestState--;
		return;
	}
	else
	{
		tickRestState = std::max(0, 3 - (int)getWorld()->getLevel() / 4);
		tickTurn++;
		tickWithoutWarning++;
	}
	if (leave)
	{
		if (getX() == 60 && getY() == 30) {
			notAlive();
			getWorld()->decProtester();
			return;
		}
			getWorld()->exitBFS(this);
			return;
	}
	if (getWorld()->clientRadius(this, 4) && facing())
	{
		if (tickWithoutWarning > 15)
		{
			getWorld()->getIceMan()->isAnnoyed(2);
			getWorld()->playSound(SOUND_PROTESTER_YELL);
			tickWithoutWarning = 0;
			return;
		}
		return;
	}
	if (directionToIceMan() != none && (!getWorld()->clientRadius(this, 4)))
	{
		setDirection(directionToIceMan());
		moveDirectionPro(directionToIceMan());
		numMove = 0;
		return;
	}
	numMove--;
	if (numMove < 1)
	{
		Direction temp = randomDirection();
		while (true)
		{
			temp = randomDirection();
			if (getWorld()->isNotBoundary(getX(), getY(), temp));
				break;
		}
		setDirection(temp);
		randomMove();
	}
	else if (isAtPath() && tickTurn > 200)
	{
		pickOptimalDirection();
		tickTurn = 0;
		randomMove();
	}
	moveDirectionPro(getDirection());
	if (!getWorld()->isNotBoundary(getX(), getY(), getDirection()))
	{
		numMove = 0;
	}
}

GraphObject::Direction AI::randomDirection()
{
	/*int randNum;
	randNum = rand() % 4;
	if (randNum == 0)
	{
		return left;
	}
	else if (randNum == 1)
	{
		return right;
	}
	else if (randNum == 2)
	{
		return up;
	}
	else
	{
		return down;
	}
	return none;*/
	int r;
	r = rand() % 4;
	switch (r) {
	case 0: return left;
	case 1: return  right;
	case 2: return up;
	case 3: return down;
	}
	return none;
}

bool AI::straightPathToIceMan(Direction dir)
{
	int x = getWorld()->getIceMan()->getX();
	int y = getWorld()->getIceMan()->getY();
	switch (dir)
	{
	case left:
		for (int i = getX(); i >= x; i--)
		{
			if (getWorld()->isThereAnyIce(i, getY()) || getWorld()->isBoulder(i, getY(), 3))
				return false;
		}
		return true;
		break;

	case right:
		for (int i = getX(); i <= x; i++)
		{
			if (getWorld()->isThereAnyIce(i, getY()) || getWorld()->isBoulder(i, getY(), 3))
				return false;
		}
		return true;
		break;

	case up:
		for (int i = getY(); i <= y; i++)
		{
			if (getWorld()->isThereAnyIce(getX(),i) || getWorld()->isBoulder(getX(), i, 3))
				return false;
		}
		return true;
		break;

	case down:
		for (int i = getY(); i >= y; i--)
		{
			if (getWorld()->isThereAnyIce(getX(),i) || getWorld()->isBoulder(getX(), i, 3))
				return false;
		}
		return true;
		break;
	case none:
		return false;
	}
}
bool AI::facing()
{
	switch (getDirection())
	{
	case left:
		return getWorld()->getIceMan()->getX() <= getX();
	case right:
		return getWorld()->getIceMan()->getX() >= getX();
	case up:
		return getWorld()->getIceMan()->getY() >= getY();
	case down:
		return getWorld()->getIceMan()->getY() >= getY();
	case none:
		return false;
	}
	return false;
}


GraphObject::Direction AI::directionToIceMan()
{
	int x = getWorld()->getIceMan()->getX();
	int y = getWorld()->getIceMan()->getY();
	if (getY() == y && getX() == x)
	{
		return getDirection();
	}
	if (getX() == x)
	{
		if (getY() < y)
		{
			return up;
		}
		if (getY() > y)
		{
			return down;
		}
	}
	if (getY() == y)
	{
		if (getX() > x)
		{
			return left;
		}
		if (getY() < y)
		{
			return right;
		}
	}
	return none;
}

bool AI::isAtPath()
{
	if (getDirection() == up || getDirection() == down)
	{
		return (getWorld()->isNotBoundary(getX(), getY(), left) || (getWorld()->isNotBoundary(getX(), getY(), right)));
	}
	else
	{
		return (getWorld()->isNotBoundary(getX(), getY(), up) || (getWorld()->isNotBoundary(getX(), getY(), down)));
	}
}

void AI::pickOptimalDirection()
{
	if (getDirection() == up || getDirection() == down) {
		if (!getWorld()->isNotBoundary(getX(), getY(), left))
		{
			setDirection(right);
		}
		else if (!getWorld()->isNotBoundary(getX(), getY(), right))
		{
			setDirection(left);
		}
		else
		{
			/*if (rand() % 2 == 0)
			{
				setDirection(left);
			}
			else if (rand() % 2 == 1)
			{
				setDirection(right);
			}*/
		}
	}
	else
	{
		if (!getWorld()->isNotBoundary(getX(), getY(), up))
		{
			setDirection(down);
		}
		else if (!getWorld()->isNotBoundary(getX(), getY(), down))
		{
			setDirection(up);
		}
		else
		{
			/*if (rand() % 2 == 0)
			{
				setDirection(up);
			}
			else if (rand() % 2 == 1)
			{
				setDirection(down);
			}*/
			switch (rand() % 2) {
			case 0: setDirection(up);
			case 1: setDirection(down);
			}
		}
	}
}

void AI::isAnnoyed(int hp)
{
	hp = 0;
	 
}

void AI::randomMove()
{
	numMove = rand() % 55 + 6;  //0-60 random
}

RegularProtester::RegularProtester(StudentWorld* world):AI(world,IID_PROTESTER,5)
{}

//HardcoreProtester::HardcoreProtester(StudentWorld* world) : AI(world, IID_HARD_CORE_PROTESTER, 15)
//{}