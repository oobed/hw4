#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    // Add helper functions here
    void insert_fix(AVLNode<Key,Value>* p,AVLNode<Key,Value>* n );
    void rRight(AVLNode<Key, Value> * n);
    void rLeft(AVLNode<Key,Value>* n);
    int getHeight(AVLNode<Key, Value>* node) const;
    void remove_fix(AVLNode<Key,Value>*n,int8_t diff);




};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
//194 and 199
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
    // TODO
    // if the balancing factor is positive, followed by another positive node: left rotation
    //root is positive, next node is negative: left, right rotation
    // neg, neg right rotation
    //neg, positive: right left rotation

    AVLNode<Key, Value> * newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    AVLNode<Key, Value> * currNode = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value> * parentNode = NULL;
    if(BinarySearchTree<Key,Value>::empty()){
        newNode->setBalance(0);
        this->root_ = newNode;

        return;
    }
    while(currNode != nullptr){

      if(newNode->getKey() < currNode->getKey()){
        parentNode = currNode;
        currNode = currNode->getLeft();
      }
      else if(newNode->getKey() > currNode->getKey()){
        parentNode = currNode;
        currNode = currNode->getRight();
      }
      else{
        currNode->setValue(newNode->getValue());
        delete newNode;
        return;
      }
    }
    newNode->parent_ = parentNode;
    newNode->setBalance(0);
    if(parentNode->getKey() > newNode->getKey()){
      parentNode->setLeft(newNode);
    }
    else{
      parentNode->setRight(newNode);
    }
    if(newNode->getParent()->getBalance() == -1){
      newNode->getParent()->setBalance(0);
      return;
    }
    else if(newNode->getParent()->getBalance() == 1){
      newNode->getParent()->setBalance(0);
      return;
    }
    else if(newNode->getParent()->getBalance() == 0){
      newNode->getParent()->setBalance(getHeight(newNode->getParent()->getRight()) - getHeight(newNode->getParent()->getLeft()));
      insert_fix(newNode->getParent(), newNode);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {
  if(p == nullptr || p->getParent()==nullptr){
    return;
  }
  AVLNode<Key,Value>* g = p->getParent();

  if(p == g->getLeft()){
    g->updateBalance(-1);
    if(g->getBalance() == 0){
      return;
    }
    else if(g->getBalance() == -1){
      insert_fix(g,p);
    }
    else if(g->getBalance() == -2){
      if(n->getParent()->getParent()->getLeft()->getLeft() ==n){
        rRight(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else if(n->getParent()->getParent()->getLeft()->getRight()== n){
        rLeft(p);
        rRight(g);
        if(n->getBalance() == -1){
          p->setBalance(0);
          g->setBalance(1);
          n->setBalance(0);
        }
        else if(n->getBalance() == 0){
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else if(n->getBalance() == 1){
          p->setBalance(-1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
      
    }
  }
  else if(p == g->getRight()){
    g->updateBalance(1);
    if(g->getBalance() == 0){
      return;
    }
    else if(g->getBalance() == 1){
      insert_fix(g,p);
      
    }
    else if(g->getBalance() == 2){
      if(p->getRight() == n && g->getRight() == p){
        rLeft(g);
        p->setBalance(0);
        g->setBalance(0);
      }
      else if(n->getParent()->getParent()->getRight()->getLeft() == n){
        rRight(p);
        rLeft(g);
        if(n->getBalance() == 1){
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
        }
        else if(n->getBalance() == 0){
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
        }
        else if(n->getBalance() == -1){
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
        }
      }
      
    
  
  
  
}
}
}



template<class Key, class Value>
void AVLTree<Key, Value>::rRight(AVLNode<Key, Value>* n)
{
  if(n != nullptr){
    AVLNode<Key, Value>* z = n;
    AVLNode<Key, Value>* y = z->getLeft();
    AVLNode<Key, Value>* c = y->getRight();
    AVLNode<Key, Value> * root = static_cast<AVLNode<Key, Value>*>(this->root_);
    if(z == root){
      BinarySearchTree<Key, Value>::root_ = y;
        }
      y->setParent(z->getParent());
      if(z->getParent() != nullptr){
        if(z->getParent()->getLeft() == z){
          z->getParent()->setLeft(y);
            }else if(z->getParent()->getRight() == z){ 
              z->getParent()->setRight(y);
            }
        }
        y->setRight(z);
        z->setLeft(c);
        z->setParent(y);
        if (c != nullptr){
          c->setParent(z);
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rLeft(AVLNode<Key, Value>* n )
{
  if(n != nullptr){
    AVLNode<Key, Value> * root = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value> * x = n;
    AVLNode<Key, Value> * y = n->getRight();
    AVLNode<Key, Value> * b = y->getLeft();

    if(x == root){
      BinarySearchTree<Key, Value>::root_ = y;

    }
    y->setParent(x->getParent());
    if(x->getParent() != nullptr){
      if(x->getParent()->getLeft() == x){
        x->getParent()->setLeft(y);
      }
      else if(x->getParent()->getRight() == x){
        x->getParent()->setRight(y);
      }
    }
    y->setLeft(x);
    x->setRight(b);
    x->setParent(y);
    if(b != nullptr){
      b->setParent(x);
    }

}
}

template <class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) const{
if (node == nullptr) {
        return 0;
    }
    int leftHeight = getHeight(node->getLeft());
    int rightHeight = getHeight(node->getRight());
    return 1 + std::max(leftHeight, rightHeight);

}
/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO 
    AVLNode<Key,Value> * remove_node = static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::internalFind(key));
    //AVLNode<Key,Value> * p = remove_node->getParent();
    int8_t diff = 0;
    if(BinarySearchTree<Key,Value>::empty()){
      return;
    }
    if(BinarySearchTree<Key,Value>::internalFind(key) == nullptr){
      return;
      }
    else{
      if(remove_node->getLeft() != nullptr && remove_node->getRight() != nullptr){
        nodeSwap(remove_node,static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::predecessor(remove_node)));
      }
      //case 1
      if(remove_node->getLeft() == nullptr && remove_node->getRight() == nullptr){

        if(remove_node == BinarySearchTree<Key,Value>::root_){
          BinarySearchTree<Key,Value>::root_ = nullptr;
          delete remove_node;
          return;

        }
        else{
          
            if(remove_node->parent_->getLeft() == remove_node){
              diff = 1;
              remove_node->parent_->setLeft(nullptr);
            // delete remove_node;
            }
            else if(remove_node->parent_->getRight() == remove_node){
              diff = -1;
              remove_node->parent_->setRight(nullptr);
            // delete remove_node;
            }
          
        }
      }
      //case 2
      else if((remove_node->getLeft() == nullptr && remove_node->getRight() != nullptr)){
        if(remove_node == BinarySearchTree<Key,Value>::root_){
          BinarySearchTree<Key,Value>::root_ = remove_node->getRight();
          (remove_node->getRight())->setParent(nullptr);
        }
        else{
        AVLNode<Key, Value> * parent = remove_node->getParent();
        AVLNode<Key, Value> * child = remove_node->getRight();
        
          child->setParent(parent);
          if(parent->getLeft() == remove_node){ 
            diff = 1;
            parent->setLeft(remove_node->getRight());
            }
          else if(parent->getRight() == remove_node){ 
            diff = -1;
            parent->setRight(remove_node->getRight());
                  }
          //delete remove_node;
        // parent->setRight(child);
        // child->setParent(parent);
      
      }
      }
      else if((remove_node->getLeft() != nullptr && remove_node->getRight() == nullptr)){
        if(remove_node == BinarySearchTree<Key,Value>::root_){
          BinarySearchTree<Key,Value>::root_ = remove_node->getLeft();
          (remove_node->getLeft())->setParent(nullptr);
        }
        else{
        AVLNode<Key, Value> * parent = remove_node->getParent();
        AVLNode<Key, Value> * child = remove_node->getLeft();
        
        child->setParent(parent);
        if (parent->getLeft() == remove_node){ 
            diff = 1;
            parent->setLeft(remove_node->getLeft());
                }
        else if (parent->getRight() == remove_node){
          diff = -1;
          parent->setRight(remove_node->getLeft());
                }
        //delete remove_node;
        
      }
      }
      AVLNode<Key,Value> * p = remove_node->getParent();
      delete remove_node;
      remove_fix(p, diff);
      } 
}

template<class Key, class Value>
void AVLTree<Key,Value>::remove_fix(AVLNode<Key,Value> * n, int8_t diff){
  int ndiff = 0;
  if(n == nullptr){
    return;
  }
  AVLNode<Key, Value>* p = n->getParent();
  if(p != nullptr){
    if(p->getLeft() == n){ 
      ndiff = 1;
    }
    else if(p->getRight() == n){
      ndiff = -1;
    }
  }
  if(diff == -1) {
    if(n->getBalance() + diff == -2) {
      AVLNode<Key, Value> *c = n->getLeft();
      if(c->getBalance() == -1){
        rRight(n);
        n->setBalance(0);
        c->setBalance(0);
        remove_fix(p, ndiff);
      } 
      else if(c->getBalance() == 0){
        rRight(n);
        n->setBalance(-1);
        c->setBalance(1);
        return;
      } 
      else if(c->getBalance() == 1){ 
        AVLNode<Key, Value> *g = c->getRight();
        rLeft(c);
        rRight(n);
        if(g->getBalance() == 1){
        n->setBalance(0);
        c->setBalance(-1);
        g->setBalance(0);
      } 
      else if(g->getBalance() == 0){
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);
      } 
      else if(g->getBalance() == -1){
        n->setBalance(1);
        c->setBalance(0);
        g->setBalance(0);
        }
      remove_fix(p, ndiff);
      }
    } 
    else if(n->getBalance() + diff == -1){
      n->setBalance(-1);
      return;
    } 
    else if(n->getBalance() + diff == 0){
      n->setBalance(0);
      remove_fix(p, ndiff);
    }
  } 
  else if(diff == 1){
    if(n->getBalance() + diff == 2){
      AVLNode<Key, Value> *c = n->getRight();
      if(c->getBalance() == 1){ 
        rLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        remove_fix(p, ndiff);
      }
      else if(c->getBalance() == 0){ 
        rLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
        return;
      }
      else if(c->getBalance() == -1){ 
        AVLNode<Key, Value> *g = c->getLeft();
        rRight(c);
        rLeft(n);
        if(g->getBalance() == -1){
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(g->getBalance() == 0){
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        } 
        else if(g->getBalance() == 1){
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        remove_fix(p, ndiff);
        }
    }
    else if(n->getBalance() + diff == 1){
      n->setBalance(1);
      return;
    } 
    else if(n->getBalance() + diff == 0){
      n->setBalance(0);
      remove_fix(p, ndiff);
      }
      }
    }
  



template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
