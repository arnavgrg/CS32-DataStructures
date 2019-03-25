// MyHash.h
#ifndef MYHASH
#define MYHASH

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash{
public:
	MyHash(double maxLoadFactor = 0.5);
	~MyHash();
	void reset();
	void associate(const KeyType& key, const ValueType& value);
	int getNumItems() const;
	double getLoadFactor() const;
	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const; 
	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key) { 
		return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
	}
	// C++11 syntax for preventing copying and assignment
	MyHash(const MyHash&) = delete;
	MyHash& operator=(const MyHash&) = delete;
private:
	struct LinkedList {
		KeyType m_key;
		ValueType m_value;
		LinkedList* m_next = nullptr;
		//Constructors
		LinkedList() { m_next = nullptr; }
		LinkedList(KeyType k, ValueType v) {
			m_key = k;
			m_value = v;
			m_next = nullptr;
		}
	};
	int m_items;
	int m_bucketCount;
	double m_loadFactor;
	LinkedList** m_hash;
	/*tsallgood adds key-value pairs to associate. 
	The function name is an abbreviation for "It's all good", something I heard all CS majors including my friends
	say when they solved a CS problem*/
	void tsallg(const KeyType& k, const ValueType& v);
};

//TSALLG
////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::tsallg(const KeyType& key, const ValueType& val) {
	//Hash function prototype
	unsigned int hash(const KeyType& k);
	//Setting h to bucket in our hash table using hash function
	unsigned int h = hash(key) % m_bucketCount;
	//If empty bucket, create new node and store it
	if (m_hash[h] == nullptr) {
		m_hash[h] = new LinkedList(key, val);
	}
	else {
		//Create a node pointer pointing to the bucket in the hash
		LinkedList* currentNode = m_hash[h];
		//Create a new node and store its pointer in the node pointer created above
		m_hash[h] = new LinkedList(key, val);
		//Move to the next node
		m_hash[h]->m_next = currentNode;
	}
}

//Constructor
////////////////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
MyHash<KeyType,ValueType>::MyHash(double maxLoadFactor) {
	//If negative or 0, maxLoadFactor = 0.5
	if (maxLoadFactor <= 0.0) {m_loadFactor = 0.5;}
	//if greater than 2, then maxLoadFactor = 2.0
	else if (maxLoadFactor > 2.0) {m_loadFactor = 2.0;}
	//Else update to the value passed in
	else {m_loadFactor = maxLoadFactor;}	
	//Initializing number of items to zero
	m_items = 0;
	//Initialize number of buckets to 100
	m_bucketCount = 100;
	//Initialize hash table with 100 buckets and set each object to nullptr
	m_hash = new LinkedList*[100];
	for (int i = 0; i < 100; i++) {
		m_hash[i] = nullptr;
	}
}

//Destructor
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash() {
	//Traverse through all the buckets in the hash table
	for (int i = 0; i < m_bucketCount; i++) {
		//Create a pointer p to the first LinkedList object in m_hash
		LinkedList *p = m_hash[i];
		//if p is a nullptr, do nothing
		if (p == nullptr) { continue; }
		//Otherwise Traverse through the linked list and delete all the nodes at that bucket
		while (p->m_next != nullptr) {
			LinkedList *curr = p;
			p = p->m_next;
			delete curr;
		}
		//Delete the last node
		delete p;
	}
	//Delete the hash table array
	delete[] m_hash;
}

//Reset
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset() {
	//Traverse through all the buckets in the hash table
	for (int i = 0; i < m_bucketCount; i++) {
		//Create a pointer p to the first LinkedList object in m_hash
		LinkedList *p = m_hash[i];
		//if p is a nullptr, do nothing
		if (p == nullptr) { continue; }
		//Traverse through the linked list and delete all the nodes
		while (p->m_next != nullptr) {
			LinkedList *curr = p;
			p = p->m_next;
			delete curr;
		}
		//Delete the last node
		delete p;
	}
	//Delete the hashtable array
	delete[] m_hash;
	//reinitialize the member variables m_bucketCount to 100 and items to 0
	m_bucketCount = 100;
	m_items = 0;
	//create a new hash table with 100 buckets and set each object to nullptr
	m_hash = new LinkedList*[m_bucketCount];
	for (int i = 0; i < m_bucketCount; i++) {
		m_hash[i] = nullptr;
	}
}

//Associate
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
	//See if the key already exists and update the value if it does
	if (find(key) != nullptr) { *find(key) = value; }
	//Otherwise, increment the number of items
	else { m_items++; }

	//Check if the current load factor is greater than the initial load factor
	if (getLoadFactor() > m_loadFactor) {
		//Assign current hash table to a temp hash table
		LinkedList** temp = m_hash;
		//Increase bucket count by 2
		m_bucketCount = m_bucketCount * 2;
		//Set current hash table to a new, bigger hash table with twice the bucket size
		m_hash = new LinkedList*[m_bucketCount];
		for (int i = 0; i < m_bucketCount; i++) {
			m_hash[i] = nullptr;
		}
		
		//For every item in the temp hash table, associate it to the new hash table with twice the number of buckets
		for (int i = 0; i < m_bucketCount / 2; i++) {
			//Create a new LinkedList pointer and initialize it to the first item in the hash table
			LinkedList* currentBucket = temp[i];
			//If the bucket is empty, do nothing
			if (currentBucket == nullptr) { continue; }
			//Otherwise add new item using tasallg 
			tsallg(currentBucket->m_key, currentBucket->m_value);
			//Otherwise, traverse through the rest of the linkedList
			while (currentBucket->m_next != nullptr) {
				//Set our traversal iterator to the next object 
				currentBucket = currentBucket->m_next;
				//Add the item at each point to our new hash table using tsallg
				tsallg(currentBucket->m_key, currentBucket->m_value);
			}
		}

		//Delete the temp hash table
		//Traverse through all the buckets in the temp hash table
		for (int i = 0; i < m_bucketCount / 2; i++) {
			//Create a pointer p to the first LinkedList object in temp hash table
			LinkedList *p = temp[i];
			//if p is a nullptr, do nothing
			if (p == nullptr) { continue; }
			//Traverse through the linked list and delete all the nodes
			while (p->m_next != nullptr) {
				LinkedList *curr = p;
				p = p->m_next;
				delete curr;
			}
			//Delete the last node
			delete p;
		}
		//Delete the temp hashtable array
		delete[] temp;
	}
	//Add key-value pair
	tsallg(key, value);
}

//getNumItems
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const {
	return m_items;
}

//getLoadFactor
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const {
	return ((double)(m_items))/m_bucketCount;
}

//find(const KeyType)
/////////////////////////////////////////////////////////////
template<typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const {
	//hash function prototype
	unsigned int hash(const KeyType& k);
	//pass key through hash function and determine where to place it in the hash table
	unsigned int h = hash(key) % m_bucketCount; 

	//if this bucket is empty, return nullptr
	if (m_hash[h] == nullptr) {return nullptr;}
	//If head node has the key, then return the value at headnode
	else if (m_hash[h]->m_key == key) {
		return &(m_hash[h]->m_value);
	}
	
	//Initialize an iterator starting at this bucket in the hash table
	LinkedList *it = m_hash[h];

	//While it does not reach the end of the LinkedList at this bucket
	while (it->m_next != nullptr) {
		//Set iterator to the next object
		it = it->m_next;
		//If the key at the linkedList* object matches up, return the value at this point.
		if (it->m_key == key) {return &(it->m_value);}
	}
	//Otherwise return nullptr
	return nullptr;
}

#endif // !MYHASH