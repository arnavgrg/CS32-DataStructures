#ifndef CARMAP_H
#define CARMAP_H

#include "Map.h"

class CarMap
{
public:
	CarMap();
	bool addCar(std::string license);
	double gas(std::string license) const;
	bool addGas(std::string license, double gallons);
	bool useGas(std::string license, double gallons);
	int fleetSize() const; 
	void print() const;
private:
	Map carData;
};

#endif // !CARMAP_H

