#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
	CarMap c1;
	c1.addCar("Merc");
	c1.addCar("Porsche");
	assert(c1.fleetSize() == 2);
	assert(c1.gas("Merc") == 0 && c1.gas("Porsche") == 0);
	c1.addGas("Merc", 100);
	assert(c1.gas("Merc") == 100);
	c1.useGas("Merc", 75);
	assert(c1.gas("Merc") == 25);
	c1.addGas("Porsche", 50);
	assert(c1.gas("Porsche") == 50);
	c1.print();
}
