/*
 *	avl::Set class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2013, IRIT UPS.
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
#ifndef ELM_AVL_SET_H_
#define ELM_AVL_SET_H_

#include <elm/avl/GenTree.h>

namespace elm { namespace avl {

template <class T, class C = elm::Comparator<T> >
class Set: public GenTree<T, IdAdapter<T>, C> {
public:
	typedef T t;
	typedef Set<T, C> self_t;
	typedef GenTree<T, IdAdapter<T>, C> base_t;

	// MutableCollection concept
	inline self_t& operator+=(const T& x) { insert(x); return *this; }
	inline self_t& operator-=(const T& x) { base_t::remove(x); return *this; }
	inline self_t& operator=(const self_t& s) { base_t::copy(s); return *this; }

	// Set concept
	inline void insert(const T& x) { base_t::add(x); }

	bool subsetOf(const Set<T, C>& s) const {
		auto i = base_t::begin(); auto j = s.begin();
		while(i() && j()) {
			int c = C::doCompare(*i, *j);
			if(c == 0) i++;
			else if(c < 0) return false;
			j++;
		}
		return !i();
	}

	inline void join(const Set<T, C>& s) { for(const auto x: s) base_t::add(x); }
	inline void diff(const Set<T, C>& s) { for(const auto x: s) base_t::remove(x); }
	void meet(const Set<T, C>& s) {
		self_t is;
		for(const auto x: *this) if(s.contains(x)) is.add(x);
        base_t::copy(is);
	}
	inline self_t& operator+=(const Set<T, C>& s) { join(s); return *this; }
	inline self_t& operator|=(const Set<T, C>& s) { join(s); return *this; }
	inline self_t& operator-=(const Set<T, C>& s) { diff(s); return *this; }
	inline self_t& operator&=(const Set<T, C>& s) { meet(s); return *this; }
	inline self_t& operator*=(const Set<T, C>& s) { meet(s); return *this; }

	inline self_t operator+(const Set<T, C>& s) const { self_t r(*this); r.join(s); return r; }
	inline self_t operator|(const Set<T, C>& s) const { self_t r(*this); r.join(s); return r; }
	inline self_t operator-(const Set<T, C>& s) const { self_t r(*this); r.diff(s); return r; }
	inline self_t operator*(const Set<T, C>& s) const { self_t r(*this); r.meet(s); return r; }
	inline self_t operator&(const Set<T, C>& s) const { self_t r(*this); r.meet(s); return r; }

};

} }	// elm::avl

#endif /* ELM_AVL_SET_H_ */
