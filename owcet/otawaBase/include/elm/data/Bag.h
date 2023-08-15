/*
 *	ArrayList class interface
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
#ifndef ELM_DATA_BAG_H_
#define ELM_DATA_BAG_H_

#include <elm/data/Array.h>
#include <elm/data/Vector.h>

namespace elm {

template <class T>
class Bag {
public:

	static inline void operator delete(void *p)
		{ delete [] reinterpret_cast<char *>(p); }

	inline static Bag<T> *make(int count, const T *items)
		{ Bag *b = new(count) Bag(count); array::copy(b->base(), items, count); return b; }

	inline static Bag<T> *make(const Array<T>& a)
		{ return make(a.count(), a.buffer()); }

	inline static Bag *make(const Vector<T>& v)
		{ return make(v.asArray()); }

	inline int count() const { return cnt; }
	inline bool isEmpty() const { return cnt == 0; }
	inline operator bool() const { return !isEmpty(); }
	inline const T& get(int i) const { ASSERTP(0 <= i && i < cnt, "bad index"); return *(base() + i); }
	inline T& get(int i) { ASSERTP(0 <= i && i < cnt, "bad index"); return *(base() + i); }

	inline const T& operator[](int i) const { return get(i); }
	inline T& operator[](int i) { return get(i); }

	class Iter: public PreIterator<Iter, T> {
	public:
		inline Iter(const Bag<T> *bag, int idx = 0): b(*bag), i(idx) { }
		inline bool ended() const { return i >= b->count(); }
		inline const T& item() const { return b[i]; }
		inline void next() { i++; }
		inline bool equals(const Iter& ii) const { return &b == &ii.b && i == ii.i; }
	private:
		const Bag<T>& b;
		int i;
	};

	inline Iter begin() const { return Iter(this); }
	inline Iter end() const { return Iter(this, cnt); }

	inline bool contains(const T& x) const
		{ for(auto y: *this) if(x == y) return true; return false; }
	inline bool containsAll(const Bag<T>& c) const
		{ for(auto x: c) if(!contains(x)) return false; return true; }

	inline bool equals(const Bag<T>& c) const {
		if(cnt != c.count()) return false;
		for(int i = 0; i < cnt; i++) if(get(i) != c.get(i)) return false;
		return true;
	}
	inline bool operator==(const Bag<T>& b) const { return equals(b); }
	inline bool operator!=(const Bag<T>& b) const { return !equals(b); }

	inline int length() const { return count(); }
	inline int indexOf(const T& x, int i = -1) const
		{ for(i++; i < cnt; i++) if(get(i) == x) return i; return -1; }
	inline int lastIndexOf(const T& x, int i = -1) const
		{ if(i < 0) i = cnt; i--; while(i >= 0 && x != get(i)) i--; return i; }

private:
	static inline void *operator new(std::size_t size, int cnt)
		{ return new char[size + sizeof(T) * cnt]; }

	inline Bag(int count): cnt(count) { }
	inline const T *base() const { return reinterpret_cast<const T *>(this + 1); }
	inline T *base() { return reinterpret_cast<T *>(this + 1); }
	int cnt;
};

};	// elm

#endif /* ELM_DATA_BAG_H_ */
