//
// Created by nuh on 8/18/24.
//

#include "Hash.h"

/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
bool isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;

    if ( n == 1 || n % 2 == 0 )
        return false;

    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;

    return true;
}


/**
    * Internal method to return a prime number
   * at least as large as n.  Assumes n > 0.
    */
int nextPrime( int n)
{
    if (n%2 == 0)
        n++;

    for(; !isPrime(n); n+=2)
        ;

    return n;
}


/**
 * Construct the hash table.
 */
template<class Key, class Value>
Hash<Key, Value>::Hash(const Key& notFound, const Value& missing, int size)
    : ITEM_NOT_FOUND(notFound), ITEM_MISSING(missing), currentSize(size)
{
    if(isPrime(size))
    {
        array.resize(size);
    }
    else
    {
        array.resize(nextPrime(size));
    }
    makeEmpty();
}

/**
 * Method that performs quadratic probing resolution.
 * Return the position where the search for x terminates.
 */
template<class Key, class Value>
int Hash<Key, Value>::findPos(const Key& x) const
{
    int collisionNum = 0;
    std::hash<Key> hashFunction;
    int currentPos = hashFunction(x) % array.size();

    while(array[currentPos].info != EMPTY && array[currentPos].key != x)
    {
        currentPos += 2 * ++collisionNum - 1;
        if(currentPos >= array.size())
        {
            currentPos = currentPos - array.size();
        }
    }

    return currentPos;
}

/**
 * Return true if currentPos exists and is active.
 */
template<class Key, class Value>
bool Hash<Key, Value>::isActive(int currentPos) const
{
    return array[currentPos].info == ACTIVE;
}

/**
 * Remove item x from the hash table.
 * x has to be in the table
 */
template<class Key, class Value>
void Hash<Key, Value>::remove(const Key& x)
{
    int currentPos = findPos(x);
    if(isActive(currentPos))
    {
        array[currentPos].info = DELETED;
    }
}

/**
 * Find item x in the hash table.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template<class Key, class Value>
const Value& Hash<Key, Value>::find(const Key& x) const
{
    int currentPos = findPos(x);
    if(isActive(currentPos))
    {
        return array[currentPos].value;
    }

    return ITEM_MISSING;

}

/**
 * Insert item x into the hash table. If the item is
 * already present, then do nothing.
 */
template<class Key, class Value>
void Hash<Key, Value>::insert(const Key& x, const Value& y)
{
    //insert x as active
    int currentPos = findPos(x);
    if(isActive(currentPos))
    {
        return;
    }

    array[currentPos] = HashEntry(x, y, ACTIVE);

    //enlarge the hash table if necessary

    //calculate lambda
    double lambda = double(++currentSize) / array.size();
    if(lambda >= 0.7)
    {
        rehash();
    }
}

/**
 * Expand the hash table.
 */
template<class Key, class Value>
void Hash<Key, Value>:: rehash()
{
    vector<HashEntry> oldArray = array;

    //create new double-sized, empty table
    array.resize(nextPrime(2* oldArray.size()));

    int newsize = array.size();
    double cur = currentSize;
    double lambda = cur / array.size();

    cout << "rehashed..." << endl;
    cout << "previous table size: " << oldArray.size() << ", new table size: " << array.size()
    << ", current unique word count " << currentSize << ", current load factor: " << double(currentSize) / array.size()
    << endl;

    for (int i=0; i< oldArray.size(); i++)
    {
        array[i].info = EMPTY;
    }

    //copy table over
    currentSize = 0;
    for(int i=0; i<oldArray.size(); i++)
    {
        if(oldArray[i].info == ACTIVE)
        {
            insert(oldArray[i].key, oldArray[i].value);
        }
    }
}

/**
 * Make the hash table logically empty.
 */
template<class Key, class Value>
void Hash<Key, Value>::makeEmpty()
{
    for(int i=0; i<array.size(); i++)
    {
        //delete the dynamic memory
        delete array[i].value;
        array[i].info = EMPTY;
    }
}

template<class Key, class Value>
void Hash<Key, Value>::tableProperties()
{
    cout << endl << "After preprocessing, the unique word count is " <<
        currentSize << ". Current load ratio is " << double(currentSize) / array.size() << endl;
}

