#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	//all functions except swap
	Map m;
	assert(m.empty());
	ValueType v = -1234.5;
	assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
	m.insert("xyz", 9876.5);
	assert(m.size() == 1);
	assert(m.contains("xyz"));
	m.update("xyz", 1000.0);
	KeyType k = "hello";
	assert(m.get(0, k, v) && k == "xyz"  &&  v == 1000.0);
	m.insertOrUpdate("xyz", 1001.0);
	assert(m.get("xyz", v) && v == 1001.0);
	m.insertOrUpdate("abc", 500.0);
	assert(m.contains("abc") && m.contains("xyz") && m.size() == 2);
	m.erase("abc");
	m.erase("xyz");
	assert(!m.get("abc", v) && !m.get("xyz", v) && !m.contains("abc") && !m.contains("xyz") && m.size() == 0 && m.empty());

	//three argument get function
	Map m1;
	m1.insert("A", 10);
	m1.insert("B", 44);
	m1.insert("C", 10);
	KeyType all;
	ValueType total = 0;
	for (int n = 0; n < m1.size(); n++) {
		KeyType k;
		ValueType v;
		m1.get(n, k, v);
		all += k;
		total += v;
	}
	m1.erase("C");
	m1.erase("B");
	m1.erase("A");
	assert(m1.size() == 0 && !m1.get("C", total) && !m1.contains("C"));
	cout << all << total << endl;

	//three argument get function
	Map gpas;
	gpas.insert("Fred", 2.956);
	gpas.insert("Ethel", 3.538);
	ValueType val;
	KeyType k1;
	assert(gpas.get(1, k1, val) && (k1 == "Fred" || k1 == "Ethel"));
	string k2;
	assert(gpas.get(1, k2, val) && k2 == k1);

	//works for empty strings
	Map gpas1;
	gpas1.insert("Fred", 2.956);
	assert(!gpas1.contains(""));
	gpas1.insert("Ethel", 3.538);
	gpas1.insert("", 4.000);
	gpas1.insert("Lucy", 2.956);
	assert(gpas1.contains(""));
	gpas1.erase("Fred");
	assert(gpas1.size() == 3 && gpas1.contains("Lucy") && gpas1.contains("Ethel") &&
		gpas1.contains(""));

	//swap
	Map v1;
	v1.insert("Fred", 2.956);
	Map m2;
	m2.insert("Ethel", 3.538);
	m2.insert("Lucy", 2.956);
	v1.swap(m2);
	assert(v1.size() == 2 && v1.contains("Ethel") && v1.contains("Lucy") &&
		m2.size() == 1 && m2.contains("Fred"));

	//test for operator= function and copy constructor
	Map try1;
	try1.insert("Harshitha", 1);
	try1.insert("Ifteda", 2);
	try1.insert("Frank", 3);
	Map try3 = try1;
	assert(try3.contains("Harshitha") && try3.contains("Ifteda") && try3.contains("Frank"));
	Map try2;
	try2.insert("Deva", 9);
	try2.insert("Kriti", 8);
	try1 = try2;
	assert(try1.contains("Deva") && try1.contains("Kriti"));

	Map a(1000);   // a can hold at most 1000 key/value pairs
	Map b(5);      // b can hold at most 5 key/value pairs
	Map c;         // c can hold at most DEFAULT_MAX_ITEMS key/value pairs
	KeyType k10[6] = {"a", "b", "c", "d", "e", "f"};
	ValueType v10 = 10;

	// No failures inserting pairs with 5 distinct keys into b
	for (int n = 0; n < 5; n++) {
		assert(b.insert(k10[n], v10));
	}

	// Failure if we try to insert a pair with a sixth distinct key into b
	assert(!b.insert(k10[5], v10));

	// When two Maps' contents are swapped, their capacities are swapped
	// as well:
	a.swap(b);
	assert(!a.insert(k10[5], v10) && b.insert(k10[5], v10));

	cout << "Passed all tests" << endl;
}