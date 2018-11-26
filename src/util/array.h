/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0), _isSorted(0) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return (*_node); }
      T& operator * () { return (*_node); }
      iterator& operator ++ () { _node += 1; return *this; }
      iterator operator ++ (int) { iterator _temp = *this; _node += 1; return _temp; }
      iterator& operator -- () { _node -= 1; return *this; }
      iterator operator -- (int) { iterator _temp = *this; _node -= 1; return _temp; }

      iterator operator + (int i) const { iterator _temp(_node + i); return _temp; }
      iterator& operator += (int i) { _node += i; return *this; }

      iterator& operator = (const iterator& i) { _node = i._node; return *this; }

      bool operator != (const iterator& i) const { if(_node != i._node)return true; else return false; }
      bool operator == (const iterator& i) const { if(_node != i._node)return false; else return true; }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return _data; }
   iterator end() const { return _data + _size; }
   bool empty() const { if(_size == 0) return true; else return false; }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) {
     //cout << x << endl;
     if(_capacity == 0){ 
       _size = _capacity = 1;
       _data = new T;
       _data[0] = x;
     }
     else{
        if(_size == _capacity){
          reserve(2*_capacity);
        }
        *(_data + _size) = x;
        //cout << _data[_size] << endl;
        ++_size;
     }
     _isSorted = 0;
   }
   void pop_front() { if(_size > 1) *(_data) = *(_data + _size - 1); if(_size > 0) --_size; _isSorted = 0;}
   void pop_back() { if(_size > 0) --_size; }

   bool erase(iterator pos) {  
     size_t idx = (size_t)(pos._node - _data);
     if(idx < _size){
       _data[idx] = _data[_size - 1];
       --_size;
       return true;
     }
     else{
       return false;
     }
   }
   bool erase(const T& x) { 
     for(size_t i = 0; i < _size; i++){
       if(x == _data[i]){
         _data[i] = _data[_size - 1];
         --_size;
         return true;
       }
     }
     return false;
   } 

   void clear() {  _size = 0; }

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data+_size); _isSorted = 1;}

   // Nice to have, but not required in this homework...
   void reserve(size_t n) { 
     if(n > _size){
       _capacity = n;
       T* temp = new T[_capacity];
       for(size_t i = 0; i < _size; i++){
         temp[i] = _data[i];
         //cout << _data[i] << " " << _size << endl;
       }
       _data = temp;
     } 
   }
   void resize(size_t n) { if(n > 0) _size = n; }

   iterator find(const T& x){
     if(_isSorted == 0){
       for(size_t i = 0; i < _size; i++){
         if(x == _data[i]){
            return _data + i;
         }
       }
     }
     else{
       size_t _head = 0;
       size_t _tail = _size - 1;
       size_t mid = (_head + _tail)/2;
       while(_head != _tail){
         mid = (_head + _tail)/2;
         //cerr << mid << endl;
         if(_data[mid] == x){
           return _data + mid;
         }
         else if(_data[mid] < x){
           _head = mid + 1;
         }
         else{
           _tail = mid - 1;
         }
       }
       mid = (_head + _tail)/2;
       if(_data[mid] == x){
         return _data + mid;
       }
     }
     return end();
   }


private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

#endif // ARRAY_H
