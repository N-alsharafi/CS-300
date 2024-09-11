//
// Created by nuh on 8/18/24.
//

#ifndef BST_H
#define BST_H



#include <string>
#include <vector>
using namespace std;

template <class Key, class Value>
class BST;

template <class Key, class Value>
class BSTNode
{
    Key key;
    Value value;
    BSTNode* left;
    BSTNode* right;
    int height;

    BSTNode(const Key& theKey, const Value& theValue, BSTNode* lt, BSTNode* rt, int h = 0)
        : key(theKey), value(theValue), left(lt), right(rt), height(h) {}

    friend class BST<Key, Value>;
};


template <class Key, class Value>
class BST
{
public:
    explicit BST(const Key& notFound, const Value& missing);
    BST();
    BST(const BST& rhs);
    ~BST();

    //no need for findmin and findmax
    const Value& find( const Key& x) const;
    bool isEmpty() const;
    void printTree() const;

    void makeEmpty();
    void insert(const Key& x, const Value& y);
    void remove(const Key& x);

    const BST& operator=(const BST& rhs);

private:
    BSTNode<Key, Value>* root;
    const Key ITEM_NOT_FOUND;
    const Value ITEM_MISSING;

    const Value& elementAt( BSTNode<Key, Value>* t) const;

    void insert(const Key& x, const Value& y, BSTNode<Key, Value>* &t) const;
    void remove(const Key& x, BSTNode<Key, Value>* &t) const;

    BSTNode<Key, Value>* find(const Key& x, BSTNode<Key, Value>* t) const;
    BSTNode<Key, Value>* findMax(BSTNode<Key, Value>* t) const;
    BSTNode<Key, Value>* findMin(BSTNode<Key, Value>* t) const;

    void makeEmpty(BSTNode<Key, Value>* &t) const;
    void printTree(BSTNode<Key, Value>* t) const;
    BSTNode<Key, Value>* clone(BSTNode<Key, Value>* t) const;

    //AVL manipulations
    //int height(BSTNode<Key, Value>* t) const;
    //int max(int lhs, int rhs) const;
    //void rotateWithLeftChild(BSTNode<Key, Value>* &k2) const;
    //void rotateWithRightChild(BSTNode<Key, Value>* &k1) const;
    //void doubleWithLeftChild(BSTNode<Key, Value>* &k3) const;
    //void doubleWithRightChild(BSTNode<Key, Value>* &k1) const;
};



#include "BST.cpp"

#endif //BST_H
