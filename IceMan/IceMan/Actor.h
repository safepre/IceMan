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

    StudentWorld* getWorld() const;   //Returns a pointer to the gameworld, access to all actors, and update

    bool isAlive();

    void notAlive();

    void moveToModified(int x, int y);

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

    ~Ice();

private:

};

class CharacterBase : public Actor
{
public:
    CharacterBase(int imageID, int xAxis, int yAxis, Direction startDirection, float size, unsigned int depth, StudentWorld* tempWorld);

    virtual void isAnnoyed(int hp) = 0;

    void decrementHealth(int hp);

    void setHealth(int hp);

    int getHealth() const;

    ~CharacterBase();
private:
    int health;
};
class IceMan : public CharacterBase
{
public:
    IceMan(StudentWorld* world);

    int getSonar() const;

    int getGold() const;

    int getSquirt() const;

    void updateSonar();

    void updateGold();

    void updateSquirt();

    virtual void doSomething();

    virtual void isAnnoyed(int health);

    void addGoodies(int add);

    void shoot();

    ~IceMan();
private:
    int water;
    int sonar;
    int gold;
};

class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int x, int y);
    virtual void doSomething();
    void annoyIceMan();

private:
    int ticks = 0;
    bool isStable;
    bool flagFall;
};

class Squirt : public Actor 
{
public:
    Squirt(int x, int y, StudentWorld* world, Direction dir);
    virtual void doSomething();
    bool annoyProtester();

private:
    int squirtTravel;
};


class Items : public Actor
{
public:
    Items(StudentWorld* world, int imageID, int x, int y, int maxTick);
    void increaseTick();
    int getTick() const;
    virtual void updateTick() = 0;
private:
    int itemTick;
};

class Gold : public Items
{
public:
    Gold(StudentWorld* world, int x, int y, bool drop);
    virtual void doSomething();
    void pickUpDisappear();
    virtual void updateTick();

private:
    bool isGrabItem;
    int tickDrop;
};


class Barrels : public Items
{
public:
    Barrels(StudentWorld* world, int x, int y);
    virtual void doSomething();
    virtual void updateTick();
};


class WaterRefills : public Items
{
public:
    WaterRefills(StudentWorld* world, int x, int y);
    virtual void doSomething();
    virtual void updateTick();
private:
    int tickWater;
};

class Sonar : public Items
{
public:
    Sonar(StudentWorld* world, int x, int y);
    virtual void doSomething();
    virtual void updateTick();
private:
    int tickSonar;
};

class AI : public CharacterBase
{
public:
    AI(StudentWorld* world, int image);
    virtual void doSomething();
    virtual void isAnnoyed(int health);
    void getBribed();
    void moveAIDirection(Direction d);
    void randomMove();
    void stunned();
    void pickOptimalDirection();
    void moveDirection(Direction d);
    void moveDirectionPro(Direction dir);
    bool facing();
    bool isAtPath();
    bool straightPathToIceMan(Direction dir);
    Direction directionToIceMan();
    Direction randomDirection();
    
private:
    bool leave;
    int numMove;
    int tickTurn;
    int tickWithoutWarning;
    int tickRestState;

};

class RegularProtester : public AI
{
public:
    RegularProtester(StudentWorld* world);
};

//class HardcoreProtester : public AI
//{
//public:
//    HardcoreProtester(StudentWorld* world);
//};
#endif //ACTOR_H