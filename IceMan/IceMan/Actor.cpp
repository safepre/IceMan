#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

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

bool CharacterBase::isAnnoyed(int hp)
{
	setDamage(hp);
	if (getHealth() <= 0) {
		notAlive();
		getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
		return true;
	}
	return false;
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
		if (!getWorld()->isThereAnyIceInBothDirections(getX() - 4, getY()) && !getWorld()->isBoulder(getX() - 4, getY(),3))
			getWorld()->addActors(new Squirt(getX() - 4, getY(),getWorld(),getDirection()));

		break;
	case right:


		break;
	case down:

		break;
	case up:


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
	setVisible(false);
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
		pickUp();
	}
	increaseTick();
}

void Gold::pickUp()
{
	if (getTick() == tickDrop + 100) {
		notAlive();
	}

}

void Gold::updateTick()
{
	tickDrop = getTick();
}

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
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->addInventory(IID_GOLD);
		getWorld()->increaseScore(10);
		return;
	}
}

void Barrels::updateTick()
{
	return;
}
//RegularProtestor::RegularProtestor(StudentWorld* world):CharacterBase(IID_PROTESTER, 30, 60, left, 1.0, 0, world)
//{
//	setVisible(true);
//	this->setHealth(5);
//}