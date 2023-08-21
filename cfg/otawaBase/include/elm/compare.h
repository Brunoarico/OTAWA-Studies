/*
 *	Comparator class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2008, IRIT UPS.
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
#ifndef ELM_COMPARATOR_H_
#define ELM_COMPARATOR_H_

#include <elm/compat.h>
#include <elm/string.h>
#include <elm/type_info.h>
#include <elm/util/Pair.h>
#include "equiv.h"

namespace elm {

// Comparator class
template <class T>
class Comparator {
public:
	typedef T t;
	static inline int compare(const T& v1, const T& v2)
		{ if(v1 == v2) return 0; else if(v1 > v2) return 1; else return -1; }
	int doCompare(const T& v1, const T& v2) const { return compare(v1, v2); }
};

// StaticComparator class
template <class T, class C>
class StaticComparator {
public:
	typedef T t;
	static inline int compare(const T& v1, const T& v2) { return C::compare(v1, v2); }
	inline int doCompare(const T& v1, const T& v2) const { return compare(v1, v2); }
};

// DynamicComparator class
template <class T>
class DynamicComparator {
public:
	typedef T t;
	static inline int compare(const T& v1, const T& v2) { return v1.compare(v2); }
	inline int doCompare(const T& v1, const T& v2) const { return v1.compare(v2); }
};
template <> class Comparator<cstring>: public DynamicComparator<cstring> { };
template <> class Comparator<string>: public DynamicComparator<string> { };

// AssocComparator class
template <class K, class T, class C = Comparator<K> >
class AssocComparator: public C {
public:
	typedef Pair<K, T> pair_t;
	typedef pair_t t;
	inline AssocComparator(const C& c = single<C>()): _c(c) { }
	static inline int compare(const pair_t& v1, const pair_t& v2)
		{ return C::compare(v1.fst, v2. fst); }
	inline int doCompare(const pair_t& v1, const pair_t& v2) const
		{ return _c.doCompare(v1.fst, v2. fst); }
	inline int compareKey(const K& k1, const K& k2) const { return _c.doCompare(k1, k2); }
private:
	const C& _c;
};
template <class K, class T> class Comparator<Pair<K, T> >
	: public AssocComparator<K, T> { };

// ReverseComparator class
template <class T, class C>
class ReverseComparator {
public:
	typedef T t;
	inline ReverseComparator(const C& c = single<C>()): _c(c) { }
	static inline int compare(const T& v1, const T& v2)
		{ return -C::compare(v1, v2); }
	inline int doCompare(const T& v1, const T& v2) const
		{ return -_c.doCompare(v1, v2); }
private:
	const C& _c;
};

// GlobalComparator class
template <class T>
class GlobalComparator {
public:
	typedef T t;
	static inline int compare(const T& v1, const T& v2)
		{ return compare(v1, v2); }
};

// Useful inlines
template <class T> inline const T& min(const T& x, const T& y)
	{ if(Comparator<T>::compare(x, y) >= 0) return y; else return x; }
template <class T, class C> inline const T& min(const T& x, const T& y, const C& c = single<C>())
	{ if(c.doCompare(x, y) >= 0) return y; else return x; }
template <class T> inline const T& max(const T& x, const T& y)
	{ if(Comparator<T>::compare(x, y) >= 0) return x; else return y; }
template <class T, class C> inline const T& max(const T& x, const T& y, const C& c = single<C>())
	{ if(c.doCompare(x, y) >= 0) return x; else return y; }

template <class C> inline typename C::t min(const C& c)
	{ 	typename C::Iter i(c); typename C::t m = *i;
		for(i++; i(); i++) if(Comparator<typename C::t>::compare(*i, m) < 0) m = *i; return m; }
template <class C, class CC> inline typename C::t min(const C& c, const CC& cc)
	{ 	typename C::Iter i(c); typename C::t m = *i;
		for(i++; i; i++) if(cc.doCompare(*i, m) < 0) m = *i; return m; }
template <class C> inline typename C::t max(const C& c)
	{ 	typename C::Iter i(c); typename C::t m = *i;
		for(i++; i(); i++) if(Comparator<typename C::t>::compare(*i, m) > 0) m = *i; return m; }

} // elm

#endif /* ELM_COMPARATOR_H_ */
