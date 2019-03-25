#include "Actor.h"
#include "StudentWorld.h"
#include <vector>

//ACTOR
////////

//sets alive
void Actor::setAlive(bool isAlive) {
	m_isAlive = isAlive;
}

//checks if alive
bool Actor::isAlive() const{
	return m_isAlive;
}

//returns m_world
StudentWorld* Actor::getWorld() {
	return m_world;
}

//checks if the aliens have colided
bool Actor::hasCollided(Actor *a, Actor *b) {
	//makes sure stars don't collide
	if (a->getID() == IID_STAR || b->getID() == IID_STAR || a == b) {
		return false;
	}
	//calls euclidean_dist function
	if (euclidean_dist(a->getX(),a->getY(),b->getX(),b->getY()) < (0.75)*(a->getRadius() + b->getRadius())) {
		return true;
	}
	return false;
}

//returns the ID of the object
int Actor::getID() const {
	return m_ID;
}

//Euclidean_distance calculator
double Actor::euclidean_dist(double x1, double y1, double x2, double y2) {
	double x = x1 - x2; //calculating number to square in next step
	double y = y1 - y2;
	double dist;

	dist = pow(x, 2) + pow(y, 2); //calculating Euclidean distance
	dist = sqrt(dist);

	return dist;
}

//Virtual destructor
Actor::~Actor() {}

//STAR
///////////////////////////////////////////////

void Star::doSomething() {
	//move 1 pixel to the left
	moveTo(getX() - 1, getY());

	//check if out of screen, then setAlive(false)
	if (getX() < 0) {
		setAlive(false);
	}
}

//NACHENBLASTER
///////////////////////////////////////////////

NachenBlaster::NachenBlaster(StudentWorld *m_world)
	: Actor(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, m_world){
	//initialize private member variables
	m_hp = 50;
	m_cabbageEP = 30;
	m_torpedoCount = 0;
}

void NachenBlaster::doSomething() {
	//return if not alive
	if (!isAlive()) {
		return;
	}

	//boundary condition for cabbages
	if (m_cabbageEP < 30) {
		m_cabbageEP++;
	}

	//Check if user hits a key
	int ch;
	if (getWorld()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			if (getX() >= 6)
				moveTo(getX() - 6, getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getX() <= VIEW_WIDTH - 6)
				moveTo(getX() + 6, getY());
			break;
		case KEY_PRESS_DOWN:
			if (getY() >= 6)
				moveTo(getX(), getY() - 6);
			break;
		case KEY_PRESS_UP:
			if (getY() <= VIEW_HEIGHT - 6)
				moveTo(getX(), getY() + 6);
			break;
		case KEY_PRESS_SPACE:
			if (m_cabbageEP >= 5) {
				//push to the m_Actors vector
				getWorld()->getStartVec()->push_back(new Cabbage(getWorld(), getX()+12, getY()));
				m_cabbageEP -= 5;
				getWorld()->playSound(SOUND_PLAYER_SHOOT);
			}
			break;
		case KEY_PRESS_TAB:
			if (m_torpedoCount > 0) {
				//push to the m_Actors vector
				getWorld()->getStartVec()->push_back(new Torpedo(getWorld(), getX()+12, getY(), 0));
				m_torpedoCount--;
				getWorld()->playSound(SOUND_TORPEDO);
			}
			break; 
		default:
			break;
		}
	}

	//for loop to go through all the Actors in the vector m_Actors using getWorld() to access StudentWorld
	for (std::vector<Actor*>::iterator it = getWorld()->getBegin(); it != getWorld()->getEnd(); it++) {
		//Checks if collided
		if (hasCollided(this,*it)) {
			//stores the x and y location of the object that colided
			double explosion_x = (*it)->getX();
			double explosion_y = (*it)->getY();
			//check for collision with Turnip using the ID
			if ((*it)->getID() == IID_TURNIP) {
				//dec health by 2 and set false
				decHealth(2);
				(*it)->setAlive(false);
				//if health is less than 0, decrease lives and set false
				if (m_hp <= 0) {
					getWorld()->decLives();
					setAlive(false);
					return;
				}
				//play sound
				getWorld()->playSound(SOUND_BLAST);
			}
			//check for collision with Torpedo using the ID
			if ((*it)->getID() == IID_TORPEDO) {
				//dec health by 8 and set false
				decHealth(8);
				(*it)->setAlive(false);
				//if health is less than 0, decrease lives and set false
				if (m_hp <= 0) {
					getWorld()->decLives();
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
			}
			//check for collision with Smallgon using the ID
			if ((*it)->getID() == IID_SMALLGON) {
				//decrease health of NachenBlaster and increaseScore
				decHealth(5);
				getWorld()->increaseScore(250);
				(*it)->setAlive(false);
				//Create an explosion
				getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, explosion_x, explosion_y, getWorld()));
				//If health less than 0, decrease lives and setAlive(false)
				if (m_hp <= 0) {
					getWorld()->decLives();
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_DEATH);
			}
			//check for collision with Smoregon using the ID
			if ((*it)->getID() == IID_SMOREGON) {
				//decrease health of NachenBlaster and increaseScore
				decHealth(5);
				getWorld()->increaseScore(250);
				(*it)->setAlive(false);
				getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, explosion_x, explosion_y, getWorld()));
				if (m_hp <= 0) {
					getWorld()->decLives();
					setAlive(false);
					if (randInt(1, 3) == 1) {
						int temp = randInt(1, 2);
						if (temp == 1) {
							getWorld()->getStartVec()->push_back(new TorpedoGoodie(getWorld(), explosion_x, explosion_y));
						}
						if (temp == 2) {
							getWorld()->getStartVec()->push_back(new RepairGoodie(getWorld(), explosion_x, explosion_y));
						}
					}
					return;
				}
				getWorld()->playSound(SOUND_DEATH);
			}
			//check for collision with Snagglegon using the ID
			if ((*it)->getID() == IID_SNAGGLEGON) {
				decHealth(15);
				getWorld()->increaseScore(1000);
				(*it)->setAlive(false);
				getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, explosion_x, explosion_y, getWorld()));
				if (m_hp <= 0) {
					getWorld()->decLives();
					setAlive(false);
					if (randInt(1, 6) == 1) {
						getWorld()->getStartVec()->push_back(new LifeGoodie(getWorld(), explosion_x, explosion_y));
					}
					return;
				}
				getWorld()->playSound(SOUND_DEATH);
			}
			//Check for collision with Life Goodie
			if ((*it)->getID() == IID_LIFE_GOODIE) {
				//Increase lives, increase score and mark as dead
				getWorld()->incLives();
				getWorld()->increaseScore(100);
				(*it)->setAlive(false);
				getWorld()->playSound(SOUND_GOODIE);
				return;
			}
			//Check for collision with Repair Goodie
			if ((*it)->getID() == IID_REPAIR_GOODIE) {
				//increase health and increase score
				if (m_hp > 40) {m_hp = 50;}
				else {m_hp += 10;}
				getWorld()->increaseScore(100);
				(*it)->setAlive(false);
				getWorld()->playSound(SOUND_GOODIE);
				return;
			}
			//Check for collision with Torpedo Goodie
			if ((*it)->getID() == IID_TORPEDO_GOODIE) {
				//increase score and torpedo count
				getWorld()->increaseScore(100);
				m_torpedoCount += 5;
				(*it)->setAlive(false);
				getWorld()->playSound(SOUND_GOODIE);
				return;
			}
		}
	}
}

//return the health
int NachenBlaster::getHealth() const {
	return m_hp;
}

//decrease health
void NachenBlaster::decHealth(int m) {
	m_hp -= m;
}

//return CabbageCount
int NachenBlaster::getCabbageCount() {
	return m_cabbageEP;
}

//return torpedoCount
int NachenBlaster::getTorpedoCount() {
	return m_torpedoCount;
}

//PROJECTILE
////////////////////////////////////////////////

Projectile::Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld* world) 
	: Actor(imageID, startX, startY, dir, 0.5, 1, world) {
}

//CABBAGE
////////////////////////////////////////////////

Cabbage::Cabbage(StudentWorld* world, double startX, double startY) 
	: Projectile(IID_CABBAGE, startX, startY, 0, world) {
}

//Cabbage's do something
void Cabbage::doSomething() {
	//if not alive, return
	if (!isAlive()) {
		return;
	}

	//move 8 pixels to the right
	moveTo(getX() + 8, getY());
	//Create rotation
	setDirection(getDirection() + 20);
	//Check boundary condition. If true, set to dead.
	if (getX() >= VIEW_WIDTH) {
		setAlive(false);
	}
}

//TURNIP
////////////////////////////////////////////////

Turnip::Turnip(StudentWorld* world, double startX, double startY)
	: Projectile(IID_TURNIP, startX, startY, 0, world) {

}

//Turnip's Do Something function
void Turnip::doSomething() {
	//Check if alive. If not alive, return
	if (!isAlive()) {
		return;
	}
	
	//Move 6 pixels to the left
	moveTo(getX() - 6, getY());
	//Rotation by 20 degrees
	setDirection(getDirection() + 20);
	//check boundary condition, return false is true.
	if (getX() <= 0) {
		setAlive(false);
	}
}

//TORPEDO
/////////////////////////////////////////////////

Torpedo::Torpedo(StudentWorld* world, double startX, double startY, Direction dir)
	: Projectile(IID_TORPEDO, startX, startY, dir, world) {
	m_dir = dir; //initialize direction
}

//Torpedo's do something
void Torpedo::doSomething() {
	//Check if alive, otherwise return false
	if (!isAlive()) {
		return;
	}
	
	//If dir == 0, move to the right by 8. Check boundary condition.
	if (m_dir == 0) {
		moveTo(getX() + 8, getY());
		if (getX() >= VIEW_WIDTH - 1) {
			setAlive(false);
		}
	}
	//If dir == 180, move to the left by 8. Check boundary condition.
	else if (m_dir == 180) {
		moveTo(getX() - 8, getY());
		if (getX() <= 0) {
			setAlive(false);
		}
	}
	
	//Safety Check
	if (m_dir == 0 && getX() >= VIEW_WIDTH) {
		setAlive(false);
	}

	if (m_dir == 180 && getX() <= 0) {
		setAlive(false);
	}
}

//ALIEN
//////////////////////////////////////////////////////

Alien::Alien(int imageID, double startX, double startY, double deltaX, double deltaY, double speed, double flightplan_length, StudentWorld *world)
	: Actor(imageID, startX, startY, 0, 1.5, 1, world) {
	//Initialize private member variables.
	m_deltaX = deltaX;
	m_deltaY = deltaY;
	m_speed = speed;
	m_flightplan_length = flightplan_length;
	m_health = 5 * (1 + (getWorld()->getLevel() - 1)*(0.1));
}

//get the Alien's health
double Alien::getHealth() const {
	return m_health;
}

//Check if within the screen/boundary
bool Alien::isInBoundary() const {
	if (getX() < 0)
		return false;
	return true;
}

//Change direction as determined by flight plan
void Alien::changeDirection(int m) {
	dir = m;
}

//Fire for alien. Runs through the probability.
void Alien::fire() {
	int level = getWorld()->getLevel();
	if ((randInt(1, (20 / level) + 5)) == 1) {
		//Create new turnip
		getWorld()->getStartVec()->push_back(new Turnip(getWorld(), getX() - 14, getY()));
		getWorld()->playSound(SOUND_ALIEN_SHOOT);
	}
}

//Decrease flight length as neeeded
void Alien::decFlightLength() {
	m_flightplan_length -= 1;
}

//Set Flight Length as needed
void Alien::setFlightLength(int m) {
	m_flightplan_length = m;
}

//Get flight length
double Alien::getFlightLength() const {
	return m_flightplan_length;
}

//Get Dir (either left, up and left, or down and left)
int Alien::getDir() const {
	return dir;
}

//return the speed
double Alien::getSpeed() const {
	return m_speed;
}

//set the speed
void Alien::setSpeed(int speed){
	m_speed = speed;
}

//decrease the alien's health
void Alien::decHealth(int m) {
	m_health -= m;
}

//SMALLGON
//////////////////////////////////////////////////////

Smallgon::Smallgon(StudentWorld* world, double startX, double startY)
	: Alien(IID_SMALLGON, startX, startY, 0, 0, 2.0, 0, world) {

}

//Smallgon's doSomething()
void Smallgon::doSomething() {
	//Check if alive, else return false.
	if (!isAlive()) {
		return;
	}

	//if Health is less than 0, set to dead
	if (getHealth() <= 0) {
		setAlive(false);
	}

	//if not within boundary, set to dead
	if (!isInBoundary()) {
		setAlive(false);
	}

	//if flight length is 0, change to a random direction and change flight length to anything between 1 & 32.
	if (getFlightLength() == 0) {
		changeDirection(randInt(1,3));
		setFlightLength(randInt(1, 32));
	}

	//Change direction is y <= 0
	if (getY() <= 0) {
		changeDirection(2);
	}

	//Change direction if y >= width
	if (getY() >= VIEW_WIDTH) {
		changeDirection(3);
	}

	//Depending on the direction number, it moves accordingly and decreases flight length by 1.
	if (getDir() == 1) {
		moveTo(getX() - getSpeed(), getY());
		decFlightLength();
	}
	if (getDir() == 2) {
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		decFlightLength();
	}
	if (getDir() == 3) {
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		decFlightLength();
	}

	//Calculate probability of firing things, and then calls the function is the probability is satisfied
	if ((getWorld()->getPlayer()->getX() < getX())) {
		if (getWorld()->getPlayer()->getY() >= getY() - 4 && getWorld()->getPlayer()->getY() <= getY() + 4) {
			fire();
		}
	}

	//Go through all the actors
	for (std::vector<Actor*>::iterator it = getWorld()->getBegin(); it != getWorld()->getEnd(); it++) {
		//If it colides 
		if (hasCollided(this, *it)) {
			//Collides with NachenBlaster's Cabbage
			if ((*it)->getID() == IID_CABBAGE) {
				//Decrease health and set to false
				decHealth(2);
				(*it)->setAlive(false);
				if (getHealth() <= 0) {
					//Increase score, and create an explosion.
					getWorld()->increaseScore(250);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
				//break out of the function
				return;
			}
			//Collides with NachenBlaster's torpedo
			if ((*it)->getID() == IID_TORPEDO) {
				//Decrease health
				decHealth(8);
				(*it)->setAlive(false);
				//If health is less than 0
				if (getHealth() <= 0) {
					//Increase health and create explosion
					getWorld()->increaseScore(250);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}

	}
}
 
//SMOREGON
//////////////////////////////////////////////////////

Smoregon::Smoregon(StudentWorld* world, double startX, double startY)
	: Alien(IID_SMOREGON, startX, startY, 0, 0, 2.0, 0, world) {

}

//Smoregon's doSomething
void Smoregon::doSomething() {
	//If not alive, return
	if (!isAlive()) {
		return;
	}

	//If not in boundary, set state to dead
	if (!isInBoundary()) {
		setAlive(false);
	}

	//If health is lesss than 0, set state to dead
	if (getHealth() <= 0) {
		setAlive(false);
	}

	//Update flight length to 1/3 possibilities, 
	if (getFlightLength() == 0) {
		changeDirection(randInt(1, 3));
		setFlightLength(randInt(1, 32));
	}

	//Change direction if Y <= 0
	if (getY() <= 0) {
		changeDirection(2);
	}

	//Change directio nif Y >= End of right side of the screen
	if (getY() >= VIEW_WIDTH) {
		changeDirection(3);
	}

	//Change direction according to number (left, up and left, down and left). Decrease flight length by 1.
	if (getDir() == 1) {
		moveTo(getX() - getSpeed(), getY());
		decFlightLength();
	}
	
	if (getDir() == 2) {
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		decFlightLength();
	}
	
	if (getDir() == 3) {
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		decFlightLength();
	}

	//Check if the alien is within the range of the NachenBlaster, then call the fire() function
	if ((getWorld()->getPlayer()->getX() < getX())) {
		if (getWorld()->getPlayer()->getY() >= getY() - 4 && getWorld()->getPlayer()->getY() <= getY() + 4) {
			fire();
		}
	}

	//Calculate the probability that the Smoregon will speed through the screen
	int m = randInt(1, (20/(getWorld()->getLevel()))+5);
	if (m == 1) {
		setSpeed(5);
		setFlightLength(VIEW_WIDTH);
		changeDirection(1);
	}

	//Go through all the items in the m_Actors vector
	for (std::vector<Actor*>::iterator it = getWorld()->getBegin(); it != getWorld()->getEnd(); it++) {
		//Check if collided
		if (hasCollided(this, *it)) {
			//If cabbage, decrease health by 2 and set Cabbage's state to death
			if ((*it)->getID() == IID_CABBAGE) {
				decHealth(2);
				(*it)->setAlive(false);
				//If health < 0, increase score and create Explosion.
				if (getHealth() <= 0) {
					getWorld()->increaseScore(250);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					//Check the probability of dropping the goodie
					if (randInt(1, 3) == 1) {
						int temp = randInt(1, 2);
						if (temp == 1) {
							//Drop a torpedo Goodie
							getWorld()->getStartVec()->push_back(new TorpedoGoodie(getWorld(), getX(), getY()));
						}
						if (temp == 2) {
							//Drop a Repair Goodie
							getWorld()->getStartVec()->push_back(new RepairGoodie(getWorld(), getX(), getY()));
						}
					}
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
			}
			//Check if collision with Torpedo
			if ((*it)->getID() == IID_TORPEDO) {
				decHealth(8);
				(*it)->setAlive(false);
				//If dead, increase NachenBlaster's score and create an explosion
				if (getHealth() <= 0) {
					getWorld()->increaseScore(250);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					//Check probability to drop a goodie
					if (randInt(1, 3) == 1) {
						int temp = randInt(1, 2);
						if (temp == 1) {
							//Drops a TorpedoGoodie
							getWorld()->getStartVec()->push_back(new TorpedoGoodie(getWorld(), getX(), getY()));
						}
						if (temp == 2) {
							//Drops a RepairGoodie
							getWorld()->getStartVec()->push_back(new RepairGoodie(getWorld(), getX(), getY()));
						}
					}
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
			}
		}

	}
}

//SNAGGLEGON
//////////////////////////////////////////////////////

Snagglegon::Snagglegon(StudentWorld* world, double startX, double startY)
	: Alien(IID_SNAGGLEGON, startX, startY, 0, 0, 1.75, 0, world) { //A Snagglegon starts with an initial travel direction of down and left.
	changeDirection(3);
}

//SnaggleGon's doSomething()
void Snagglegon::doSomething() {
	//If not alive, return
	if (!isAlive()){
		return;
	}

	//If not in boundary, set state to dead
	if (!isInBoundary()) {
		setAlive(false);
	}

	//If health less than 0, set state to dead
	if (getHealth() <= 0) {
		setAlive(false);
	}

	//If Y<=0, set Direction to 2
	if (getY() <= 0) {
		changeDirection(2);
	}
	
	//If Y >= end of the screen, dir = 3
	if (getY() >= VIEW_WIDTH) {
		changeDirection(3);
	}
	
	//Change direction according to number. Decrease flight length by 1.
	if (getDir() == 2) {
		moveTo(getX() - getSpeed(), getY() + getSpeed());
		decFlightLength();
	}
	if (getDir() == 3) {
		moveTo(getX() - getSpeed(), getY() - getSpeed());
		decFlightLength();
	}

	if ((getWorld()->getPlayer()->getX() < getX())) {
		if (getWorld()->getPlayer()->getY() >= getY() - 4 && getWorld()->getPlayer()->getY() <= getY()+4) {
			fire();
		}
	}

	//Go through the actor array using iterators
	for (std::vector<Actor*>::iterator it = getWorld()->getBegin(); it != getWorld()->getEnd(); it++) {
		//If collision
		if (hasCollided(this, *it)) {
			//Check collision with Cabbage. 
			if ((*it)->getID() == IID_CABBAGE) {
				//Decrease health and set state to dead
				decHealth(2);
				(*it)->setAlive(false);
				if (getHealth() <= 0) {
					//Increase score and create explosion
					getWorld()->increaseScore(1000);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					//If probability matches, create a LifeGoodie
					if (randInt(1, 6) == 1) {
						getWorld()->getStartVec()->push_back(new LifeGoodie(getWorld(), getX(), getY()));
					}
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
			}
			//Check collision with Torpedo using the same process as above
			if ((*it)->getID() == IID_TORPEDO) {
				decHealth(8);
				(*it)->setAlive(false);
				if (getHealth() <= 0) {
					getWorld()->increaseScore(1000);
					getWorld()->getStartVec()->push_back(new Explosion(IID_EXPLOSION, getX(), getY(), getWorld()));
					getWorld()->playSound(SOUND_DEATH);
					if (randInt(1, 6) == 1) {
						getWorld()->getStartVec()->push_back(new LifeGoodie(getWorld(), getX(), getY()));
					}
					setAlive(false);
					return;
				}
				getWorld()->playSound(SOUND_BLAST);
			}
		}
	}
}

//Return Snagglegon's health
void Snagglegon::health() {
	m_health = 10 * (1 + (getWorld()->getLevel() - 1)*(0.1));
}

//getHealth()
double Snagglegon::getHealth() {
	return m_health;
}

//Fire using the probability given()
void Snagglegon::fire() {
	int level = getWorld()->getLevel();
	if ((randInt(1, (15/level) + 10)) == 1) {
		//Create new torpedo
		getWorld()->getStartVec()->push_back(new Torpedo(getWorld(), getX() - 14, getY(), 180));
		getWorld()->playSound(SOUND_TORPEDO);
	}
}

//EXPLOSION
/////////////////////////////////////////////////////

Explosion::Explosion(int imageID, double startX, double startY, StudentWorld* world)
	: Actor(IID_EXPLOSION, startX, startY, 0, 1, 0, world) {
	m_Ticks = 0;
}

//Explosion's do Something
void Explosion::doSomething() {
	//If not alive, return
	if (!isAlive()) {
		return;
	}
	//Increment Ticks
	m_Ticks++;
	//Increase size
	double size = getSize();
	size *= 1.5;
	setSize(size);
	//If ticks is 4, break out
	if (m_Ticks == 4) {
		setAlive(false);
	}
}

//GOODIES
/////////////////////////////////////

Goodies::Goodies(int imageID, double startX, double startY, StudentWorld *world)
	: Actor(imageID, startX, startY, 0, 0.5, 1, world) {

}

//Goodies doSomething()
void Goodies::doSomething() {
	//If not alive, return
	if (!isAlive()) {
		return;
	}

	//Move down and left by 0.75 pixel
	moveTo(getX() - 0.75, getY() - 0.75);

	//Check bottom and left side of screen. Set False if out of bounds.
	if (getX() <= 0 || getY() <= 0) {
		setAlive(false);
	}
}

//LIFE GOODIE
/////////////////////////////////////

LifeGoodie::LifeGoodie(StudentWorld *world, double startX, double startY)
	: Goodies(IID_LIFE_GOODIE, startX, startY, world) {

}

//REPAIR GOODIE
/////////////////////////////////////

RepairGoodie::RepairGoodie(StudentWorld *world, double startX, double startY)
	: Goodies(IID_REPAIR_GOODIE, startX, startY, world) {

}

//TORPEDO GOODIE
/////////////////////////////////////

TorpedoGoodie::TorpedoGoodie(StudentWorld *world, double startX, double startY)
	: Goodies(IID_TORPEDO_GOODIE, startX, startY, world) {

}
