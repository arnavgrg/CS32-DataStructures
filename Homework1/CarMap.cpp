#include "CarMap.h"
#include <iostream>
using namespace std;


// Create an empty car map.
CarMap::CarMap(){}


// If a car with the given license plate is not currently in the map, 
// and there is room in the map, add an entry for that car recording
// that it has 0 gallons of gas in it, and return true.  Otherwise,
// make no change to the map and return false.
bool CarMap::addCar(std::string license) {
	if (!carData.contains(license)) {
		carData.insert(license, 0);
		return true;
	}
	return false;
}


// If a car with the given license plate is in the map, return the
// number of gallons of gas in its tank; otherwise, return -1.
double CarMap::gas(std::string license) const {
	ValueType gallons;
	if (carData.get(license, gallons)) {
		return gallons;
	}
	else {
		return -1;
	}
}


// If no car with the given license plate is in the map or if
// gallons is negative, make no change to the map and return
// false.  Otherwise, increase the number of gallons of gas in the
// indicated car by the gallons parameter and return true.
bool CarMap::addGas(std::string license, double gallons) {
	if (gallons < 0) {
		return false;
	}
	if (!carData.contains(license)) {
		return false;
	}
	ValueType gallon;
	carData.get(license, gallon);
	carData.update(license, (gallon + gallons));
	return true;
}


// If no car with the given license plate is in the map or if
// gallons is negative or if gallons > gas(), make no change to the
// map and return false.  Otherwise, decrease the number of gallons
// of gas in the indicated car by the gallons parameter and return
// true.
bool CarMap::useGas(std::string license, double gallons) {
	if (gallons < 0) {
		return false;
	}
	if (gallons > gas(license)) {
		return false;
	}
	if (!carData.contains(license)) {
		return false;
	}
	ValueType gallon;
	carData.get(license, gallon);
	carData.update(license, (gallon - gallons));
	return true;
}


// Return the number of cars in the CarMap.
int CarMap::fleetSize() const {
	return carData.size();
}

// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of gallons in that car's tank.  Write
// no other text.
void CarMap::print() const {
	KeyType key;
	ValueType value;
	for (int i = 0; i < fleetSize(); i++) {
		carData.get(i, key, value);
		cout << key << " " << value << endl;
	}
}
