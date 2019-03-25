#include "Map.h"
#include <iostream>
using namespace std;

// Create an empty map (i.e., one with no key/value pairs)
Map::Map() {
	m_mapIndex = 0;
}


// Return true if the map is empty, otherwise false.
bool Map::empty() const {
	if (m_mapIndex > 0) {
		return false;
	}
	else {
		return true;
	}
}


// Return the number of key/value pairs in the map.
int Map::size() const {
	return m_mapIndex;
}


// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
bool Map::insert(const KeyType& key, const ValueType& value) {
	if (m_mapIndex == DEFAULT_MAX_ITEMS) {
		return false;
	}
	for (int i = 0; i < m_mapIndex; i++) {
		if (m_map[i].keyString == key) {
			return false;
		}
	}
	m_map[m_mapIndex].keyString = key;
	m_map[m_mapIndex].keyValue = value;
	m_mapIndex++;
	return true;
}


// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
bool Map::update(const KeyType& key, const ValueType& value) {
	for (int i = 0; i < m_mapIndex; i++) {
		if (m_map[i].keyString == key) {
			m_map[i].keyValue = value;
			return true;
		}
	}
	return false;
}


// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (!update(key, value)) {
		if (insert(key, value)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return true;
	}
}


// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
bool Map::erase(const KeyType& key) {
	for (int i = 0; i < m_mapIndex; i++) {
		if (m_map[i].keyString == key) {
			while (i < m_mapIndex) {
				m_map[i].keyString = m_map[i + 1].keyString;
				m_map[i].keyValue = m_map[i + 1].keyValue;
				i++;
			}
			m_mapIndex--;
			return true;
		}
	}
	return false;
}


// Return true if key is equal to a key currently in the map, otherwise
// false.
bool Map::contains(const KeyType& key) const {
	for (int i = 0; i < m_mapIndex; i++) {
		if (m_map[i].keyString == key) {
			return true;
		}
	}
	return false;
}


// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
bool Map::get(const KeyType& key, ValueType& value) const {
	for (int i = 0; i < m_mapIndex; i++) {
		if (key == m_map[i].keyString) {
			value = m_map[i].keyValue;
			return true;
		}
	}
	return false;
}


// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
bool Map::get(int i, KeyType& key, ValueType& value) const {
	if (i >= 0 && i < m_mapIndex) {
		key = m_map[i].keyString;
		value = m_map[i].keyValue;
		return true;
	}
	return false;
}

// Exchange the contents of this map with the other one.
void Map::swap(Map& other) {
	mapData temp;
	for (int i = 0; i < m_mapIndex && i < other.m_mapIndex; i++) {
		temp = other.m_map[i];
		other.m_map[i] = m_map[i];
		m_map[i] = temp;
	}

	if (m_mapIndex < other.m_mapIndex) {
		for (int i = m_mapIndex; i < other.m_mapIndex; i++) {
			m_map[i] = other.m_map[i];
		}
	}

	if (m_mapIndex > other.m_mapIndex) {
		for (int i = other.m_mapIndex; i < m_mapIndex; i++) {
			other.m_map[i] = m_map[i];
		}
	}

	int temp_mapIndex = m_mapIndex;
	m_mapIndex = other.m_mapIndex;
	other.m_mapIndex = temp_mapIndex;
}