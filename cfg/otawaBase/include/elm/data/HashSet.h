/*
 *	HashSet class interface
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
#ifndef ELM_DATA_HASHSET_H_
#define ELM_DATA_HASHSET_H_

#include "List.h"
#include "HashTable.h"
#include <elm/adapter.h>

namespace elm {

template <class T, class H = HashKey<T>, class A = DefaultAlloc>
class HashSet {
	typedef HashTable<T, H, A> tab_t;
public:
	typedef HashSet<T, H, A> self_t;

	inline HashSet(int size = 211): _tab(size) { }
	inline HashSet(const HashSet<T>& s): _tab(s._tab) { }
	inline const H& hash() const { return _tab.hash(); }
	inline H& hash() { return _tab.hash(); }
	inline const A& allocator() const { return _tab.allocator(); }
	inline A& allocator() { return _tab.allocator(); }

	// Collection concept
	inline int count(void) const { return _tab.count(); }
	inline bool contains(const T& val) const { return _tab.hasKey(val); }
	template <class C> inline bool containsAll(const C& coll)
		{ for(typename C::Iter i(coll); i(); i++) if(!contains(*i)) return false; return true; }
	inline bool isEmpty(void) const { return _tab.isEmpty(); }
	inline operator bool(void) const { return !isEmpty(); }

	class Iter: public InplacePreIterator<Iter, T> {
		friend class HashSet;
	public:
		inline Iter(const HashSet& set): i(set._tab) { }
		inline Iter(const HashSet& set, bool end): i(set._tab, end) { }
		inline bool ended(void) const { return i.ended(); }
		inline const T& item(void) const { return i.item(); }
		inline void next(void) { i.next(); }
		inline bool equals(const Iter& it) const { return i.equals(it.i); }
	private:
		typename tab_t::Iter i;
	};
	inline Iter begin(void) const { return Iter(*this); }
	inline Iter end(void) const { return Iter(*this, true); }

	inline bool equals(const HashSet<T>& s) const
		{ return _tab.equals(s._tab); }
	inline bool operator==(const HashSet<T>& s) const { return equals(s); }
	inline bool operator!=(const HashSet<T>& s) const { return !equals(s); }

	// MutableCollection concept
	inline void clear(void) { _tab.clear(); }
	inline void add(const T& val) { insert(val); }
	template <class C> void addAll(const C& coll)
		{ for(typename C::Iter i(coll); i(); i++) add(*i); }
	inline void remove(const T& val) { _tab.remove(val); }
	template <class C> void removeAll(const C& c)
		{ for(const auto x: c) remove(x); }
	inline void remove(const Iter& i) { _tab.remove(i.i); }
	inline void copy(const HashSet<T>& s) { _tab.copy(s._tab); }
	inline self_t operator=(const HashSet<T>& s) { copy(s); return *this; }
	inline self_t operator+=(const T& x) { add(x); return *this; }
	inline self_t operator-=(const T& x) { remove(x); return *this; }

	// Set concept
	inline void insert(const T& val) { _tab.put(val); }
	inline bool subsetOf(const HashSet<T>& s) const
		{ for(const auto x: *this) if(!s.contains(x)) return false; return true; }
	inline bool operator<=(const HashSet<T>& s) const { return subsetOf(s); }
	inline bool operator>=(const HashSet<T>& s) const { return s.subsetOf(*this); }
	inline bool operator<(const HashSet<T>& s) const { return _tab < s._tab; }
	inline bool operator>(const HashSet<T>& s) const { return _tab > s._tab; }
	inline void join(const HashSet<T>& c)
		{ for(const auto x: c) insert(x); }
	inline void diff(const HashSet<T>& c)
		{ for(const auto x: c) remove(x); }
	void meet(const HashSet<T>& c) {
		List<T> l;
		for(const auto x: *this)
			if(!c.contains(x))
				l.add(x);
		for(const auto x: l)
			remove(x);
	}
	inline self_t& operator+=(const HashSet<T>& s) { join(s); return *this; }
	inline self_t& operator|=(const HashSet<T>& s) { join(s); return *this; }
	inline self_t& operator-=(const HashSet<T>& s) { diff(s); return *this; }
	inline self_t& operator&=(const HashSet<T>& s) { meet(s); return *this; }
	inline self_t& operator*=(const HashSet<T>& s) { meet(s); return *this; }
	inline self_t operator+(const HashSet<T>& s) const
		{ self_t r(*this); r.join(s); return r; }
	inline self_t operator|(const HashSet<T>& s) const
		{ self_t r(*this); r.join(s); return r; }
	inline self_t operator-(const HashSet<T>& s) const
		{ self_t r(*this); r.diff(s); return r; }
	inline self_t operator&(const HashSet<T>& s) const
		{ self_t r(*this); r.meet(s); return r; }
	inline self_t operator*(const HashSet<T>& s) const
		{ self_t r(*this); r.meet(s); return r; }

	static const self_t null;

#	ifdef ELM_STAT
		int minEntry(void) const { return _tab.minEntry(); }
		int maxEntry(void) const { return _tab.maxEntry(); }
		int zeroEntry(void) const { return _tab.zeroEntry(); }
		int size(void) const { return _tab.size(); }
#	endif

	// deprecated
	inline Iter items(void) const { return Iter(*this); }
	inline Iter operator*(void) const { return items(); }

private:
	tab_t _tab;
};

template <class T, class H, class A>
const HashSet<T, H, A> HashSet<T, H, A>::null(1);

}	// elm

#endif /* ELM_DATA_HASHSET_H_ */
