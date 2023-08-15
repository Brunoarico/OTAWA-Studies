/*
 *	Array classes interface
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
#ifndef ELM_ARRAY_TABLE_H_
#define ELM_ARRAY_TABLE_H_

#include <elm/assert.h>
#include <elm/PreIterator.h>
#include <elm/array.h>
#include <elm/compare.h>
#include <elm/data/util.h>

namespace elm {

template <class T>
class Array {
public:
	typedef T t;
	typedef Array<T> self_t;

	inline Array(void): cnt(0), buf(0) { }
	inline Array(int count, T *buffer): cnt(count), buf(buffer) { }

	inline const T *buffer(void) const { return buf; }
	inline T *buffer(void) { return buf; }
	inline int size(void) const { return count(); }
	inline void set(int count, T *buffer) { cnt = count; buf = buffer; }
	inline void set(const Array<T>& t) { cnt = t.cnt; buf = t.buf; }
	inline void copy(const Array& t) { cnt = min(cnt, t.cnt); array::copy(buf, t.buf, cnt); }
	inline void fill(const T& val) { array::set(buf, cnt, val); }
	inline const T *operator()(void) const { return buffer(); }
	inline T *operator()(void) { return buffer(); }
	inline Array<T>& operator=(const Array<T>& t) { set(t); return *this; }

	class PreIter: public elm::PreIter<PreIter, T> {
		friend class Array;
	public:
		inline PreIter(): p(0), t(0) { }
		inline PreIter(const Array<T>& table): p(table.buffer()), t(table.buffer() + table.count()) { }
		inline PreIter(const Array<T>& table, bool end): p(table.buffer() + (end ? table.count() : 0)), t(table.buffer() + table.count()) { }
		inline bool ended(void) const { return p >= t; }
		inline void next(void) { p++; }
		inline bool equals(const PreIter& i) const { return p == i.p && t == i.t; }
	protected:
		const T *p, *t;
	};

	class Iter: public PreIter, public ConstPreIter<Iter, T> {
	public:
		using PreIter::PreIter;
		inline const T& item() const { ASSERT(PreIter::p < PreIter::t); return *PreIter::p; }
	};

	class MutIter: public PreIter, public MutPreIter<MutIter, T> {
	public:
		inline MutIter(self_t& a): PreIter(a) { }
		inline MutIter(self_t& a, bool e): PreIter(a, e) { }
		inline T& item() const { ASSERT(PreIter::p < PreIter::t); return *const_cast<T *>(PreIter::p); }
	};
	
	class BackPreIter: public elm::PreIter<BackPreIter, T> {
	public:
		inline BackPreIter(): p(nullptr), t(nullptr) { }
		inline BackPreIter(const Array<T>& table): p(table.buffer() + table.count() - 1), b(table.buffer()) { }
		inline BackPreIter(const Array<T>& table, bool end): p(table.buffer() + (end ? 0 : table.count()) - 1), b(table.buffer()) { }
		inline bool ended(void) const { return p < b; }
		inline void next(void) { p--; }
		inline bool equals(const BackPreIter& i) const { return p == i.p && b == i.b; }
	protected:
		const T *p, *b;
	};

	class BackIter: public BackPreIter, public ConstPreIter<BackIter, T> {
	public:
		using BackPreIter::BackPreIter;
		inline const T& item() const { ASSERT(BackPreIter::p < BackPreIter::t); return *BackPreIter::p; }
	};

	class BackMutIter: public BackPreIter, public MutPreIter<BackMutIter, T> {
	public:
		inline BackMutIter(self_t& a): BackPreIter(a) { }
		inline BackMutIter(self_t& a, bool e): BackPreIter(a, e) { }
		inline T& item() const { ASSERT(BackPreIter::p >= BackPreIter::b); return *const_cast<T *>(BackPreIter::p); }
	};
	
	// Collection concept
	static const Array<T> null;
	inline Iter items(void) const { return Iter(*this); }
	inline Iter operator*(void) const { return items(); }
	inline Iter begin(void) const { return items(); }
	inline Iter end(void) const { return Iter(*this, true); }
	inline Iterable<BackIter> back() const { return subiter(BackIter(*this), BackIter(*this, true)); }
	
	inline int count(void) const { return cnt; }
	inline bool contains(const T& item)
		{ for(auto x: *this) if(x == item) return true; return false; }
	template <class C> inline bool containsAll(const C& c)
		{ for(auto x: c) if(!contains(x)) return false; return true; }
	inline bool isEmpty(void) const { return cnt == 0; }
	inline operator bool(void) const { return !isEmpty(); }
	inline bool equals(const Array<T>& a) const {
		if(cnt != a.cnt) return false;
		for(auto i = begin(), j = a.begin(); i(); ++i, ++j)
			if(*i != *j) return false;
		return true;
	}
	inline bool operator==(const Array<T>& a) const { return equals(a); }
	inline bool operator!=(const Array<T>& a) const { return !equals(a); }
	inline bool operator<=(const Array<T>& a) const { return a.containsAll(*this); }
	inline bool operator<(const Array<T>& a) const { return a.containsAll(*this) && !equals(a); }
	inline bool operator>=(const Array<T>& a) const { return containsAll(a); }
	inline bool operator>(const Array<T>& a) const { return containsAll(a) && !equals(a); }

	// Array concept
	inline int length(void) const { return count(); }
	inline const T& get(int idx) const { ASSERT(0 <= idx && idx < cnt); return buf[idx]; }
	inline int indexOf(const T& v, int i = 0) const
		{ for(; i < count(); i++) if(v == get(i)) return i; return -1; }
	inline int lastIndexOf(const T& v, int i = -1) const
		{ if(i < 0) i = count() - 1; for(; i >= 0; i--) if(v == get(i)) return i; return -1; }
	inline const T& operator[](int idx) const { return get(idx); }

	// MutableArray concept
	inline void set(int idx, const T& val) { ASSERT(0 <= idx && idx < cnt); buf[idx] = val; }
	inline void set(const MutIter& i, const T& val) { ASSERT(!i.ended()); *i = val; }
	inline T& get(int idx) { ASSERT(0 <= idx && idx < cnt); return buf[idx]; }
	inline T& operator[](int idx) { return get(idx); }
	inline MutIter begin(void) { return MutIter(*this); }
	inline MutIter end(void) { return MutIter(*this, true); }
	inline Iterable<BackMutIter> back() { return subiter(BackMutIter(*this), BackMutIter(*this, true)); }


protected:
	int cnt;
	T *buf;
};

template <class T>
const Array<T> Array<T>::null;


template <class T>
class AllocArray: public Array<T> {
public:
	inline AllocArray(void) { }
	inline AllocArray(int count, T *buffer): Array<T>(count, buffer) { }
	inline AllocArray(int count): Array<T>(count, new T[count]) { }
	inline AllocArray(int count, const T& val): Array<T>(count, new T[count]) { fill(val); }
	inline AllocArray(const Array<T>& t): Array<T>(t.count(), new T[t.count()]) { Array<T>::copy(t); }
	inline AllocArray(const AllocArray<T>& t): Array<T>(t.cnt, new T[t.cnt]) { Array<T>::copy(t); }
	inline ~AllocArray(void) { if(this->buf) delete [] this->buf; }

	inline void copy(const Array<T>& t)
		{ if(this->count() < t.count()) { if(this->buf) delete [] this->buf;
		  Array<T>::set(t.count(), new T[t.count()]); } Array<T>::copy(t); }
	inline void tie(int cnt, T *buffer) { if(this->buf) delete [] this->buf; Array<T>::set(cnt, buffer); }
	inline void tie(const Array<T>& t) { if(this->buf) delete [] this->buf; Array<T>::set(t); }

	inline AllocArray<T>& operator=(const Array<T>& t) { copy(t); return *this; }
	inline AllocArray<T>& operator=(const AllocArray<T>& t) { copy(t); return *this; }
};

template <class T>
inline Array<T> _array(int n, T t[]) { return Array<T>(n, t); }

} // elm

#endif /* ELM_ARRAY_TABLE_H_ */
