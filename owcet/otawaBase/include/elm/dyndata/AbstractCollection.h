/*
 *	$Id$
 *	AbstractCollection class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-08, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_DYNDATA_ABSTRACTCOLLECTION_H
#define ELM_DYNDATA_ABSTRACTCOLLECTION_H

#include <elm/PreIterator.h>
#include <elm/ptr.h>

namespace elm { namespace dyndata {

// IteratorInst class
template <class T>
class AbstractIter: public Lock {
public:
	virtual ~AbstractIter(void) { }
	virtual bool ended(void) const = 0;
	virtual T item(void) const = 0;
	virtual void next(void) = 0;
};


// Iterator class
template <class T>
class Iter: public PreIterator<Iter<T>, T> {
public:
	inline Iter(AbstractIter<T> *iter): i(iter) { }
	inline Iter(const Iter<T>& iter): i(iter.i) { }
	inline AbstractIter<T> *instance(void) const { return &i; }
	inline bool ended(void) const { return i->ended(); }
	inline T item(void) const { return i->item(); }
	inline void next(void) { i->next(); }
	inline Iter<T>& operator=(const Iter<T>& iter) { i = iter.i; return *this; }
protected:
	LockPtr<AbstractIter<T> > i;
};

// AbstractCollection class
template <class T>
class AbstractCollection {
public:
	virtual ~AbstractCollection(void) { }
	virtual int count(void) = 0;
	virtual bool contains(const T& item) const = 0;
	virtual bool isEmpty(void) const = 0;
	inline operator bool(void) const { return !isEmpty(); }
	virtual Iter<T> items(void) const = 0;
	inline Iter<T> operator*(void) const { return items(); }
};

// MutableAbstractCollection class
template <class T>
class MutableAbstractCollection: public virtual AbstractCollection<T> {
public:
	virtual ~MutableAbstractCollection(void) { }
	virtual void clear(void) = 0;
	virtual void add(const T& item) = 0;
	virtual void addAll(const AbstractCollection<T>& items) = 0;
	virtual void remove(const T& item) = 0;
	virtual void removeAll(const AbstractCollection<T>& items) = 0;
	virtual void remove(const Iter<T>& iter) = 0;
};

} } // elm::dyndata

#endif	// ELM_DYNDATA_ABSTRACTCOLLECTION_H
