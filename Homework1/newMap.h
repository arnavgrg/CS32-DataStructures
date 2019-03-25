#ifndef  NEWMAP_H
#define  NEWMAP_H

#include <string>

const int DEFAULT_MAX_ITEMS = 250;

//Type Alias
using KeyType = std::string;
using ValueType = double;

//Map Class
class Map
{
public:
	Map();
	Map(int value);
	~Map(); //destructor
	Map(const Map& oldMap); //copy constructor
	Map& operator=(const Map &old); //assignment operator
	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);
private:
	struct mapData {
		KeyType keyString;
		ValueType keyValue;
	};
	mapData	*m_map;
	int m_mapIndex;
	int m_size;
};

#endif