#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <vector>
#include <cmath>

//ACTOR
///////////////////////////////////////////////

class StudentWorld;

class Actor : public GraphObject { 
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld *world) :
		GraphObject(imageID, startX, startY, dir, size, depth) {
		m_world = world;
		m_isAlive = true;
		m_ID = imageID;
	};
	double euclidean_dist(double x1, double y1, double x2, double y2);
	virtual void doSomething() = 0;
	bool isAlive() const;
	void setAlive(bool isAlive);
	StudentWorld* getWorld();
	int getID() const;
	bool hasCollided(Actor *a, Actor *b);
	virtual ~Actor();
private:
	StudentWorld* m_world;
	int m_ID;
	bool m_isAlive;
};

//STAR
////////////////////////////////////////////////

class Star : public Actor {
public:
	Star(double startX, double startY, double size, StudentWorld *world) : Actor(IID_STAR, startX, startY, 0, size, 3, world) {};
	virtual void doSomething();
};

//NACHENBLASTER
/////////////////////////////////////////////////

class Cabbage;
class Torpedo;
class Projectile;

class NachenBlaster : public Actor {
public:
	NachenBlaster(StudentWorld *m_world);
	virtual void doSomething();
	int getHealth() const;
	void decHealth(int m);
	int getTorpedoCount();
	int getCabbageCount();
private:
	int m_hp;
	int m_cabbageEP;
	int m_torpedoCount;
};

//PROJECTILE
//////////////////////////////////////////////////

class Projectile : public Actor {
public:
	Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* world);
	virtual void doSomething()=0;
private:
};

//CABBAGE
//////////////////////////////////////////////////

class Cabbage : public Projectile {
public:
	Cabbage(StudentWorld* w, double startX, double startY);
	virtual void doSomething();
private:

};

//TURNIP
//////////////////////////////////////////////////

class Turnip : public Projectile {
public:
	Turnip(StudentWorld* w, double startX, double startY);
	virtual void doSomething();
private:
};

//TORPEDO
//////////////////////////////////////////////////

class Torpedo : public Projectile {
public:
	Torpedo(StudentWorld* w, double startX, double startY, Direction dir);
	virtual void doSomething();
private:
	Direction m_dir;
};

//ALIEN
//////////////////////////////////////////////////

class Alien : public Actor {
public:
	Alien(int imageID, double startX, double startY, double deltaX, double deltaY, double speed, double flightplan_length, StudentWorld *world);
	virtual void doSomething() = 0;
	virtual void fire();
	double getHealth() const;
	bool isInBoundary() const;
	double getFlightLength() const;
	virtual void changeDirection(int m);
	int getDir() const;
	void decHealth(int m);
	void decFlightLength();
	void setFlightLength(int m);
	double getSpeed() const;
	virtual void setSpeed(int speed);
private:
	double m_health, m_speed, m_deltaX, m_deltaY, m_flightplan_length;
	int dir;
};

//SMALLGON
/////////////////////////////////////////////////

class Smallgon : public Alien {
public:
	Smallgon(StudentWorld* w, double startX, double startY);
	virtual void doSomething();
};

//SMOREGON
/////////////////////////////////////////////////

class Smoregon : public Alien {
public:
	Smoregon(StudentWorld* w, double startX, double startY);
	virtual void doSomething();
};

//SNAGGLEGON
/////////////////////////////////////////////////

class Snagglegon : public Alien {
public:
	Snagglegon(StudentWorld* w, double startX, double startY);
	virtual void doSomething();
	virtual void health();
	virtual void fire();
	double getHealth();
private:
	double m_health;
};

//EXPLOSION
/////////////////////////////////////////////////

class Explosion : public Actor {
public:
	Explosion(int imageID, double startX, double startY, StudentWorld* world);
	virtual void doSomething();
private:
	int m_Ticks;
};

//GOODIES
/////////////////////////////////////////////////

class Goodies : public Actor {
public:
	Goodies(int imageID, double startX, double startY, StudentWorld* world);
	virtual void doSomething();
private:
};

//EXTRA LIFE GOODIE
/////////////////////////////////////////////////

class LifeGoodie : public Goodies {
public:
	LifeGoodie(StudentWorld* world, double startX, double startY);
private:
};

//REPAIR GOODIE
////////////////////////////////////////////////

class RepairGoodie : public Goodies {
public:
	RepairGoodie(StudentWorld* world, double startX, double startY);
private:
};

//TORPEDO GOODIE
////////////////////////////////////////////////

class TorpedoGoodie : public Goodies {
public:
	TorpedoGoodie(StudentWorld* world, double startX, double startY);
private:
};

#endif // ACTOR_H_
