/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;

   public:

   BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* p = 0):
     _data(d), _left(l), _right(r) ,_parent(p) {}
   ~BSTreeNode() {};

   private:
   
   T _data;
   BSTreeNode<T>* _left;
   BSTreeNode<T>* _right;
   BSTreeNode<T>* _parent;

};


template <class T>
class BSTree
{
   // TODO: design your own class!!
   public:
   BSTree(){
     _head = new BSTreeNode<T>(T());
     _head->_left = _head->_right = _head->_parent = 0;
   }
   ~BSTree() { clear(); delete _head;}
   class iterator { 

     friend class BSTree;

     public:
     iterator(BSTreeNode<T>* n = 0): _node(n) {}
     iterator(const iterator& i): _node(i._node) {}
     ~iterator() {}

     const T& operator * () const { return _node->_data; }
     T& operator * () { return _node->_data; }
     iterator& operator ++ () { 
       if(_node->_right != 0){
         _node = _node->_right;
         while(_node->_left != 0){
           _node = _node->_left;
         }
         return (*this);
       }
       else {
         BSTreeNode<T>* _ptr = _node;
         if(_node->_parent != 0) _node = _node->_parent;
         while(_node->_data <= _ptr->_data && _node->_parent != 0){
           _node = _node->_parent;
         }
         return (*this);
       }
     }
     iterator operator ++ (int) { 
       BSTreeNode<T>* _ptr = _node;
       if(_node->_right != 0){
         _node = _node->_right;
         while(_node->_left != 0){
           _node = _node->_left;
         }
         return _ptr;
       }
       else {
         if(_node->_parent != 0) _node = _node->_parent;
         while(_node->_data <= _ptr->_data && _node->_parent != 0){
           _node = _node->_parent;
         }
         return _ptr;
       }
     }
     iterator& operator -- () {
       if(_node->_left != 0){
         _node = _node->_left;
         while(_node->_right != 0){
           _node = _node->_right;
         }
         return (*this);
       }
       else{
         BSTreeNode<T>* _ptr = _node;
         if(_node->_parent != 0)  _node = _node->_parent;
         while(_node->_data > _ptr->_data && _node->_parent != 0){
           _node = _node->_parent;
         }
         return (*this);
       }
     }
     iterator operator -- (int) {
       BSTreeNode<T>* _ptr = _node;
       if(_node->_left != 0){
         _node = _node->_left;
         while(_node->_right != 0){
           _node = _node->_right;
         }
         return _ptr;
       }
       else{
         if(_node->_parent != 0) _node = _node->_parent;
         while(_node->_data > _ptr->_data && _node->_parent != 0){
           _node = _node->_parent;
         }
         return _ptr;
       }
     }
     iterator& operator = (const iterator& i) { _node = i._node; return _node; }
     bool operator == (const iterator& i) { if(_node == i._node) return true; else return false; }
     bool operator != (const iterator& i) { if(_node == i._node) return false; else return true; }

     private:
     BSTreeNode<T>* _node;  
   };

   iterator begin() const{
     BSTreeNode<T>* _first = _head;
     while(_first->_left != 0){
       _first = _first->_left;
     }
     return _first;
   }
   iterator end() const{
     return _head;
   }
   bool empty() const{ if(_head->_left == 0) return true; else return false; }
   size_t size() const{ 
     iterator pos = begin();
     size_t size = 0;
     while(pos._node != _head){
       ++size;
       ++pos;
     } 
     return size;
   }

   void insert(const T& x){
     BSTreeNode<T>* _ptr = new BSTreeNode<T>(x);
     if(_head->_left == 0) {
       _ptr->_parent = _head;
       _head->_left = _head->_right = _ptr;
     }
     else{
       BSTreeNode<T>* _temp = _head->_left;
       while(1){
         if(x >= _temp->_data){
           if(_temp->_right == 0)   { _temp->_right = _ptr; break; }
           else                     _temp = _temp->_right;
         }
         else{
           if(_temp->_left == 0)    { _temp->_left = _ptr; break; }
           else                     _temp = _temp->_left;
         }
       }
       _ptr->_parent = _temp;
     }
   }
   void pop_front(){
     if(empty() == 0){
       iterator _front = begin();
       _front._node->_parent->_left = _front._node->_right;
       if(_front._node->_parent == _head)   _front._node->_parent->_right = _front._node->_right;
       if(_front._node->_right != 0)        _front._node->_right->_parent = _front._node->_parent;
       delete _front._node;
     }
   }
   void pop_back(){
     if(empty() == 0){
       iterator _last = end();
       _last--;
       _last._node->_parent->_right = _last._node->_left;
       if(_last._node->_parent == _head)  _last._node->_parent->_left = _last._node->_left;
       if(_last._node->_left != 0)        _last._node->_left->_parent = _last._node->_parent;
       delete _last._node;
     }
   }
   bool erase(const T& x){ 
     BSTreeNode<T>* _ptr = _head->_left;
     while(empty() == 0){
       //cerr << _ptr->_data << ' ' << x << endl;
       if(_ptr->_data == x){
         deleteNode(_ptr);
         return true;
       }
       else if(x >= _ptr->_data){
         if(_ptr->_right != 0){
           _ptr = _ptr->_right;
         }
         else{
           return false;
         }
       }
       else{
         if(_ptr->_left != 0){
           _ptr = _ptr->_left;
         }
         else{
           return false;
         }
       }
     }
     return false;
   }
   bool erase(iterator pos){ 
     /*iterator _ptr = begin();
     while(_ptr != end()){
       if(_ptr == pos){
         //cerr << _ptr._node->_data << endl;
         deleteNode(_ptr._node);
         return true;
       }
       else ++_ptr;
     }
     return false;*/
     return erase(*pos);
   }
   iterator find(const T& x){ 
     BSTreeNode<T>* _ptr = _head->_left;
     while(empty() == 0){
       //cerr << x << ' ' << _ptr->_data << endl;
       if(_ptr->_data == x)        return _ptr;
       else if(x >= _ptr->_data){
         if(_ptr->_right == 0)      return end();
         else                      _ptr = _ptr->_right;
       }
       else{
         if(_ptr->_left == 0)     return end();
         else                      _ptr = _ptr->_left;
       }
     }
     return end();
   }
   void clear(){ if(empty() == 0) clearNodes(_head->_left); _head->_left = _head->_right = 0; }

   void sort(){}
   void print() const{
     size_t _space = 0;
     BSTreeNode<T>* _ptr = _head->_left;
     printTree(_space, _ptr);
   }

   private:
   BSTreeNode<T>* _head;

   void deleteNode(BSTreeNode<T>* n){
     if(n->_left != 0 && n->_right != 0){
       //cerr << "l+r" << endl;
       iterator suc(n);
       ++suc;
       //cerr << *(suc) << endl;
       if(suc._node->_parent->_left == suc._node)  suc._node->_parent->_left = suc._node->_right;
       else                                        suc._node->_parent->_right = suc._node->_right;
       if(suc._node->_right != 0)                  suc._node->_right->_parent = suc._node->_parent;
       n->_left->_parent = suc._node;
       if(n->_right != 0)
       n->_right->_parent = suc._node;
       suc._node->_left = n->_left;
       suc._node->_right = n->_right;
       if(n->_parent->_left == n){
         n->_parent->_left = suc._node;
         if(n->_parent == _head)                   n->_parent->_right = suc._node;
       }
       else{
         n->_parent->_right =suc._node;
       }
       suc._node->_parent = n->_parent;
     }
     else if(n->_left != 0){
       if(n->_parent->_left == n){
         n->_parent->_left = n->_left;
         if(n->_parent == _head)  n->_parent->_right = n->_left;
       }
       else{
         n->_parent->_right = n->_left;
       }
       n->_left->_parent = n->_parent;
     }
     else{
       if(n->_parent->_left == n){
         n->_parent->_left = n->_right;
         if(n->_parent == _head)  n->_parent->_right = n->_right;
       }
       else{
         n->_parent->_right = n->_right;
       }
       if(n->_right != 0)   n->_right->_parent = n->_parent;
     }
     delete n;
   }
   void printTree(size_t _space , BSTreeNode<T>* _ptr) const {
     if(empty() == 0){
       for(size_t i = 0; i < _space; i++)      cout << " ";
       cout << _ptr->_data << endl;
       if(_ptr->_left == 0){
         for(size_t i = 0; i < _space + 2; i++)      cout << " ";
         cout << "[0]" << endl;
       }
       else{
         printTree(_space + 2, _ptr->_left);
       }
       if(_ptr->_right == 0){
         for(size_t i = 0; i < _space + 2; i++)      cout << " ";
         cout << "[0]" << endl;
       }
       else{
         printTree(_space + 2, _ptr->_right);
       }
     }
   }
   void clearNodes(BSTreeNode<T>* _ptr){
     if(_ptr->_left != 0)  clearNodes(_ptr->_left);
     if(_ptr->_right != 0) clearNodes(_ptr->_right);
     delete _ptr;
   }
};

#endif // BST_H
