/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
//#include <algorithm>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = 0;
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node = _node->_next; iterator _ptr(_node); return _ptr; }
      iterator operator ++ (int) { DListNode<T>* _ptr = _node; _node = _node->_next; return _ptr; }
      iterator& operator -- () { _node = _node->_prev; iterator _ptr(_node); return _ptr; }
      iterator operator -- (int) { DListNode<T>* _ptr = _node; _node = _node->_prev; return _ptr; }

      iterator& operator = (const iterator& i) { _node = i._node; return _node; }

      bool operator != (const iterator& i) const { if(_node == i._node)return false; else return true; }
      bool operator == (const iterator& i) const { if(_node == i._node)return true; else return false; }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _head; }
   iterator end() const { return _head->_prev; }
   bool empty() const { if(_head->_prev == _head && _head->_next == _head) return true; else return false; }
   size_t size() const { 
     size_t _size = 0; 
     DListNode<T>* _ptr = _head->_next;
     while(_ptr != _head){
       ++_size;
       _ptr = _ptr->_next;
     }
     return _size;  
   }

   void push_back(const T& x) {
     _isSorted = 0;
     DListNode<T>* _temp = new DListNode<T>(x, end()._node->_prev, end()._node);
     if(empty()) _head = _temp;
     end()._node->_prev->_next = _temp;
     end()._node->_prev = _temp;
   }
   void pop_front() { 
     DListNode<T>* ptr = _head->_next;
     _head->_prev->_next = _head->_next; 
     _head->_next->_prev = _head->_prev; 
     delete _head; _head = ptr; 
   }
   void pop_back() {
     DListNode<T>* _ptr = _head->_prev->_prev;  
     _head->_prev->_prev->_prev->_next = _head->_prev; 
     _head->_prev->_prev = _head->_prev->_prev->_prev;
     delete _ptr; 
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
     iterator cur(_head);
     while(cur._node != _head->_prev){
       if(pos == cur){
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         if(pos._node == _head)      _head = _head->_next;
         delete pos._node;
         return true;
       }
       else ++cur;
     }
     return false;
   }
   bool erase(const T& x) { 
     iterator pos(_head);
     while(pos._node != _head->_prev){
       if(x == *(pos)){
         pos._node->_prev->_next = pos._node->_next;
         pos._node->_next->_prev = pos._node->_prev;
         if(pos._node == _head)      _head = _head->_next;
         delete pos._node;
         return true;
       }
       else ++pos;
     }
     return false;
   }

   iterator find(const T& x) { 
     iterator pos(_head);
     while(pos._node != _head->_prev){
       if(x == *(pos)){
         return pos;
       }
       else ++pos;
     }
     return end();
   }

   void clear() {
     if(empty() == 0){ 
       DListNode<T>* _ptr = _head->_next->_next;
       while(_ptr != _head){
         delete _ptr->_prev;
         _ptr = _ptr->_next;
       }
       _head = _head->_prev;
       delete _head->_next;
       _head->_prev = _head->_next = _head;
     }
   }
     // delete all nodes except for the dummy node

   void sort() const {
     /*vector<T> l1;
     vector<T> l2;
     size_t _size = size();
     if(_size >= 2){
       iterator pos(_head);
       size_t i = 0;
       for(; i < _size/2; i++){
         l1.push_back(pos._node->_data);
         ++pos;
       }
       for(; i < _size; i++){
         l2.push_back(pos._node->_data);
         ++pos;
       }
       ::sort(l1.begin(), l1.begin() + l1.size());
       ::sort(l2.begin(), l2.begin() + l2.size());
       size_t idx1 = 0;
       size_t idx2 = 0;
       iterator pos3(begin());
       while(idx1 != l1.size() && idx2 != l2.size()){
         if(l1[idx1] < l2[idx2]){
           pos3._node->_data = l1[idx1];
           ++idx1;
         }
         else{
           pos3._node->_data = l2[idx2];
           ++idx2;
         }
         ++pos3;
       }
       if(idx1 == l1.size()){
         while(idx2 != l2.size()){
           pos3._node->_data = l2[idx2];
           ++idx2;
           ++pos3;
         }
       }
       else if(idx2 == l2.size()){
         while(idx1 != l1.size()){
           pos3._node->_data = l1[idx1];
           ++idx1;
           ++pos3;
         }
       }
     }*/
     if(_isSorted == 0){
       DListNode<T>* _ptr1 = _head;
       DListNode<T>* _ptr2 = _head->_next;
       for(; _ptr1 != _head->_prev; _ptr1 = _ptr1->_next){
         //cout << _ptr1->_data << endl;
         _ptr2 = _ptr1->_next;
         for(; _ptr2 != _head->_prev; _ptr2 = _ptr2->_next){
           if(_ptr2->_data < _ptr1->_data){
             T _temp = _ptr1->_data;
             _ptr1->_data = _ptr2->_data;
             _ptr2->_data = _temp;
           }
         }
       }
       _isSorted = 1;
     }
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
