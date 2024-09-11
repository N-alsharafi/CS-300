//
// Created by nuh on 8/10/24.
//

#ifndef AVLSEARCHTREE_H
#define AVLSEARCHTREE_H
#include <string>
#include <vector>
using namespace std;

template <class Key, class Value>
class AVLSearchTree;

template <class Key, class Value>
class AVLNode
{
    Key key;
    Value value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const Key& theKey, const Value& theValue, AVLNode* lt, AVLNode* rt, int h = 0)
        : key(theKey), value(theValue), left(lt), right(rt), height(h) {}

    friend class AVLSearchTree<Key, Value>;
};


template <class Key, class Value>
class AVLSearchTree
{
public:
    explicit AVLSearchTree(const Key& notFound, const Value& missing);
    AVLSearchTree(const AVLSearchTree& rhs);
    ~AVLSearchTree();

    //no need for findmin and findmax
    const Value& find( const Key& x) const;
    bool isEmpty() const;
    void printTree() const;

    void makeEmpty();
    void insert(const Key& x, const Value& y);
    void remove(const Key& x);

    const AVLSearchTree& operator=(const AVLSearchTree& rhs);

private:
    AVLNode<Key, Value>* root;
    const Key ITEM_NOT_FOUND;
    const Value ITEM_MISSING;

    const Value& elementAt( AVLNode<Key, Value>* t) const;

    void insert(const Key& x, const Value& y, AVLNode<Key, Value>* &t) const;
    void remove(const Key& x, AVLNode<Key, Value>* &t) const;

    AVLNode<Key, Value>* find(const Key& x, AVLNode<Key, Value>* t) const;
    AVLNode<Key, Value>* findMax(AVLNode<Key, Value>* t) const;
    AVLNode<Key, Value>* findMin(AVLNode<Key, Value>* t) const;

    void makeEmpty(AVLNode<Key, Value>* &t) const;
    void printTree(AVLNode<Key, Value>* t) const;
    AVLNode<Key, Value>* clone(AVLNode<Key, Value>* t) const;

    //AVL manipulations
    int height(AVLNode<Key, Value>* t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AVLNode<Key, Value>* &k2) const;
    void rotateWithRightChild(AVLNode<Key, Value>* &k1) const;
    void doubleWithLeftChild(AVLNode<Key, Value>* &k3) const;
    void doubleWithRightChild(AVLNode<Key, Value>* &k1) const;
};



#include "AVLSearchTree.cpp"

#endif //AVLSEARCHTREE_H
