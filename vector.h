#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <sstream>

using namespace std;

//static constexpr size_type min_sz = 5;

template <typename T>
class Vector 
{
  
public:
  

class Iterator;
class ConstIterator;
using value_type = T;
using size_type = size_t;
using difference_type = ptrdiff_t;
using reference =value_type&;
using const_reference = const value_type&;
using pointer  = value_type*;
using const_pointer = const value_type*;
using iterator = Vector::Iterator;
using const_iterator = Vector::ConstIterator;


private: 

size_type sz;
size_type max_sz;
pointer values;

static constexpr size_type min_sz = 5;


public:
  
class Iterator
{
  
public:
  
 using value_type = Vector::value_type;
 using reference = Vector::reference;
 using pointer = Vector::pointer;
 using difference_type = Vector::difference_type;
 using iterator_category = std::forward_iterator_tag;

private:

pointer ptr;


public:

Iterator(): ptr{nullptr} {}   // Liefert einen Iterator auf nullptr
Iterator(pointer ptr): ptr{ptr}{} // Liefert einen Iterator, der die Instanzvariable auf ptr setzt.



reference operator*() 
{
  return *ptr;
}     



pointer operator->() 
{
  return ptr;
}



friend bool operator==(const iterator& lop, const iterator& rop) 
{
   return lop.ptr == rop.ptr;
}

friend bool operator!=(const iterator& lop, const iterator& rop) 
{
   return lop.ptr != rop.ptr;
}

friend Vector::difference_type operator-(const iterator& lop, const iterator& rop)
{
  return lop.ptr-rop.ptr;
}



//Prefix
iterator& operator++() 
{
     ++ptr;     
     return *this;
}  

//Postfix
iterator operator++(int) 
{
  iterator key = (*this); // copy alte values
   ++(*this);             // aufrufe operator++; 
   return key;            // returnieren alte values
} 

//Type-Conversion
operator const_iterator() const
{
  return const_iterator(ptr); 
}  

};

 
class ConstIterator 
{
  
public:
  
 using value_type = Vector::value_type;
 using reference = Vector::const_reference;
 using pointer = Vector::const_pointer;
 using difference_type = Vector::difference_type;
 using iterator_category = std::forward_iterator_tag;

private:

pointer ptr;


public:
ConstIterator(): ptr{nullptr} {}   // Liefert einen Iterator auf nullptr
ConstIterator(pointer ptr): ptr{ptr}{} // Liefert einen Iterator, der die Instanzvariable auf ptr setzt.


reference operator*() 
{
  return *ptr;
}        
 



const pointer operator->() 
{
  return ptr;
}


friend bool operator==(const const_iterator& lop, const const_iterator& rop ) 
{
   return lop.ptr == rop.ptr;
}

friend bool operator!=(const const_iterator& lop, const const_iterator& rop ) 
{
   return lop.ptr != rop.ptr;
}


friend Vector::difference_type operator -(const const_iterator& lop, const const_iterator& rop)
{
  return lop.ptr - rop.ptr;
}

//Prefix
const_iterator& operator++() 
{
   ++ptr;     
   return *this;
}  

//Postfix
const_iterator operator++(int) 
{
  const_iterator key = (*this); // copy alte values
   ++(*this);             // aufrufe operator++; 
   return key;            // returnieren alte values
} 


};






//default construktor
Vector(): sz{0}, max_sz{min_sz}, values{new value_type[max_sz]}{}

// vector v(5)
Vector(size_type n): sz{0} 
{  if(n < min_sz)
  { 
   values = new value_type[min_sz];
   max_sz = min_sz;
  }

 else 
  {  values = new value_type[n];
     max_sz = n;
  }
}


 // v({1.0,2.0}usw)
Vector(initializer_list<value_type> li): Vector(li.size())
{
  for(auto elem: li)
   push_back(elem);
}


   // desktructor
~Vector(){delete[] values;}   


  // copy construktor
Vector(const Vector& x): Vector(x.size())
{
  for(size_type i{0}; i < x.size(); i++)
   push_back(x[i]);
}


size_type size() const{ return sz;} 


size_type empty() const{ return sz==0;}


void reserve(size_type n)
{
    // tafel 1
    pointer temp = new value_type[n];
     for(size_type i{0}; i < sz; ++i)
      temp[i] = values[i];
    
    // tafel 2
    delete [] values;
    // tafel 3
    values = temp;
    max_sz = n;
}

  
void push_back (value_type objekt)
{
    if(sz == max_sz) {reserve(sz*2);}
    values[sz++] = objekt;
}


void pop_back ()
{
  if(sz == 0) throw runtime_error ("Out of Bounds");
  sz--;
}


void clear(){ sz=0;}
   
  
void shrink_to_fit(){ max_sz = sz;}
  

   
Vector operator= (Vector src) 
{
   delete[] values;
   sz = 0;
   max_sz = src.size();
  
   values = new value_type[src.size()];
    for(size_type i = 0; i < src.size(); i++)
     push_back(src[i]);
      return *this;
}
  

 
reference operator[]( size_type index)
{
  if(index < 0 || index >= sz)
  {
   throw runtime_error ("Out of Bounds"+ to_string(index));
  } else  return values[index];
}


const reference operator[] (size_type index) const 
{
  if (index < 0 || index >=sz) 
  {
   throw runtime_error("Out of Bounds");
  } else return values[index];
}


friend ostream& operator<<(ostream& o, const Vector& v){
o << "[";
for(size_type i{0}; i < v.sz-1; ++i){
o<< v.values[i] << ", ";
}

if(v.sz >0) {
o << v.values[v.sz-1];
}
o << "]";
return o;

}
 



 iterator begin() {return iterator(values);}

 iterator end() {return iterator(values+sz);}

 const_iterator begin() const {return const_iterator(values);}

 const_iterator end() const {return const_iterator(values+sz);}




iterator insert(const_iterator pos, const_reference val)
{
	auto diff = pos-begin();
	if(diff<0 || static_cast<size_type>(diff)>sz)
		throw runtime_error("Iterator_out_of_bounds");
	size_type current{static_cast<size_type>(diff)};
	if(sz>=max_sz)
		reserve(max_sz*2);
	for(size_t i{sz}; i-->current;)
		values[i+1]=values[i];
	values[current]=val;
	++sz;
	return iterator{values+current};
}

iterator erase(const_iterator pos)
{
	auto diff = pos-begin();
	if(diff<0 || static_cast<size_type>(diff)>=sz)
		throw runtime_error("Iterator_out_of_bounds");
	size_type current{static_cast<size_type>(diff)};
	for(size_type i{current}; i<sz-1; ++i)
		values[i]=values[i+1];
	--sz;
	return iterator{values+current};
}

};

#endif
