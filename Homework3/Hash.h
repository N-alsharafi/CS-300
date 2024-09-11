//
// Created by nuh on 8/18/24.
//

#ifndef HASH_H
#define HASH_H
#include<vector>
#include<cmath>
using namespace std;

template<class Key, class Value>
class Hash {
public:
    explicit Hash(const Key& notFound, const Value& missing, int size = 50);
    Hash(const Hash& rhs)
        : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND),
          array(rhs.array), currentSize(rhs.currentSize) {}

    const Value& find(const Key& x) const;

    void makeEmpty();
    void insert(const Key& x, const Value& y);
    void remove(const Key& x);

    const Hash& operator=(const Hash& rhs);
    void tableProperties();

    enum EntryType { ACTIVE, EMPTY, DELETED } info;

private:
    struct HashEntry {
        Key key;
        Value value;
        EntryType info;

        HashEntry(const Key& k = Key(), const Value& v = Value(), EntryType i = EMPTY)
            : key(k), value(v), info(i) {}
    };

    vector<HashEntry> array;
    int currentSize;
    const Key ITEM_NOT_FOUND;
    const Value ITEM_MISSING;

    bool isActive(int currentPos) const;
    int findPos(const Key& x) const;
    void rehash();
};


#include "Hash.cpp"
#endif //HASH_H
