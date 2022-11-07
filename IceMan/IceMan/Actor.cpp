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

std::string Actor::type()
{
	return "Actor";
}

Actor::~Actor()
{
	studentWorld = nullptr;
}

Ice::Ice(int xAxis, int yAxis, StudentWorld* world) :Actor(IID_ICE, xAxis, yAxis, right, 0.25, 3, world)
{}

void Ice::doSomething()
{}

std::string Ice::type()
{
	return "Ice";
}

Ice::~Ice()
{}

CharacterBase::CharacterBase(int imageID, int xAxis, int yAxis, Direction startDirection, float size = 1.0, unsigned int depth = 0, StudentWorld* tempWorld = nullptr) :Actor(imageID, xAxis, yAxis, startDirection, size, depth, tempWorld)
{
	this->health = 0;
}

//bool CharacterBase::isAnnoyed(int annoy)
//{
//
//}
//
bool CharacterBase::isNotMap(int ch)
{

	switch (ch) {
	case KEY_PRESS_LEFT:
		if (getX() - 1 < 0) {
			return false;
		}

		break;
	case KEY_PRESS_RIGHT:
		if (getX() + 1 > 60) {
			return false;
		}

		break;
	case KEY_PRESS_UP:
		if (getY() + 1 > 60) {
			return false;
		}

		break;
	case KEY_PRESS_DOWN:
		if (getY() - 1 < 0) {
			return false;
		}

		break;
	};

	return true;
}

//int CharacterBase::getDamage()
//{
//
//}
//
//void CharacterBase::setDamage(int hp)
//{
//
//}

void CharacterBase::setHealth(int hp)
{
	this->health = hp;
}

int CharacterBase::getHealth()
{
	return this->health;
}

CharacterBase::~CharacterBase()
{}

IceMan::IceMan(StudentWorld* world) : CharacterBase(IID_PLAYER, 30, 60, right, 1.0, 0, world)
{
	this->setHealth(10);
}

void IceMan::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		Direction dir = getDirection();

		switch (ch) {
		case KEY_PRESS_LEFT:

			if (getDirection() != left) {
				setDirection(left);
				break;
			}

			if (isNotMap(KEY_PRESS_LEFT)) {
				moveTo(getX() - 1, getY());
			}

			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() != right) {
				setDirection(right);

				break;
			}


			if (isNotMap(KEY_PRESS_RIGHT)) {
				moveTo(getX() + 1, getY());
			}

			break;
		case KEY_PRESS_UP:
			if (getDirection() != up) {
				setDirection(up);

				break;
			}


			if (isNotMap(KEY_PRESS_UP)) {
				moveTo(getX(), getY() + 1);
			}

			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down) {
				setDirection(down);
				break;
			}


			if (isNotMap(KEY_PRESS_DOWN)) {
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}

			break;
		}
		return;
	}


}

//void IceMan::setInventoryCount(int item)
//{
//
//}
//
//int IceMan::getInventoryCount()
//{
//
//}
//


std::string IceMan::type()
{
	return "IceMan";
}

IceMan::~IceMan()
{}

Boulder::Boulder(StudentWorld* world, int x, int y) :Actor(IID_BOULDER, x, y, right, 1.0, 1, world)
{
	setVisible(true);
}

void Boulder::doSomething()
{

}