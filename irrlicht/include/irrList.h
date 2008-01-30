// Copyright (C) 2002-2007 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_LIST_H_INCLUDED__
#define __IRR_LIST_H_INCLUDED__

#include "irrTypes.h"

namespace irr
{
namespace core
{


//! Double linked list template.
template <class T>
class list
{
private:

	//! List element node with pointer to previous and next element in the list.
	struct SKListNode
	{
		SKListNode() : Next(0), Prev(0) {};

		SKListNode* Next;
		SKListNode* Prev;
		T Element;
	};

public:
	class ConstIterator;

	//! List iterator.
	class Iterator
	{
	public:
		Iterator() : Current(0) {}

		Iterator& operator ++()    { Current = Current->Next; return *this; }
		Iterator& operator --()    { Current = Current->Prev; return *this; }
		Iterator  operator ++(s32) { Iterator tmp = *this; Current = Current->Next; return tmp; }
		Iterator  operator --(s32) { Iterator tmp = *this; Current = Current->Prev; return tmp; }

		Iterator& operator +=(s32 num)
		{
			if(num > 0)
			{
				while (num-- && this->Current != 0) ++(*this);
			}
			else
			{
				while(num++ && this->Current != 0) --(*this);
			}
			return *this;
		}

		Iterator  operator + (s32 num) const { Iterator tmp = *this; return tmp += num; }
		Iterator& operator -=(s32 num) const { return (*this)+=(-num); }
		Iterator  operator - (s32 num) const { return (*this)+ (-num); }

		bool operator ==(const Iterator&      other) const { return Current == other.Current; }
		bool operator !=(const Iterator&      other) const { return Current != other.Current; }
		bool operator ==(const ConstIterator& other) const { return Current == other.Current; }
		bool operator !=(const ConstIterator& other) const { return Current != other.Current; }

		#if defined (_MSC_VER) && (_MSC_VER < 1300)
			#pragma warning(disable:4284) // infix notation problem when using iterator operator ->
		#endif 

		T & operator * () { return Current->Element;  }
		T * operator ->() { return &Current->Element; }

	private:
		Iterator(SKListNode* begin) : Current(begin) {}

		SKListNode* Current;

		friend class list<T>;
	};

	class ConstIterator
	{
	public:

		ConstIterator() : Current(0) {}

		ConstIterator& operator ++()    { Current = Current->Next; return *this; }
		ConstIterator& operator --()    { Current = Current->Prev; return *this; }
		ConstIterator  operator ++(s32) { ConstIterator tmp = *this; Current = Current->Next; return tmp; }
		ConstIterator  operator --(s32) { ConstIterator tmp = *this; Current = Current->Prev; return tmp; }

		ConstIterator& operator +=(s32 num)
		{
			if(num > 0)
			{
				while(num-- && this->Current != 0) ++(*this);
			}
			else
			{
				while(num++ && this->Current != 0) --(*this);
			}
			return *this;
		}

		ConstIterator  operator + (s32 num) const { ConstIterator tmp = *this; return tmp += num; }
		ConstIterator& operator -=(s32 num) const { return (*this)+=(-num); }
		ConstIterator  operator - (s32 num) const { return (*this)+ (-num); }

		bool operator ==(const ConstIterator& other) const { return Current == other.Current; }
		bool operator !=(const ConstIterator& other) const { return Current != other.Current; }
		bool operator ==(const Iterator&      other) const { return Current == other.Current; }
		bool operator !=(const Iterator&      other) const { return Current != other.Current; }

		const T & operator * () { return Current->Element; }
		const T * operator ->() { return &Current->Element; }

		ConstIterator & operator =(const Iterator & iterator) { Current = iterator.Current; return *this; }

	private:
		ConstIterator(SKListNode* begin) : Current(begin) {}

		SKListNode* Current;

		friend class Iterator;
		friend class list<T>;
	};

	//! constructor
	list()
		: First(0), Last(0), Size(0) {}


	//! copy constructor
	list(const list<T>& other) : First(0), Last(0), Size(0)
	{
		*this = other;
	}


	//! destructor
	~list()
	{
		clear();
	}


	//! Assignment operator
	void operator=(const list<T>& other)
	{
		if(&other == this)
		{
			return;
		}

		clear();

		SKListNode* node = other.First;
		while(node)
		{
			push_back(node->Element);
			node = node->Next;
		}
	}



	//! Returns amount of elements in list.
	//! \return Returns amount of elements in the list.
	u32 getSize() const
	{
		return Size;
	}



	//! Clears the list, deletes all elements in the list. All existing
	//! iterators of this list will be invalid.
	void clear()
	{
		while(First)
		{
			SKListNode * next = First->Next;
			delete First;
			First = next;
		}

		//First = 0; handled by loop
		Last = 0;
		Size = 0;
	}



	//! Returns ture if the list is empty.
	//! \return Returns true if the list is empty and false if not.
	bool empty() const
	{
		return (First == 0);
	}



	//! Adds an element at the end of the list.
	//! \param element: Element to add to the list.
	void push_back(const T& element)
	{
		SKListNode* node = new SKListNode;
		node->Element = element;

		++Size;

		if (First == 0)
			First = node;

		node->Prev = Last;

		if (Last != 0)
			Last->Next = node;

		Last = node;
	}


	//! Adds an element at the begin of the list.
	//! \param element: Element to add to the list.
	void push_front(const T& element)
	{
		SKListNode* node = new SKListNode;
		node->Element = element;

		++Size;

		if (First == 0)
		{
			Last = node;
			First = node;
		}
		else
		{
			node->Next = First;
			First->Prev = node;
			First = node;
		}
	}


	//! Gets begin node.
	//! \return Returns a list iterator pointing to the begin of the list.
	Iterator begin()
	{
		return Iterator(First);
	}


	//! Gets begin node.
	//! \return Returns a list iterator pointing to the begin of the list.
	ConstIterator begin() const
	{
		return ConstIterator(First);
	}


	//! Gets end node.
	//! \return Returns a list iterator pointing to null.
	Iterator end()
	{
		return Iterator(0);
	}


	//! Gets end node.
	//! \return Returns a list iterator pointing to null.
	ConstIterator end() const
	{
		return ConstIterator(0);
	}


	//! Gets last element.
	//! \return Returns a list iterator pointing to the end of the list.
	Iterator getLast()
	{
		return Iterator(Last);
	}


	//! Gets last element.
	//! \return Returns a list iterator pointing to the end of the list.
	ConstIterator getLast() const
	{
		return ConstIterator(Last);
	}


	//! Inserts an element after an element.
	//! \param it: Iterator pointing to element after which the new element
	//! should be inserted.
	//! \param element: The new element to be inserted into the list.
	void insert_after(const Iterator& it, const T& element)
	{
		SKListNode* node = new SKListNode;
		node->Element = element;

		node->Next = it.Current->Next;

		if (it.Current->Next)
			it.Current->Next->Prev = node;

		node->Prev = it.Current;
		it.Current->Next = node;
		++Size;

		if (it.Current == Last)
			Last = node;
	}


	//! Inserts an element before an element.
	//! \param it: Iterator pointing to element before which the new element
	//! should be inserted.
	//! \param element: The new element to be inserted into the list.
	void insert_before(const Iterator& it, const T& element)
	{
		SKListNode* node = new SKListNode;
		node->Element = element;

		node->Prev = it.Current->Prev;

		if (it.Current->Prev)
			it.Current->Prev->Next = node;

		node->Next = it.Current;
		it.Current->Prev = node;
		++Size;

		if (it.Current == First)
			First = node;
	}


	//! Erases an element
	//! \param it: Iterator pointing to the element which shall be erased.
	//! \return Returns iterator pointing to next element.
	Iterator erase(Iterator& it)
	{
		// suggest changing this to a const Iterator& and
		// working around line: it.Current = 0 (possibly with a mutable, or just let it be garbage?)

		Iterator returnIterator(it);
		++returnIterator;

		if(it.Current == First)
		{
			First = it.Current->Next;
		}
		else
		{
			it.Current->Prev->Next = it.Current->Next;
		}

		if(it.Current == Last)
		{
			Last = it.Current->Prev;
		}
		else
		{
			it.Current->Next->Prev = it.Current->Prev;
		}

		delete it.Current;
		it.Current = 0;
		--Size;

		return returnIterator;
	}

private:

	SKListNode* First;
	SKListNode* Last;
	u32 Size;

};


} // end namespace core
}// end namespace irr

#endif

