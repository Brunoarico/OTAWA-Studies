/*
 *	ListSet class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
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
#ifndef ELM_DATA_LISTSET_H_
#define ELM_DATA_LISTSET_H_

#include "SortedList.h"

namespace elm {

template <class T, class C = Comparator<T>, class A = DefaultAlloc >
class ListSet: public SortedList<T, C, A>  {
	typedef SortedList<T, C, A> base_t;
public:
	typedef T t;
	typedef ListSet<T, C, A> self_t;
	inline ListSet(void) { }
	inline ListSet(const ListSet<T, C>& set): base_t(set) { }

	// Set concept
	inline void insert(const T& v)
		{ if(!base_t::contains(v)) base_t::add(v); }

	bool subsetOf(const SortedList<T>& l) const {
		auto i = l.base_t::begin(), j = base_t::begin();
		for(; i() && j(); i++) {
			int cmp = base_t::comparator().doCompare(*i, *j);
			if(cmp > 0)
				return false;
			if(cmp == 0)
				j++;
		}
		return !j();
	}
	inline bool operator<=(const SortedList<T>& l) const { return subsetOf(l); }
	inline bool operator<(const SortedList<T>& l) const { return !equals(l) && subsetOf(l); }
	inline bool operator>=(const SortedList<T>& l) const { return l.subsetOf(*this); }
	inline bool operator>(const SortedList<T>& l) const { return !equals(l) && l.subsetOf(*this); }

	void join(const self_t& set) {
		typename base_t::list_t::PrecIter i(base_t::list);
		typename base_t::Iter j(set);
		while(i() && j()) {
			int c = base_t::comparator().doCompare(*i, *j);
			if(c == 0) { i++; j++; }
			else if(c < 0) i++;
			else { base_t::list.addBefore(i, *j); j++; }
		}
		while(j()) { base_t::list.addBefore(i, *j); j++; }
	}

	void meet(const self_t& set) {
		typename base_t::list_t::PrecIter i(base_t::list);
		typename base_t::Iter j(set);
		while(i() && j()) {
			int c =  base_t::comparator().doCompare(*i, *j);
			if(c == 0) { i++; j++; }
			else if(c < 0) base_t::list.remove(i);
			else j++;
		}
		while(i()) base_t::list.remove(i);
	}

	void diff(const self_t& set) {
		typename base_t::list_t::PrecIter i(base_t::list);
		typename base_t::Iter j(set);
		while(i() && j()) {
			int c =  base_t::comparator().doCompare(*i, *j);
			if(c == 0) { base_t::list.remove(i); j++; }
			else if(c < 0) i++;
			else j++;
		}
	}

	inline self_t& operator+=(const self_t& set) { join(set); return *this; }
	inline self_t& operator|=(const self_t& set) { join(set); return *this; }
	inline self_t& operator*=(const self_t& set) { meet(set); return *this; }
	inline self_t& operator&=(const self_t& set) { meet(set); return *this; }
	inline self_t& operator-=(const self_t& set) { diff(set); return *this; }

	inline self_t& operator+(const self_t& set) { self_t res = *this; res.join(set); return res; }
	inline self_t& operator|(const self_t& set) { self_t res = *this; res.join(set); return res; }
	inline self_t& operator*(const self_t& set) { self_t res = *this; res.meet(set); return res; }
	inline self_t& operator&(const self_t& set) { self_t res = *this; res.meet(set); return res; }
	inline self_t& operator-(const self_t& set) { self_t res = *this; res.diff(set); return res; }

	// MutableCollection concept fix
	inline void add(const T& v) { insert(v); }
	inline self_t& operator+=(const T& val) { insert(val); return *this; }
	inline self_t& operator-=(const T& val) { base_t::remove(val); return *this; }

};

template <class T, class M>
inline bool operator<=(const T& val, const ListSet<T, M>& set) { return set.contains(val); }

}	// elm

#endif /* ELM_DATA_LISTSET_H_ */
