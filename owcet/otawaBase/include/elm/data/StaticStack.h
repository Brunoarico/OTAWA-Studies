/*
 *	StaticStack class
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
#ifndef ELM_DATA_STATICSTACK_H_
#define ELM_DATA_STATICSTACK_H_

#include <elm/array.h>
#include <elm/assert.h>
#include <elm/PreIterator.h>

namespace elm {

template <class T, int N>
class StaticStack {
public:
	inline StaticStack(void): _t(0) { }
	inline StaticStack(const StaticStack<T, N>& s): _t(0) { copy(s); }
	inline void copy(const StaticStack<T, N>& s)
		{ _t = s._t; array::copy(_s, s._s, _t); }
	inline StaticStack<T, N>& operator=(const StaticStack<T, N>& s)
		{ copy(s); return *this; }

	// stack concept
	inline bool isEmpty(void) const { return _t == 0; }
	inline const T& top(void) const { return _s[_t - 1]; }
	inline const T& pop(void) { ASSERTP(_t != 0, "empty stack"); _t--; return _s[_t]; }
	inline void push(const T& v) { ASSERTP(_t != N, "stack full"); _s[_t] = v; _t++; }
	inline void reset(void) { _t = 0; }

	// Iterator
	class Iter: public PreIterator<Iter, T> {
	public:
		inline Iter(const StaticStack& s, int i = 0): _s(s), _i(i) { }
		inline bool ended(void) const { return _i >= _s._t; }
		inline const T& item(void) const { return _s._s[_i]; }
		inline void next(void) { _i++; }
	private:
		const StaticStack& _s;
		int _i;
	};

	// Collection concept
	inline int count(void) const { return _t; }
	inline bool contains(const T& v)
		{ for(int i = _t; i >= 0; i--) if(_s[i] == v) return true; return false; }
	template <class C>
	bool containsAll(const C& coll)
		{ for(typename C::Iter i(coll); i; i++) if(!contains(*i)) return false; return true; }
	inline operator bool(void) const { return !isEmpty(); }
	inline Iter begin(void) const { return Iter(*this); }
	inline Iter end(void) const { return Iter(*this, _t); }
	inline Iter items(void) const { return begin(); }
	inline Iter operator*(void) const { return begin(); }
	inline bool equals(const StaticStack<T, N>& s) const
		{ if(_t != s._t) return false; for(int i = 0; i < _t; i++) if(_s[i] != s._s[i]) return false; return true; }
	inline bool contains(const StaticStack<T, N>& s) const
		{ for(int i = 0; i < s._t; i++) if(!contains(s._s[i])) return false; return true; }
	inline bool operator==(const StaticStack<T, N>& s) const { return equals(s); }
	inline bool operator!=(const StaticStack<T, N>& s) const { return !equals(s); }
	inline bool operator<=(const StaticStack<T, N>& s) const { return s.contains(*this); }
	inline bool operator>=(const StaticStack<T, N>& s) const { return contains(s); }
	inline bool operator<(const StaticStack<T, N>& s) const { return !equals(s) && s.contains(*this); }
	inline bool operator>(const StaticStack<T, N>& s) const { return !equals(s) && contains(s); }

private:
	T _s[N];
	int _t;
};

}	// otawa

#endif /* ELM_DATA_STATICSTACK_H_ */
