/*
 *	SortedList class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2012, IRIT UPS.
 *
 *	ELM is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	ELM is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with ELM; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_DATA_SORTEDLIST_H_
#define ELM_DATA_SORTEDLIST_H_

#include "Adapter.h"
#include "List.h"
#include "util.h"
#include <elm/compare.h>
#include <elm/util/Option.h>

namespace elm {

// SortedList class
template <class T, class C = Comparator<T>, class A = DefaultAlloc >
class SortedList {
protected:
	typedef List<T, CompareEquiv<C>, A> list_t;
public:
	typedef T t;
	typedef SortedList<T, C, A> self_t;

	inline SortedList(void) { }
	inline SortedList(const SortedList<T, C, A> &l): list(l.list) { }
	inline C& comparator() { return list.equivalence(); }
	inline const C& comparator() const { return list.equivalence(); }
	inline A& allocator() { return list.allocator(); }

	inline void removeFirst(void) { list.removeFirst(); }
	inline void removeLast(void) { list.removeLast(); }

	// Collection concept
	inline int count (void) const { return list.count(); }

	inline bool contains(const T &item) const { return find(item) != end(); }
	template <class CC> bool containsAll(const CC& c) const
		{ for(const auto i: c) if(!contains(i)) return false; return true; }

	inline bool isEmpty(void) const { return list.isEmpty(); }
	inline operator bool(void) const { return !list.isEmpty(); }

	class Iter: public list_t::Iter {
	public:
		inline Iter(void) { }
		inline Iter(const self_t& _list): list_t::Iter(_list.list) { }
	};
	inline Iter items(void) const { return Iter(*this); }
	inline Iter operator*(void) const { return items(); }
	inline operator Iter(void) const { return items(); }
	inline Iter begin(void) const { return items(); }
	inline Iter end(void) const { return Iter(); }

	bool equals(const SortedList<T>& l) const {
		Iter i = begin(), j = l.begin();
		for(; i() && j(); i++, j++)
			if(comparator().doCompare(*i, *j) != 0)
				return false;
		return !i && !j;
	}
	inline bool operator==(const SortedList<T>& l) const { return equals(l); }
	inline bool operator!=(const SortedList<T>& l) const { return !equals(l); }

	// MutableCollection concept
	inline void clear(void) { list.clear(); }
	inline void copy(const SortedList<T>& l) { list.copy(l.list); }

	void add(const T &value) {
		for(typename list_t::PrecIter current(list); current(); current++)
			if(comparator().doCompare(value,  *current) < 0) {
				list.addBefore(current, value);
				return;
			}
		list.addLast(value);
	}

	template <class CC> inline void addAll (const CC &c)
		{ list.addAll(c); }
	inline void remove(const T &item) { list.remove(item); }
	template <class CC> inline void removeAll(const CC &c)
		{ list.removeAll(c); }
	inline void remove(const Iter &iter) { list.remove(*iter); }
	inline SortedList<T>& operator+=(const T& v) { add(v); return *this; }
	inline SortedList<T>& operator-=(const T& v) { remove(v); return *this; }

	// List concept
	inline const T& first(void) const { return list.first(); }
	inline const T& last(void) const { return list.last(); }
	Iter find(const T& item, const Iter& iter) const {
		Iter i = iter; for(; i(); i++) {
			int cmp = comparator().doCompare(item, *i);
			if(cmp > 0) continue; else if(!cmp) return i; else return end();
		}
		return i;
	}
	inline Iter find(const T& item) const { return find(item, begin()); }
	inline T& at(const Iter& i) { return list.at(i); }
	inline Iter nth(int n) const { Iter i(*this); while(n != 0 && i()) { n--; i++; }; return i; }

	// operators
	inline SortedList<T, C>& operator=(const SortedList<T, C>& sl) { list.copy(sl.list); return *this; }
	inline bool operator&(const T& e) const { return list.contains(e); }
	inline T& operator[](int k) { return list[k]; }
	inline const T& operator[](int k) const { return list[k]; }


protected:
	inline void set(Iter i, const T& val) { list.set(i, val); }
	list_t list;
};

}	// elm

#endif /* ELM_DATA_SORTEDLIST_H_ */
