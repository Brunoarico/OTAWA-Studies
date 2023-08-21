/*
 *	customization for data structures
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2019, IRIT UPS.
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
#ifndef ELM_DATA_CUSTOM_H_
#define ELM_DATA_CUSTOM_H_

#include <elm/alloc/DefaultAllocator.h>
#include <elm/hash.h>

namespace elm {

class DefaultAllocatorDelegate {
public:
	inline t::ptr allocate(t::size size) const { return DefaultAllocator::DEFAULT.allocate(size); }
	inline void free(t::ptr p) const { DefaultAllocator::DEFAULT.free(p); }
	template <class T> T *alloc() const { return static_cast<T *>(allocate(sizeof(T))); }
};

template <class A>
class AllocatorDelegate {
public:
	inline AllocatorDelegate(A& alloc): a(alloc) { }
	inline t::ptr allocate(t::size size) const { return a.allocate(size); }
	inline void free(t::ptr p) const { a.free(p); }
	template <class T> T *alloc() const { return static_cast<T *>(allocate(sizeof(T))); }
private:
	A& a;
};

typedef DefaultAllocatorDelegate DefaultAlloc;

template <class T, class C>
class ComparatorDelegate {
public:
	inline ComparatorDelegate(const C& comp): c(comp) { }
	inline int doCompare(const T& x, const T& y) const { return c.doCompare(x, y); }
private:
	const C& c;
};

template <class T, class H>
class HashDelegate {
public:
	inline HashDelegate(const H& hash): h(hash) { }
	inline t::hash computeHash(const T& key) const { return h.computeHash(key); }
	inline bool isEqual(const T& key1, const T& key2) const { return h.isEquals(key1, key2); }
private:
	const H& h;
};

template <class T, class E>
class EquivDelegate {
public:
	inline EquivDelegate(const E& equ): e(equ) { }
	inline bool isEqual(const T& x, const T& y) const { return e.isEquals(x, y); }
private:
	const E& e;
};

}	// elm

#endif /* ELM_DATA_CUSTOM_H_ */
