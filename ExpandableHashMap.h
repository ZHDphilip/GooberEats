/*
Project4 UCLA CS32
© 03/15/2020 by @zihaoDONG
ALL RIGHTS RESERVED
File: ExpandableHashMap.h
*/

#ifndef HASHMAP_INCLUDED
#define HASHMAP_INCLUDED
//#include <iostream>
//#include <string>
//#include <stdio.h>
//#include <functional>
#include <list>

//unsigned int hash(const GeoCoord& g);

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	double m_loadFactor;
	double m_maxFactor;
	int m_numOfBuc;
	int m_numOfAssoc;

	struct Association {
		KeyType Key;
		ValueType Value;
	};

	std::list<Association*>* m_list;

	unsigned int getBucNum(const KeyType& key) const{
		unsigned int hasher(const KeyType& Key);
		return hasher(key) % m_numOfBuc;
	}

	void cleanUp() {
		for (int i = 0;i < m_numOfBuc;i++) {
			//typename std::list<Association*>::iterator p = m_list[i].begin();
			for (typename std::list<Association*>::iterator p = m_list[i].begin();p != m_list[i].end();p++) {
				delete* p;
			}
		}
	}
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	m_maxFactor = maximumLoadFactor;
	m_loadFactor = 0;
	m_numOfBuc = 8;
	m_numOfAssoc = 0;
	m_list = new std::list<Association*>[m_numOfBuc];
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	cleanUp();
	delete[]m_list;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	cleanUp();
	//delete[]m_list;
	m_numOfBuc = 8;
	m_numOfAssoc = 0;
	m_loadFactor = 0;
	m_list = new std::list<Association*>[m_numOfBuc];
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return m_numOfAssoc;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	if (m_loadFactor > m_maxFactor) {	//if the load factor exceeds the maximum, meaning we have to resize.
		m_numOfBuc *= 2;
		std::list<Association*>* temp = new std::list<Association*>[m_numOfBuc];		//create a temporary array to keep the new hash table
		for (int i = 0;i < m_numOfBuc / 2;i++) {
			if (m_list[i].size() != 0) {
				typename std::list<Association*>::iterator p = m_list[i].begin();
				typename std::list<Association*>::iterator t;
				while (p != m_list[i].end()) {
					unsigned int index = getBucNum((*p)->Key);
					t = p;
					p++;
					temp[index].splice(temp[index].end(), m_list[i], t);
				}
			}
		}
		//cleanUp();
		delete[]m_list;
		m_list = temp;
	}
	ValueType* v = find(key);
	if (v != nullptr) {
		*v = value;
	}
	else {
		unsigned int index = getBucNum(key);	//put the new Association in the corresponding bucket
		Association* a = new Association;
		a->Key = key;
		a->Value = value;
		m_list[index].push_back(a);
		m_numOfAssoc++;
		m_loadFactor = m_numOfAssoc / (1.0*m_numOfBuc);
	}
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	/*Pseudocode
	Use the getBucNum function to generate the bucket the key should be putted in (Index)
	traverse the vector<Association*>
				compare each entry's Key with key to see if there is a match
					if so return a reference to its value
	if have not returned a value yet at this point, means the key does not exist in the hash table, so
	return nullptr;
	*/
	unsigned int index = getBucNum(key);
	typename std::list<Association*>::iterator p = m_list[index].begin();
	while (p != m_list[index].end()) {
		if ((*p)->Key == key)
			return &((*p)->Value);
		p++;
	}
	return nullptr;
}



#endif //HASHMAP_INCLUDED
