//
// Created by nuh on 8/10/24.
//

#include "AVLSearchTree.h"


//Standard binary tree operations

/**
 * Construct the tree.
 */
template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree(const Key& notFound, const Value& missing)
        : ITEM_NOT_FOUND(notFound), root(NULL), ITEM_MISSING(missing)
{}

/**
 * Copy constructor.
 */
template<class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree(const AVLSearchTree &rhs)
    :root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), ITEM_MISSING(rhs.ITEM_MISSING)
{
    *this = rhs;
}

/**
 * Destructor for the tree.
 */
template<class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree()
{
    makeEmpty();
}

/**
    * Internal method to get element field in node t.
    * Return the element field or ITEM_NOT_FOUND if t is NULL.
    */
template <class Key, class Value>
const Value& AVLSearchTree<Key, Value>::elementAt(AVLNode<Key, Value>* t) const
{
    if (t==NULL)
    {
        return ITEM_MISSING;
    }
    return t->value;
    //return t == NULL ? ITEM_NOT_FOUND : t->value;
}

/**
    *Find item x in the tree.
    *Return the matching item or ITEM_NOT_FOUND if not found.
    */
template <class Key, class Value>
const Value& AVLSearchTree<Key, Value>::find(const Key& x) const
{
    return elementAt(find(x, root));
}

/**
 * Internal method to find an item in a subtree.
 * x is item to search for.
 * t is the node that roots the tree.
 * Return node containing the matched item.
 */
template <class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::
find(const Key& x, AVLNode<Key, Value>* t) const
{
    if(t == NULL)
        return NULL;
    else if(x < t->key)
        return find(x, t->left);
    else if(t->key < x)
        return find(x, t->right);
    else
    {
        //address = t->value;
        return t;
    }
}

/**
 *internal method to find the largest item in a subtree.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::
findMax(AVLNode<Key, Value>* t) const
{
    if(t != NULL)
        while(t->right != NULL)
            t = t->right;
    return t;
}

/**
 *internal method to find the smallest item in a subtree.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::
findMin(AVLNode<Key, Value>* t) const
{
    if(t == NULL)
        return NULL;
    if(t->left == NULL)
        return t;
    return findMin(t->left);
}

/**
  * Insert x into the tree; duplicates are ignored.
  */
template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& x, const Value& y)
{
    insert(x, y, root);
}

/**
  * Remove x from the tree. Nothing is done if x is not found.
  */
template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& x)
{
    remove(x, root);
}

/**
  * Internal method to insert into a subtree.
  * x is the item to insert.
  * t is the node that roots the tree.
  * Set the new root of the subtree.
  */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::
remove(const Key& x, AVLNode<Key, Value>* &t) const
{
    if(t == NULL)
    {
        return;
    }
    if (x < t->key)
    {
        remove(x, t->left);
    }
    else if (x > t->key)
    {
        remove(x, t->right);
    }
    else if (t->left != NULL && t->right != NULL)
    {
        t->key = findMin(t->right)->key;
        remove(t->key, t->right);
    }
    else //one or no children
    {
        AVLNode<Key, Value>* oldNode = t;
        t = (t->left !=NULL) ? t->left : t->right;
        delete oldNode;
    }

    if(t == NULL)
    {
        return;
    }

    //balance
    t->height = max(height(t->left), height(t->right)) +1;

    //check balance
    if(height(t->left) - height(t->right) == 2)
    {
        if (height(t->left->left) >= height(t->left->right))
        {
            rotateWithLeftChild(t);
        }
        else
        {
            doubleWithLeftChild(t);
        }
    }
    else if(height(t->right) - height(t->left) == 2)
    {
        if (height(t->right->right) >= height(t->right->left))
        {
            rotateWithRightChild(t);
        }
        else
        {
            doubleWithRightChild(t);
        }
    }
}

/**
 *make the tree logically empty.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty()
{
    makeEmpty(root);
}

/**
 * Internal method to make subtree empty.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AVLNode<Key, Value>* &t) const
{
    if(t != NULL)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

/**
 * Print the tree contents in sorted order.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::printTree() const
{
    if(isEmpty())
    {
        cout << "Empty tree" << endl;
    }
    else
    {
        printTree(root);
    }
}

/**
 * Internal method to print a subtree rooted at t in sorted order.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::printTree(AVLNode<Key, Value>* t) const
{
    if(t != NULL)
    {
        printTree(t->left);
        cout << t->key << endl;
        printTree(t->right);
    }
}

/**
 * isEmpty function
 */
template<class Key, class Value>
bool AVLSearchTree<Key, Value>::isEmpty() const
{
    return root == NULL;
}

/**
 * Deep copy.
 */
template<class Key, class Value>
const AVLSearchTree<Key, Value>& AVLSearchTree<Key, Value>::
operator=(const AVLSearchTree &rhs)
{
    if(this != &rhs)
    {
        makeEmpty();
        root = clone(rhs.root);
    }
    return *this;
}

/**
 * Internal method to clone subtree.
 */
template<class Key, class Value>
AVLNode<Key, Value>* AVLSearchTree<Key, Value>::
clone(AVLNode<Key, Value>* t) const
{
    if(t == NULL)
    {
        return NULL;
    }
    else
    {
        return new AVLNode<Key, Value>(t->key, t->value, clone(t->left), clone(t->right), t->height);
    }
}

//AVL manipulations

/**
 * Return the height of node t or -1 if NULL.
 */
template<class Key, class Value>
int AVLSearchTree<Key, Value>::height(AVLNode<Key, Value>* t) const
{
    return t == NULL ? -1 : t->height;
}

/**
 * Return maximum of lhs and rhs.
 */
template<class Key, class Value>
int AVLSearchTree<Key, Value>::max(int lhs, int rhs) const
{
    return lhs > rhs ? lhs : rhs;
}

/**
 * insert into the tree.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key &x, const Value &y, AVLNode<Key, Value>* &t) const
{
    if(t==NULL)
    {
        t = new AVLNode<Key, Value>(x, y, NULL, NULL);
    }
    else if( x < t->key)
    {
        insert(x, y, t->left);

        //check if tree is out of balance and balance it
        if( height(t->left) - height(t->right) == 2)
        {
            if(x < t->left->key)
            {
                rotateWithLeftChild(t);
            }
            else
            {
                doubleWithLeftChild(t);
            }
        }
    }
    else if (x > t->key)
    {
        insert(x, y, t->right);

        //check if tree is out of balance and balance it
        if( height(t->right) - height(t->left) == 2)
        {
            if(x > t->right->key)
            {
                rotateWithRightChild(t);
            }
            else
            {
                doubleWithRightChild(t);
            }
        }
    }
    else
    {
        //duplicate; do nothing
    }

    //update the height
    t->height = max(height(t->left), height(t->right)) + 1;
}

/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AVLNode<Key, Value> *&k2) const
{
    AVLNode<Key, Value>* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 * For AVL trees, this is a single rotation for case 4.
 * Update heights, then set new root.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(AVLNode<Key, Value> *&k1) const
{
    AVLNode<Key, Value>* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

/**
 * Double rotate binary tree node: first left child.
 * with its right child; then node k3 with new left child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AVLNode<Key, Value> *&k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

/**
 * Double rotate binary tree node: first right child.
 * with its left child; then node k1 with new right child.
 * For AVL trees, this is a double rotation for case 3.
 * Update heights, then set new root.
 */
template<class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AVLNode<Key, Value> *&k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}


