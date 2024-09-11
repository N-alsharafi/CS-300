//
// Created by nuh on 8/12/24.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include <cmath>
using namespace std;

template <class HashedObj>
class HashTable
{
public:
    explicit HashTable( const HashedObj & notFound, int size = 1 );
    HashTable( const HashTable & rhs )
           : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
             array( rhs.array ), currentSize( rhs.currentSize ) { }

    const HashedObj & find( const HashedObj & x ) const;

    void makeEmpty( );
    void insert( const HashedObj & x );
    void remove( const HashedObj & x );

    const HashTable & operator=( const HashTable & rhs );

    enum EntryType { ACTIVE, EMPTY, DELETED };
private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ),
                   EntryType i = EMPTY )
                 : element( e ), info( i ) { }
    };

    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;

    bool isActive( int currentPos ) const;
    int findPos( const HashedObj & x ) const;
    void rehash( );
};


#include "HashTable.cpp"
#endif //HASHTABLE_H
