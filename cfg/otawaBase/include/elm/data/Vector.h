/*
 *	Vector class interface
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
#ifndef INCLUDE_ELM_DATA_VECTOR_H_
#define INCLUDE_ELM_DATA_VECTOR_H_

#include "custom.h"
#include "Array.h"
#include "Manager.h"

#include <elm/array.h>
#include <elm/compat.h>

namespace elm {

template <class T, class E = Equiv<T>, class A = DefaultAlloc >
class Vector: public E, public A {
	inline T *newVec(int size)
		{	T *t = static_cast<T *>(A::allocate(size * sizeof(T)));
			array::construct(t, size); return t; }
	inline void deleteVec(T *t, int size) { array::destruct(t, size); A::free(t); }

public:
	typedef T t;
	typedef Vector<T, E, A> self_t;

	inline Vector(int _cap = 8): tab(newVec(_cap)), cap(_cap), cnt(0) { }
	inline Vector(const Vector<T>& vec): tab(0), cap(0), cnt(0) { copy(vec); }
	inline ~Vector(void) { if(tab) deleteVec(tab, cap); }
	inline const E& equivalence() const { return *this; }
	inline E& equivalence() { return *this; }
	inline const A& allocator() const { return *this; }
	inline A& allocator() { return *this; }

	inline int capacity(void) const { return cap; }
	inline Array<const T> asArray(void) const { return Array<const T>(count(), tab); }
	inline Array<T> asArray(void) { return Array<T>(count(), tab); }
	inline Array<T> detach(void)
		{ T *rt = tab; int rc = cnt; tab = 0; cnt = 0; return Array<T>(rc, rt); }
	void grow(int new_cap)
		{	ASSERTP(new_cap >= cap, "new capacity must be bigger than old one");
			T *new_tab = newVec(new_cap); array::copy(new_tab, tab, cnt); deleteVec(tab, cap); tab = new_tab; cap = new_cap; }
	void setLength(int new_length)
		{	int new_cap; ASSERTP(new_length >= 0, "new length must be >= 0");
			for(new_cap = 1; new_cap < new_length; new_cap *= 2);
			if (new_cap > cap) grow(new_cap); cnt = new_length; }
	inline T& addNew(void) { if(cnt >= cap) grow(cap * 2); return tab[cnt++]; }

	class PreIter {
		friend class Vector;
	public:
		inline PreIter(const self_t& vec, int idx = 0): _vec(&vec), i(idx) { }
		inline bool ended(void) const { return i >= _vec->length(); }
		inline void next(void) { i++; }
		inline int index(void) const { return i; }
		inline bool equals(const PreIter& it) const { return _vec == it._vec && i == it.i; }
	protected:
		const self_t *_vec;
		int i;
	};

	// Collection concept
	class Iter: public PreIter, public elm::ConstPreIter<Iter, T>, public elm::PreIter<Iter, T> {
	public:
		using PreIter::PreIter;
		inline const T& item() const { return (*PreIter::_vec)[PreIter::i]; }
	};
	inline Iter begin() const { return Iter(*this); }
	inline Iter end() const { return Iter(*this, count()); }

	inline int count(void) const { return cnt; }
	bool contains(const T& v) const
		{ for(Iter i(*this); i(); i++) if(v == *i) return true; return false; }
	template <class C> inline bool containsAll(const C& items)
		{ for(const auto& x: items) if(!contains(x)) return false; return true; }
	inline bool isEmpty(void) const { return cnt == 0; }
	inline operator bool(void) const { return cnt != 0; }

	template <class C> inline bool equals(const C& c) const {
		int i = 0; for(const auto& x: c)
			{ if(i >= count() || !E::equals(tab[i], x)) return false; i++; } return i == count(); }
	inline bool operator==(const Vector<T>& v) const { return equals(v); }
	inline bool operator!=(const Vector<T>& v) const { return !equals(v); }

	static const Vector<T, E, A> null;

	// MutableCollection concept
	class MutIter: public PreIter, public MutPreIter<MutIter, T>, public elm::PreIter<MutIter, T> {
	public:
		using PreIter::PreIter;
		inline T& item() const { return (*const_cast<self_t *>(PreIter::_vec))[PreIter::i]; }
		inline operator Iter() const { return Iter(*PreIter::_vec, PreIter::i); }
	};
	inline MutIter begin() { return MutIter(*this); }
	inline MutIter end() { return MutIter(*this, count()); }

	inline void clear(void) { cnt = 0; }
	void add(const T& v) { if(cnt >= cap) grow(cap * 2); tab[cnt++] = v; }
	template <class C> inline void addAll(const C& c)
		{ for(typename C::Iter i(c); i(); i++) add(*i); }
	inline void remove(const T& value) { int i = indexOf(value); if(i >= 0) removeAt(i); }
	template <class C> inline void removeAll(const C& c)
		{ for(const auto x: c) remove(x); }
	inline void remove(const Iter& i) { removeAt(i.i); }
	inline Vector<T>& operator+=(const T x) { add(x); return *this; }
	inline Vector<T>& operator-=(const T x) { remove(x); return *this; }
	void copy(const Vector& vec)
		{	if(!tab || vec.cnt > cap) { if(tab) deleteVec(tab, cap); cap = vec.cap; tab = newVec(vec.cap); }
			cnt = vec.cnt; array::copy(tab, vec.tab, cnt); }
	inline Vector<T>& operator=(const Vector& vec) { copy(vec); return *this; };

	// Array concept
	inline int length(void) const { return count(); }
	inline const T& get(int i) const
		{ ASSERTP(0 <= i && i < cnt, "index out of bounds"); return tab[i]; }
	inline int indexOf(const T& v, int p = 0) const
		{ ASSERTP(0 <= p && p <= cnt, "index out of bounds");
		for(int i = p; i < cnt; i++) if(E::isEqual(v, tab[i])) return i; return -1; }
	inline int lastIndexOf(const T& v, int p = -1) const
		{	ASSERTP(p <= cnt, "index out of bounds");
			for(int i = (p < 0 ? cnt : p) - 1; i >= 0; i--) if(E::isEqual(v, tab[i])) return i; return -1; }
	inline const T & operator[](int i) const { return get(i); }

	// MutableArray concept
	inline void shrink(int l)
		{ ASSERTP(0 <= l && l <= cnt, "bad shrink value"); cnt = l; }
	inline void set(int i, const T& v)
		{ ASSERTP(0 <= i && i < cnt, "index out of bounds"); tab[i] = v; }
	inline void set (const Iter &i, const T &v) { set(i.i, v); }
	inline T& get(int index)
		{ ASSERTP(index < cnt, "index out of bounds"); return tab[index]; }
	inline T& get(const Iter& i) { return get(i.index()); }
	inline T & operator[](int i) { return get(i); }
	inline T & operator[](const Iter& i) { return get(i); }
	void insert(int i, const T& v)
		{	ASSERTP(0 <= i && i <= cnt, "index out of bounds");
			if(cnt >= cap) grow(cap * 2); array::move(tab + i + 1, tab + i, cnt - i);
			tab[i] = v; cnt++; }
	inline void insert(const Iter &i, const T &v) { insert(i.i, v); }
	void removeAt(int i)
		{ ASSERTP(0 <= i && i <= cnt, "index out of bounds");
		  array::move(tab + i, tab + i + 1, cnt - i - 1); cnt--; }
	inline void removeAt(const Iter& i) { removeAt(i.i); }

	// List concept
	inline const T& first(void) const { ASSERT(cnt > 0); return tab[0]; }
	inline const T& last(void) const { ASSERT(cnt > 0); return tab[cnt - 1]; }
	inline Iter find(const T &v) const
		{ Iter i(*this); while(i() && !E::equals(*i, v)) i++; return i; }
	inline Iter find (const T &v, const Iter &p) const
		{ Iter i(p); while(i() && !E::equals(*i, v)) i++; return i; }
	inline const T& nth(int i) const { return get(i); }

	// MutableList concept
	inline T& first() { ASSERT(cnt > 0); return tab[0]; }
	inline T& last() { ASSERT(cnt > 0); return tab[cnt - 1]; }
	inline void addFirst(const T &v) { insert(0, v); }
	inline void addLast(const T &v) { add(v); }
	inline void removeFirst(void) { removeAt(0); }
	inline void removeLast(void) { removeAt(cnt - 1); }
	inline void addAfter(const Iter &i, const T &v) { insert(i.i + 1, v); }
	inline void addBefore(const Iter &i, const T &v) { insert(i.i, v); }
	inline void removeBefore(const Iter& i) { removeAt(i.i - 1); }
	inline void removeAfter(const Iter& i) { removeAt(i.i + 1); }

	// Stack concept
	inline const T &top(void) const { return last(); }
	inline T &top(void) { return tab[cnt - 1]; }
	inline T pop(void) { ASSERTP(cnt > 0, "no more data to pop"); cnt--; return tab[cnt]; }
	inline void push(const T &v) { add(v); }
	inline void reset(void) { clear(); }

	// deprecated
	inline Iter operator*(void) const { return items(); }
	inline Iter items(void) const { return Iter(*this); }

private:
	T *tab;
	int cap, cnt;
};

template <class T, class E, class A>
const Vector<T, E, A> Vector<T, E, A>::null;

}	// elm

#endif /* INCLUDE_ELM_DATA_VECTOR_H_ */
