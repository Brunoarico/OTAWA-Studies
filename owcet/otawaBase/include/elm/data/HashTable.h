/*
 *	HashTable class interface
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
#ifndef ELM_DATA_HASHTABLE_H_
#define ELM_DATA_HASHTABLE_H_

#include "custom.h"
#include <elm/adapter.h>
#include <elm/array.h>
#include <elm/hash.h>

namespace elm {

template <class T, class H = HashKey<T>, class A = DefaultAlloc >
class HashTable: public H, public A {
public:
	typedef HashTable<T, H, A> self_t;

private:
	class node_t {
	public:
		inline node_t(const T& d): next(0), data(d)  { }
		node_t *next;
		T data;
	};

protected:
	node_t *find(const T& key) const {
		int i = H::computeHash(key) % _size;
		for(node_t *node = _tab[i], *prev = 0; node; prev = node, node = node->next)
			if(H::isEqual(node->data, key)) {
				if(prev) { prev->next = node->next; node->next = _tab[i]; _tab[i] = node; }
				return node;
			}
		return 0;
	}

	node_t *find_const(const T& key) const {
		int i = H::computeHash(key) % _size;
		for(node_t *node = _tab[i], *prev = 0; node; prev = node, node = node->next)
			if(H::isEqual(node->data, key)) {
				return node;
			}
		return 0;
	}

private:

	node_t *make(const T& data) {
		int i = H::computeHash(data) % _size;
		node_t *node = new(A::allocate(sizeof(node_t))) node_t(data);
		node->next = _tab[i];
		_tab[i] = node;
		return node;
	}

	struct InternIterator {
		friend class HashTable;
		inline InternIterator(const self_t& _htab): node(nullptr), htab(&_htab) { i = 0; step(); }
		inline InternIterator(const self_t& _htab, bool end): node(nullptr), htab(&_htab)
			{ if(end) { i = htab->_size; node = nullptr; } else { i = 0; step(); } }
		inline bool ended(void) const { return i >= htab->_size; }
		inline void next(void) { node = node->next; if(!node) { i++; step(); }  }
		inline bool equals(const InternIterator& it) const { return node == it.node && i == it.i && htab == it.htab; }
	protected:
		node_t *node;
	private:
		inline void step(void) { for(; i < htab->_size; i++) if(htab->_tab[i]) { node = htab->_tab[i]; break; } }
		const self_t *htab;
		int i;
	};

public:

	HashTable(int _size = 211): _size(_size), _tab(new(A::allocate(_size * sizeof(node_t *))) node_t *[_size])
		{ array::fast<node_t*>::clear(_tab, _size); }
	HashTable(const self_t& h): _size(h._size), _tab(new(A::allocate(_size * sizeof(node_t *))) node_t *[h._size])
		{ array::fast<node_t*>::clear(_tab, _size); putAll(h); }
	~HashTable(void)
		{ clear(); delete [] _tab; }
	inline const H& hash() const { return *this; }
	inline H& hash() { return *this; }
	inline const A& allocator() const { return *this; }
	inline A& allocator() { return *this; }

	inline const T *get(const T& key) const
		{ node_t *node = find(key); return node ? &node->data : 0; }
	inline const T *get_const(const T& key) const
		{ node_t *node = find_const(key); return node ? &node->data : 0; }
	inline bool hasKey(const T& key) const
	 	{ node_t *node = find(key); return node != 0; }
	inline bool hasKey_const(const T& key) const
		{ node_t *node = find_const(key); return node != 0; }
	inline bool exists(const T& key) const { return hasKey(key); }
	inline bool exists_const(const T& key) const { return hasKey_const(key); }

	void put(const T& data)
		{ node_t *node = find(data); if(node) node->data = data; else add(data); }
	template <class CC> void putAll(const CC& c)
		{ for(const auto& x: c) put(x); }


	// Collection concept
	bool isEmpty(void) const
		{ for(int i = 0; i < _size; i++) if(_tab[i]) return false; return true; }
	operator bool() const { return !isEmpty(); }
	int count(void) const
	 	{ int cnt = 0; for(int i = 0; i < _size; i++) for(node_t *cur = _tab[i]; cur; cur = cur->next) cnt++; return cnt; }
	inline bool contains(const T& x) const
		{ return find(x) != nullptr; }
	inline bool contains_const(const T& x) const
		{ return find_const(x) != nullptr; }
	template <class CC> bool containsAll(const CC& c) const
		{ for(const auto x: c) if(!contains(x)) return false; return true; }
	template <class CC> bool containsAll_const(const CC& c) const
		{ for(const auto x: c) if(!contains_const(x)) return false; return true; }

	class Iter: public InternIterator, public InplacePreIterator<Iter, T> {
	public:
		inline Iter(const self_t& htab): InternIterator(htab) { };
		inline Iter(const self_t& htab, bool end): InternIterator(htab, end) { };
		inline const T& item(void) const { return this->node->data; }
	};
	inline Iter begin() const { return Iter(*this); }
	inline Iter end() const { return Iter(*this, true); }

	inline bool equals(const HashTable<T>& h) const
		{ return containsAll(h) && h.containsAll(*this); }
	inline bool equals_const(const HashTable<T>& h) const
		{ return containsAll_const(h) && h.containsAll_const(*this); }
	inline bool operator==(const HashTable<T>& t) const { return equals(t); }
	inline bool operator!=(const HashTable<T>& t) const { return !equals(t); }

	// MutableCollection concept
	void clear(void) {
		for(int i = 0; i < _size; i++) {
			for(node_t *cur = _tab[i], *next; cur; cur = next) { next = cur->next; A::free(cur); }
			_tab[i] = 0;
		}
	}

	T *add(const T& data) { return &make(data)->data; }

	inline self_t& operator+=(const T& x) { add(x); return *this; }

	template <class C> void addAll(const C& c)
		{ for(const auto x: c) add(x); }

	void remove(const T& key) {
		int i = H::computeHash(key) % _size;
		for(node_t *node = _tab[i], *prev = 0; node; prev = node, node = node->next)
			if(H::isEqual(node->data, key)) {
				if(prev)
					prev->next = node->next;
				else
					_tab[i] = node->next;
				A::free(node);
				break;
			}
	}

	template <class C> void removeAll(const C& c)
		{ for(const auto x: c) remove(x); }

	inline self_t& operator-=(const T& x) { remove(x); return *this; }

	void remove(const Iter& i) {
		node_t *p = nullptr;
		for(node_t *n = _tab[i.i]; n != i.node; p = n, n = n->next);
		if(p == nullptr)
			_tab[i.i] = i.node->next;
		else
			p->next = i.node->next;
		A::free(i.node);
	}

	void copy(const HashTable<T, H>& t) {
		clear();
		if(_size != t._size)
			putAll(t);
		else {
			for(int i = 0; i < _size; i++) {
				if(t._tab[i] != nullptr) {
					node_t *q = t._tab[i];
					node_t *p = new(A::allocate(sizeof(node_t))) node_t(q->data);
					_tab[i] = p;
					while(q->next != nullptr) {
						q = q->next;
						p->next = new(A::allocate(sizeof(node_t))) node_t(q->data);
						p = p->next;
					}
				}
			}
		}
	}
	inline self_t& operator=(const HashTable<T, H>& c) { copy(c); return *this; }

	inline T *get(const T& key)
		{ node_t *node = find(key); return node ? &node->data : 0; }

#	ifdef ELM_STAT
		int minEntry(void) const { int m = count(0); for(int i = 1; i < _size; i++) m = min(m, count(i)); return m; }
		int maxEntry(void) const { int m = count(0); for(int i = 1; i < _size; i++) m = max(m, count(i)); return m; }
		int zeroEntry(void) const { int c = 0; for(int i = 0; i < _size; i++) if(count(i) == 0) c++; return c; }
		int size(void) const { return _size; }
#	endif

private:
#	ifdef ELM_STAT
		int count(int i) const { int c = 0; for(node_t *n = _tab[i]; n; n = n->next) c++; return c; }
#	endif

	int _size;
	node_t **_tab;
};

}	// otawa

#endif /* ELM_DATA_HASHTABLE_H_ */
