#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

//Function given to us
GameWorld* createStudentWorld(string assetDir) {
	return new StudentWorld(assetDir);
}

//STUDENTWORLD
//////////////////////////////////////////////////

//Initialize private variables in the constructor
StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir) {
	m_nShips = 0;
	m_shipsDestroyed = 0;
}

//Destructor calls cleanUp
StudentWorld::~StudentWorld() {
	cleanUp();
}

//Get player
NachenBlaster* StudentWorld::getPlayer() {
	return m_player;
}

//Returns a pointer to the starting of the vector of Actor pointers
vector<Actor*>* StudentWorld::getStartVec() {
	return &m_Actors;
}

//Returns an iterator to the beginning of the vector of Actor pointers
vector<Actor*>::iterator StudentWorld::getBegin() {
	return m_Actors.begin();
}

//Returns an iterator to the end of the vector of Actor pointers
vector<Actor*>::iterator StudentWorld::getEnd() {
	return m_Actors.end();
}

//Determines whether we should add ships or not
void StudentWorld::addShip() {
	//Initialize the probabilties
	int S1 = 60;
	int S2 = 20 + getLevel() * 5;
	int S3 = 5 + getLevel() * 10;
	int S = S1 + S2 + S3;
	int odds = randInt(1, S);

	// S1/S odds that the ship is a Smallgon
	if (odds <= S1) {
		m_nShips++;
		m_Actors.push_back(new Smallgon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
	}
		
	// S2/S odds that the ship is a Smoregon
	else if (odds <= S1 + S2) {
		m_nShips++;
		m_Actors.push_back(new Smoregon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
	}	

	//  S3/S odds that the ship is a Snagglegon
	else if (odds <= S) {
		m_nShips++;
		m_Actors.push_back(new Snagglegon(this, VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1)));
	}
}

//INIT()
//////////////////////////////////////////////////

//StudentWorld init()
int StudentWorld::init() {
	//Create Stars
	for (int i = 0; i < 30; i++) {
		double size = ((double)randInt(5, 50)) / 100;
		m_Actors.push_back(new Star(randInt(0, VIEW_WIDTH - 1), randInt(0, VIEW_HEIGHT - 1), size, this));
	}

	//Set privates to zero
	m_shipsDestroyed = 0;
	m_nShips = 0;

	//Create Nachenblaster
	m_player = new NachenBlaster(this);
	m_Actors.push_back(m_player);

	//Continue Game
	return GWSTATUS_CONTINUE_GAME;
}

//MOVE()
////////////////////////////////////////////////////////////

int StudentWorld::move() {
	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	//If not alive, return that the player died
	if (!m_player->isAlive()) {
		return GWSTATUS_PLAYER_DIED;
	}

	//Check health. If health <=0, player died
	if (m_player->getHealth()<=0) {
		return GWSTATUS_PLAYER_DIED;
	}

	//Call player's do something
	if (m_player->isAlive() && m_player != nullptr) {
		m_player->doSomething(); //error message
	}

	//Check if all ships are destroyed on a given level
	int m = 6 + (4 * getLevel());
	if (m == m_shipsDestroyed) {
		m_shipsDestroyed = 0;
		return GWSTATUS_FINISHED_LEVEL;
	}

	//Update Stat line
	ostringstream myStream;
	myStream.setf(ios::fixed);
	myStream.precision(0);
	myStream << "Lives: " << getLives() << setw(2) << "" << "Health: " << (m_player->getHealth() * 100 / 50) << "%" << setw(2) << ""
		<< "Score: " << getScore() << setw(2) << "" << "Level: " << getLevel() << setw(2) << "" << "Cabbages: " << (m_player->getCabbageCount() * 100 / 30) << "%" << setw(2) << ""
		<< "Torpedoes: " << m_player->getTorpedoCount();
	setGameStatText(myStream.str());
	
	//Count number of ships destroyed and delete any actors that are dead
	for (int i = 0; i < m_Actors.size(); i++) {
		if (!m_Actors[i]->isAlive()) {
				if (m_Actors[i]->getID() == IID_SMALLGON || m_Actors[i]->getID() == IID_SMOREGON || m_Actors[i]->getID() == IID_SNAGGLEGON) {
					m_nShips--;
					if (m_Actors[i]->getX() > 0) {
						m_shipsDestroyed++;
					}
				}
			delete m_Actors[i];
			m_Actors.erase(m_Actors.begin() + i);
		}
	}

	//Call the doSomething() for all actors
	for (int i = 0; i < m_Actors.size(); i++) {
		m_Actors[i]->doSomething();
	}

	//Create new stars with a 1/15th probability
	if (randInt(0, 14) == 1) {
		double size = ((double)randInt(5, 50)) / 100;
		m_Actors.push_back(new Star(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), size, this));
	}

	//If player is not alive, delete it and set it to nullptr
	if (!m_player->isAlive()) {
		delete m_player;
		m_player = nullptr;
	}

	//Determine whether to Add Ships Or Not
	int remainingToBeDestroyed = (6 + (4 * getLevel())) - m_shipsDestroyed;
	int maxAliensOnScreen = 4 + (0.5 * getLevel());
	if (m_nShips < min(maxAliensOnScreen, remainingToBeDestroyed)) {
		addShip();
	}

	//Check once again if actors are alive or not, and delete if they're state is dead
	for (int i = 0; i < m_Actors.size(); i++) {
		if (!m_Actors[i]->isAlive()) {
			if (m_Actors[i]->getID() == IID_SMALLGON || m_Actors[i]->getID() == IID_SMOREGON || m_Actors[i]->getID() == IID_SNAGGLEGON) {
				m_nShips--;
				if (m_Actors[i]->getX() > 0) {
					m_shipsDestroyed++;
				}
			}
			delete m_Actors[i];
			m_Actors.erase(m_Actors.begin() + i);
		}
	}

	//Continue the Game
	return GWSTATUS_CONTINUE_GAME; 
}

//CLEANUP
////////////////////////////////////////////////////////////

void StudentWorld::cleanUp() {
	//Go through all the vectors, delete the actor, and iterate to the next pointer in the array
	for (vector<Actor*>::iterator i = m_Actors.begin(); i != m_Actors.end();) {
		delete *i;
		i = m_Actors.erase(i);
	}
}
