#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include <iostream>
#include "LogManager.h"

// uncomment this to enable debugging
//#define HTABLE_DEBUG 1

template <class KeyType, class T>
class Hashtable;

/**
* A HashItem is a combination of a key and a value that will be stored in a
* Hashtable.  The key is an integer while the value is any type.  The class
* is a template so that the type of the value can be altered.  The value is
* copied from the constructor and stored in the HashItem.  The class destructor
* DOES NOT delete the value!
* @author R. N. Robson
*/
template <class KeyType, class T>
class HashItem
{
private:
	int key;
	KeyType keyValue;
	T value;

public:

	/**
	* Constructor to create a HashItem from a key and a value.
	* @param ke The key is a unique numeric value to identify the item.
	* @param keyVal The actual value of the key.
	* @param val The value associated with the key.
	*/
	HashItem(int ke, KeyType keyVal, T val) : key(ke), keyValue(keyVal), value(val) {}

	/**
	* A do-nothing destructor.  This will not delete the data stored in
	* the HashItem.
	*/
	~HashItem(void) {}




	/**
	* Return the hash of the key for this HashItem
	* @returns the hash of the key for this HashItem
	*/
	int getKey()
	{
		return key;
	}

	/**
	* Return the true value of th key.
	* @param the value of the key.
	*/
	KeyType getKeyValue()
	{
		return keyValue;
	}

	/**
	* Retun the value associated with this HashItem
	* @returns The value of this HashItem
	*/
	T getValue()
	{
		return value;
	}

	/**
	* This sets the value of the hashitem, replacing the current value.
	* It simply overwrites the curent value, it does not attempt to delete
	* the value.
	* @param val The value to assign to the HashItem.
	*/
	void setValue(T val)
	{
		value = val;
	}

	/**
	* Return true if this HashItem is equal to the other.
	* @param other The value to compare this to
	* @returns true if the two items are equal
	*/
	bool operator==(HashItem &other)
	{
		return key == other.key;
	}

	/**
	* Return true if this HashItem is not equal to the other.
	* @param other The value to compare this to
	* @returns true if the two items are not equal
	*/
	bool operator!=(HashItem &other)
	{
		return !(*this == other);
	}

	/**
	* Assignment operator.
	* @param other The value to assign.
	* @returns this
	*/
	HashItem operator=(HashItem<KeyType, T> other)
	{
		if (this != &other)
		{
			key = other.key;
			keyValue = other.keyValue;
			value = other.value;
		}
		return *this;
	}

};

/**
* This is a simple implementation of an STL-like forward iterator for the
* Hashtable class.  The iterator will return HashItems and in general will
* behave similar to an STL forward iterator.
* @author R. N. Robson
*/
template<class KeyType, class TT>
class HashtableIterator
{
private:
	/** Pointer to the Hashtable being iterated
	*/
	Hashtable<KeyType, TT>							*htable;
	/** The bucket containing the current vector being iterated
	* This will have the value -1 if this is a past-the-end iterator.
	*/
	int										currBucket;
	/** The iterator for the current vector
	*/
	typename std::vector<   HashItem<KeyType, TT> >::iterator		currIter;

	/**
	* This searches through the hash table of vectors to find the next vector
	* which contains data and sets the currIter to be the iterator for this vector.
	* If it reaches the end of the hash table without finding any data its sets
	* currBucket to -1 to indicate that this iterator has reached the end.  To
	* use this, set currBucket to the first bucket you want it to start searching
	* to find valid data.
	*/
	void nextValidIterator()
	{
		while (currBucket < htable->tableSize)
		{
			currIter = htable->table[currBucket].begin();
			if (currIter != htable->table[currBucket].end())
				break;
			currBucket++;
		}
		if ((currBucket >= htable->tableSize) || (currIter == htable->table[currBucket].end()))
		{
			currBucket = -1;
		}
	}

public:

	/**
	* Parameterless constructor to create iterator pointing at no table.
	*/
	HashtableIterator() : htable(NULL), currBucket(-1) {}

	/**
	* Constructor to create an iterator for a hashtable.
	* @param table The Hashtable to iterate
	* @param bucket If this is -1, we create a past-the-end iterator.  Otherwise
	* the interator is positioned at the first valid data in the Hashtable.
	*/
	HashtableIterator(Hashtable<KeyType, TT> &table, int bucket)
	{
		htable = &table;
		if (bucket == -1)
		{
			// this is the indicator for a past-the-end iterator so we set the
			// current bucket to this value
			currBucket = bucket;
		}
		else
		{
			// any bucket which is not pass-the-end is assumed to mean that we should
			// position the iterator at the first valid member of the Hashtable
			// This starts searching at the first bucket and retrieves an iterator for
			// each vector until it finds one which is not the past-the-end iterator.
			// If it searches all buckets and does not find any data, it sets currBucket
			// to -1 to indicate that this is the past-the-end iterator.
			currBucket = 0;
			nextValidIterator();
		}
	}

	/**
	* Constructor to create an iterator referencing an existing element.
	*/
	HashtableIterator(Hashtable<KeyType, TT> &table, int bucket, typename std::vector<HashItem<KeyType, TT>>::iterator& iter) :
		currBucket(bucket), currIter(iter)
	{
		htable = &table;
	}

	HashtableIterator& operator=(HashtableIterator &other)
	{
		htable = other.htable;
		currBucket = other.currBucket;
		currIter = other.currIter;

		return *this;
	}

	bool operator==(HashtableIterator &other)
	{
		bool result = false;

		if (htable == other.htable)
		{
			if (currBucket == other.currBucket)
			{
				if (currBucket == -1) result = true;
				else result = currIter == other.currIter;
			}
		}

		return result;
	}

	bool operator!=(HashtableIterator &other)
	{
		return !this->operator==(other);
	}

	/**
	* Postfix increment operator to move iterator on to next element.
	*/
	HashtableIterator& operator++(int)
	{
		if (currBucket >= 0)
		{
			if (currIter != htable->table[currBucket].end())
			{
				currIter++;
			}
			if (currIter == htable->table[currBucket].end())
			{
				currBucket++;
				nextValidIterator();
			}
		}
		return *this;
	}

	HashItem<KeyType, TT>& operator*()
	{
		return *currIter;
	}
};

/**
* This is a fixed size hashtable.  The entries in the hashtable are of type HashItem.
* The size of the hashtable is fixed when it is first created.  This means that determining
* the size of the hashtable in advance will determine the performance of the hashtable.
* In general, you should select a size about 3 times the number of entries anticipated.
* When the hashtable is deleted, the values in the hashtable ARE NOT deleted.
* @author R. N. Robson
*/
template <class KeyType, class T>
class Hashtable
{
private:
	std::vector<HashItem<KeyType, T>>		*duplicateTable;
	int										tableSize;
	std::vector<HashItem<KeyType, T>>		*table;

	friend HashtableIterator<KeyType, T>;

	/**
	* Hash function for integers which transforms a key into a value from 0 to
	* tableSize - 1.
	* @param the key to hash
	* @returns a value from 0 to tableSize -1
	*/
	int hash(int n)
	{
		int result = n % tableSize;
		if (result < 0) result *= -1;
		return result;
	}

	/**
	* Hash function for strings which transforms a string into a value from 0 to
	* tableSize - 1.
	* @param the key to hash
	* @returns a value from 0 to tableSize -1
	*/
	int hash(std::string &s)
	{
		int result = 0;
		int i;

		for (i = 0; i < s.size(); i++)
		{
			int v = (int)s[i];
			result = (result * 256 + v) % tableSize;
		}
		return result;
	}
public:

	typedef HashtableIterator<KeyType, T>	iterator;

	/**
	* Constructor to create an empty hashtable.  This allocates the table whose
	* size is determined by the parameter passed.  This should be a prime about
	* 3 times the size of the anticipated data.
	* @param sz The size of the table used for the hashtable.
	*/
	Hashtable(int sz)
	{
		tableSize = 0;
		if (sz > 0)
		{
			tableSize = sz;
			table = new std::vector<HashItem<KeyType, T>>[tableSize];
			duplicateTable = table;

		}
	}

	void realityCheck(std::string& msg)
	{
		if (tableSize < 0)
			LogManager::GetInstance().Error(std::string("Hashtable.realityCheck: tableSize < 0, ") + msg);

		if (table != duplicateTable)
			LogManager::GetInstance().Error(std::string("Hashtable.realityCheck: corrupt table, ") + msg);
	}


	/**
	* Destructor to delete the allocated table but not the values stored in the table.
	*/
	~Hashtable(void)
	{
		if (table != NULL)
		{
			delete[] table;
			table = NULL;
		}
	}

	/**
	* Return the size of the table used to implement the hashtable.
	* @returns the size of the internal table.
	*/
	int getTableSize()
	{
		return tableSize;
	}

	/**
	* This searches for an returns the HashItem associated with the given key.
	* If the key is not in the hashtable, the past the end value is returned.
	*/
	iterator get(int ke)
	{
		int k = hash(ke);
		std::vector<HashItem<KeyType, T>>::size_type i = 0;
		std::vector<HashItem<KeyType, T>> *v = &table[k];
		std::vector<HashItem<KeyType, T>>::iterator iter;

#ifdef HTABLE_DEBUG
		std::cout << "Hashtable.get: key=" << ke << " bucket=" << k << " bucketSize=" << v->size() << "\n";
#endif

		for (iter = v->begin(); iter != v->end(); iter++)
		{
#ifdef HTABLE_DEBUG
			std::cout << "Hashtable.get: compare item key " << (*iter).getKey()
				<< " to key " << ke << "\n";
#endif
			if ((*iter).getKey() == ke)
			{
#ifdef HTABLE_DEBUG
				std::cout << "Hashtable.get: returns key=" << ke << " value="
					<< (*iter).getValue() << "\n";
#endif
				return iterator(*this, k, iter);
			}
		}
#ifdef HTABLE_DEBUG
		std::cout << "Hashtable.get: returns null\n";
#endif
		return end();

	}

	/**
	* This inserts a value into the hashtable.  It makes no check that the
	* item is not in the table.  This means that it is possible that the same
	* value could be stored in the table twice.  To prevent this, you should
	* use get() to make sure the item is not in the table before inserting it.
	* @param item The item to insert into the table
	*/
	void put(HashItem<KeyType, T> item)
	{
		int k = hash(item.getKey());
		std::vector<HashItem<KeyType, T>> *v = &table[k];

		v->push_back(item);

#ifdef HTABLE_DEBUG
		std::cout << "Hashtable.put: key=" << item.getKey()
			<< " val=" << item.getValue() << " bucket=" << k << " bucketSize=" << v->size() << "\n";
#endif
	}

	/**
	* Remove the HashItem with the given key from the table.
	* @param ke The key of the HashItem to remove.
	*/
	void remove(int ke)
	{
		int k = hash(ke);
		std::vector<HashItem<KeyType, T>> *v = &table[k];
		std::vector<HashItem<KeyType, T>>::iterator iter;

		for (iter = v->begin(); iter != v->end(); iter++)
		{
			if ((*iter).getKey() == ke)
			{
				v->erase(iter);
				return;
			}
		}
	}

	/**
	* Remove all entries in the table
	*/
	void clear()
	{
		for (int i = 0; i < tableSize; i++)
		{
			std::vector<HashItem<KeyType, T>> *v = &table[i];
			v->clear();
		}
	}

	/**
	* Returns a past-the-end iterator for this container.
	*/
	iterator end()
	{
		return iterator(*this, -1);
	}

	/**
	* Return an iterator to the beginning of the container.
	*/
	iterator begin()
	{
		return iterator(*this, 0);
	}

	/**
	* Hash function for strings which transforms a string into an integer.  This returns
	* a high-resolution value which is independent of the size of the table.  This function
	* should be used by external clients who need to generate a numeric key from a string.
	* @param the key to hash
	* @returns a value representing the hash of the string
	*/
	static int stringhash(std::string &s)
	{
		int result = 0;
		std::string::size_type i;

		for (i = 0; i < s.size(); i++)
		{
			int v = (int)s[i];
			result = v + (result << 3) + (result << 8) - result;
		}
		return result;
	}

	/* This version created duplicate keys for different strings
	static int stringhash(std::string &s)
	{
	int result = 0;
	std::string::size_type i;

	for(i = 0; i < s.size(); i++)
	{
	int v = (int)s[i];
	result = (result * 256 + v);
	}
	return result;
	}*/

};

#endif
