#ifndef  MAP_H
#define  MAP_H

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
	mapData m_map[DEFAULT_MAX_ITEMS];
	int m_mapIndex;
};

#endif // ! MAP_H