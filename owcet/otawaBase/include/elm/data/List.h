/*
 *	List class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2016, IRIT UPS.
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
#ifndef ELM_DATA_LIST_H
#define ELM_DATA_LIST_H

#include <elm/assert.h>
#include <elm/PreIterator.h>
#include <elm/inhstruct/SLList.h>
#include "custom.h"
#include <elm/equiv.h>

namespace elm {

// SLList class
template <class T, class E = Equiv<T>, class A = DefaultAlloc>
class List: public E, public A {

	// Node class
	class Node: public inhstruct::SLNode {
	public:
		inline Node(const T value): val(value) { }
		T val;
		inline Node *next(void) const { return nextNode(); }
		inline Node *nextNode(void) const { return static_cast<Node *>(SLNode::next()); }
		inline static void *operator new(size_t s, List<T, E, A> *l) { return l->A::allocate(s); }
		inline void free(List<T, E, A> *l) { this->~Node(); l->A::free(this); }
	private:
		inline ~Node(void) { }
	};

public:

	inline List() { }
	inline List(const List<T, E, A>& list) { copy(list); }
	inline ~List(void) { clear(); }
	inline E& equivalence() { return *this; }
	inline const E& equivalence() const { return *this; }
	inline A& allocator() { return *this; }

	void copy(const List<T, E, A>& list) {
		clear(); Iter item(list); if(!item) return; addFirst(*item); Node *cur = firstNode();
		for(item++; item(); item++) { cur->insertAfter(new(this) Node(*item)); cur = cur->next(); }
	}

	// Iter class
	class Iter: public PreIterator<Iter, const T&> {
		friend class List;
	public:
		inline Iter(void): node(0) { }
		inline Iter(const List& _list): node(_list.firstNode()) { }

		inline bool ended(void) const { return !node; }
		inline const T& item(void) const { ASSERT(node); return node->val; }
		inline void next(void) { ASSERT(node); node = node->next(); }
		inline bool equals(const Iter& i) const { return node == i.node; }

	private:
		friend class prec_iter;
		Node *node;
	};
	inline Iter items(void) const { return Iter(*this); }
	inline Iter operator*(void) const { return items(); }
	inline operator Iter(void) const { return items(); }
	inline Iter begin(void) const { return items(); }
	inline Iter end(void) const { return Iter(); }

	// PrecIter class
	class PrecIter: public PreIterator<PrecIter, const T&> {
		friend class List;
	public:
		inline PrecIter(void): node(0), prev(0) { }
		inline PrecIter(const List& _list): node(_list.firstNode()), prev(0) { }
		inline operator Iter(void) const { Iter i; i.node = node; return i; }
		inline bool operator==(const PrecIter& i) { return node == i.node; }
		inline bool operator!=(const PrecIter& i) { return node != i.node; }

		inline bool ended(void) const { return !node; }
		inline const T& item(void) const { ASSERT(node); return node->val; }
		inline void next(void) { ASSERT(node); prev = node; node = node->next(); }

	private:
		Node *node, *prev;
	};

	// SubIter class
	class SubIter: public PreIterator<SubIter, T> {
	public:
		inline SubIter(void) { }
		inline SubIter(Iter begin, Iter end): c(begin), e(end) { }
		inline bool ended(void) const { return c.equals(e); }
		inline const T& item(void) const { return *c; }
		inline void next(void) { c++; }
		inline Iter asIter(void) const { return c; }
		inline operator Iter(void) const { return c; }
	private:
		Iter c, e;
	};

	// Collection concept
	static List<T, E, A> null;
	inline int count(void) const { return _list.count(); }
	inline bool contains (const T &item) const
		{ for(Iter iter(*this); iter(); iter++) if(E::isEqual(item, *iter)) return true; return false; }
	inline bool isEmpty(void) const { return _list.isEmpty(); };
	inline operator bool(void) const { return !isEmpty(); }
	bool equals(const List<T>& l) const
		{ Iter i1(*this), i2(l); while(i1() && i2()) { if(!E::isEqual(*i1, *i2)) return false; i1++; i2++; } return !i1 && !i2; }
	inline const T& at(const Iter& i) const { return i.node->val; }

	// MutableCollection concept
	inline void clear(void)
		{ while(!_list.isEmpty()) { Node *node = firstNode(); _list.removeFirst(); node->free(this); } }
	inline void add(const T& value) { addFirst(value); }
	template <class C> inline void addAll(const C& items)
		{ for(typename C::Iter i(items); i(); i++) add(*i); }
	template <class C> inline void removeAll(const C& items)
		{ for(const auto i: items) remove(i);	}
	void remove(const T& value) {
		if(isEmpty()) return; else if(E::isEqual(first(), value)) removeFirst(); else
		for(Node *prev = firstNode(), *cur = prev->nextNode(); cur; prev = cur, cur = cur->nextNode())
		if(E::isEqual(cur->val, value)) { prev->removeNext(); cur->free(this); return; }
	}
	inline T& at(const Iter& i) { return i.node->val; }
	inline void remove(PrecIter &iter) { remove(iter.prev, iter.node); }

	// List concept
	inline T& first(void) { return firstNode()->val; }
	inline const T& first(void) const { return firstNode()->val; }
	inline T& last(void) { return lastNode()->val; }
	inline const T& last(void) const { return lastNode()->val; }
	inline T& nth(int n) { Iter i(*this); while(n) { ASSERT(i); i++; n--; } ASSERT(i); return i.node->val; };
	inline const T& nth(int n) const { Iter i(*this); while(n) { ASSERT(i); i++; n--; } ASSERT(i); return *i; };
	Iter find(const T& item) const
		{ Iter i; for(i = items(); i(); i++) if(E::isEqual(item, *i)) break; return i; }
	Iter find(const T& item, const Iter& pos) const
		{ Iter i = pos; for(i++; i(); i++) if(E::isEqual(item, *i)) break; return i; }

	// MutableList concept
	inline void addFirst(const T& value) { _list.addFirst(new(this) Node(value)); }
	inline void addLast(const T& value) { _list.addLast(new(this) Node(value)); }
	inline void addAfter(const Iter& pos, const T& value)
		{ ASSERT(pos.node); pos.node->insertAfter(new(this) Node(value)); }
	inline void addBefore(PrecIter& pos, const T& value)
		{ 	if(!pos.prev) { addFirst(value); pos.prev = firstNode(); }
			else { pos.prev->insertAfter(new(this) Node(value)); pos.prev = pos.prev->next(); } }
	inline void removeFirst(void) { Node *node = firstNode(); _list.removeFirst(); node->free(this); }
	inline void removeLast(void) { Node *node = lastNode(); _list.removeLast(); node->free(this); }
	inline void set(const Iter &pos, const T &item) { ASSERT(pos.node); pos.node->val = item; }

	// Stack concept
	inline const T& top(void) const { return first(); }
	inline T pop(void) { T r = first(); removeFirst(); return r; }
	inline void push(const T& i) { addFirst(i); }
	inline void reset(void) { clear(); }

	// operators
	inline List& operator=(const List& list) { copy(list); return *this; }
	inline bool operator&(const T& e) const { return contains(e); }
	inline T& operator[](int k) { return nth(k); }
	inline const T& operator[](int k) const { return nth(k); }
	inline bool operator==(const List<T>& l) const { return equals(l); }
	inline bool operator!=(const List<T>& l) const { return !equals(l); }
	inline List<T>& operator+=(const T& h) { add(h); return *this; }
	inline List<T>& operator+=(const List<T>& l) { addAll(l); return *this; }
	inline List<T>& operator-=(const T& h) { remove(h); return *this; }
	inline List<T>& operator-=(const List<T>& l) { removeAll(l); return *this; }

private:
	inhstruct::SLList _list;

	inline Node *firstNode(void) const { return static_cast<Node *>(_list.first()); }
	inline Node *lastNode(void) const { return static_cast<Node *>(_list.last()); }
	void remove(Node* prev, Node*& cur)
		{	ASSERT(cur); if(!prev) { removeFirst(); cur = firstNode(); }
			else { prev->removeNext(); cur->free(this); cur = prev->next(); } }
};

template <class T, class E, class A> List<T, E, A> List<T, E, A>::null;

} // elm

#endif // ELM_DATA_LIST_H
