#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <sstream>
#include <string>
#include <vector>

//STUDENTWORLD
/////////////////////////////////////////

class StudentWorld : public GameWorld {
public:
	StudentWorld(std::string assetDir); 
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addShip();
	NachenBlaster* getPlayer();
	virtual ~StudentWorld();
	std::vector<Actor*>* getStartVec();
	std::vector<Actor*>::iterator getBegin();
	std::vector<Actor*>::iterator getEnd();
private:
	std::vector<Actor*> m_Actors; //Vector of pointers to the actors 
	NachenBlaster* m_player; //pointer to a NachenBlaster
	int m_nShips;
	int m_shipsDestroyed;
};

#endif // STUDENTWORLD_H_
