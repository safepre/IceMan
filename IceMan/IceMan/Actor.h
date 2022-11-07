#ifndef ACTOR_H
#define ACTOR_H
#include "GraphObject.h"
#include "StudentWorld.h"

class StudentWorld;
//PVF, interface for all the specified actors
class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld* world);

    StudentWorld* getWorld() const;    //Returns a pointer to the gameworld, access to all actors, and update

    bool isAlive();

    void notAlive();

    std::string type();

    void setTick(int tickUpdate);

    void getTick();

    //during each tick
    virtual void doSomething() = 0; //PVF

    ~Actor();

private:
    bool stillAlive;                //Alive or not
    StudentWorld* studentWorld;     //Allows classes to see the gameworld and use the class functions, a game pointer
    int tick = 0;
};

class Ice : public Actor
{
public:
    Ice(int x, int y, StudentWorld* world);

    virtual void doSomething();

    std::string type();

    ~Ice();

private:

};

class CharacterBase : public Actor
{
public:
    CharacterBase(int imageID, int xAxis, int yAxis, Direction startDirection, float size, unsigned int depth, StudentWorld* tempWorld);

    bool isAnnoyed(int annoy);

    virtual bool isNotMap(int pass);    //making sure that the character is not outside of the map/boundary

    int getDamage();

    void setDamage(int hp);

    void setHealth(int hp);

    int getHealth();

    ~CharacterBase();
private:
    int health;
};
class IceMan : public CharacterBase
{
public:
    IceMan(StudentWorld* world);

    virtual void doSomething();

    void setInventoryCount(int item);

    int getInventoryCount();

    std::string type();

    ~IceMan();
private:
    int invenctory[3];
};

class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int x, int y);
    virtual void doSomething();

private:

};
class Protestor : public CharacterBase
{
public:
    Protestor(StudentWorld* world);

    virtual void doSomething();

private:

};
#endif // ACTOR_H