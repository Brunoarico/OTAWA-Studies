/*
 *	HashMap class interface
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
#ifndef ELM_DATA_HASHMAP_H_
#define ELM_DATA_HASHMAP_H_
#define ELM_STAT

#include "HashTable.h"
#include "util.h"
#include <elm/delegate.h>

namespace elm {

template <class K, class T, class H = HashKey<K>, class A = DefaultAlloc, class E = Equiv<T> >
class HashMap: public E {
	typedef HashTable<Pair<K, T>, AssocHashKey<K, T, H>, A> tab_t;
public:
	typedef K key_t;
	typedef T val_t;
	typedef HashMap<K, T, H, A, E> self_t;

	inline HashMap(int _size = 211): _tab(_size) { }
	inline HashMap(const self_t& h): _tab(h._tab) { }
	inline const H& hash() const { return _tab.hash().keyHash(); }
	inline H& hash() { return _tab.hash().keyHash(); }
	inline const A& allocator() const { return _tab.allocator(); }
	inline A& allocator() { return _tab.allocator(); }
	inline const E& equivalence() const { return *this; }
	inline E& equivalence() { return *this; }

	inline void clear(void) { _tab.clear(); }
	inline void add(const K& key, const T& val) { _tab.add(pair(key, val)); }

	inline T& fetch(const K& k)
		{ auto *n = _tab.get(key(k)); if(n != nullptr) return n->snd; return _tab.add(pair(k, T()))->snd; }

	// Map concept
	inline Option<T> get(const K& k) const
		{ auto *r = _tab.get(key(k)); if(r) return some(r->snd); else return none; }
	inline const T& get(const K& k, const T& def) const
		{ auto p = key(k); auto r = _tab.get(p); if(r) return r->snd; else return def; }
	inline bool hasKey(const K& k) const { return _tab.hasKey(key(k)); }

	inline Option<T> get_const(const K& k) const
		{ auto *r = _tab.get_const(key(k)); if(r) return some(r->snd); else return none; }
	inline const T& get_const(const K& k, const T& def) const
		{ auto r = _tab.get_const(key(k)); if(r) return r->snd; else return def; }
	inline bool hasKey_const(const K& k) const { return _tab.hasKey_const(key(k)); }

	class KeyIter: public InplacePreIterator<KeyIter, K> {
	public:
		inline KeyIter(const self_t& htab): i(htab._tab) { };
		inline KeyIter(const self_t& htab, bool end): i(htab._tab, end) { };
		inline bool ended(void) const { return i.ended(); }
		inline const K& item(void) const { return i.item().fst; }
		inline void next(void) { i.next(); }
		inline bool equals(const KeyIter& it) const { return i.equals(it.i); }
	private:
		typename tab_t::Iter i;
	};
	inline Iterable<KeyIter> keys() const { return subiter(KeyIter(*this), KeyIter(*this, true)); }

	class PairIter: public InplacePreIterator<PairIter, Pair<K, T> > {
	public:
		inline PairIter(const self_t& htab): i(htab._tab) { };
		inline PairIter(const self_t& htab, bool end): i(htab._tab, end) { };
		inline bool ended(void) const { return i.ended(); }
		inline const Pair<K, T>& item(void) const { return i.item(); }
		inline void next(void) { i.next(); }
		inline bool equals(const PairIter& it) const { return i.equals(it.i); }
	private:
		typename tab_t::Iter i;
	};
	inline Iterable<PairIter> pairs() const { return subiter(PairIter(*this), PairIter(*this, true)); }

	// Collection concept
	inline int count() const { return _tab.count(); }
	inline bool isEmpty() const { return _tab.isEmpty(); }
	inline operator bool() const { return !isEmpty(); }

	class Iter: public InplacePreIterator<Iter, T> {
		friend class HashMap;
	public:
		inline Iter(const self_t& htab): i(htab._tab) { };
		inline Iter(const self_t& htab, bool end): i(htab._tab, end) { };
		inline bool ended(void) const { return i.ended(); }
		inline const T& item(void) const { return i.item().snd; }
		inline void next(void) { i.next(); }
		inline const K& key(void) const { return i.item().fst; }
		inline bool equals(const Iter& it) const { return i.equals(it.i); }
	private:
		typename tab_t::Iter i;
	};
	inline Iter begin(void) const { return Iter(*this); }
	inline Iter end(void) const { return Iter(*this, true); }

	bool contains(const T& item) const
		{ for(const auto x: *this) if(x == item) return true; return false; }
	template <class C> bool containsAll(const C& c) const
		{ for(typename C::Iter i(c); c; i++) if(!contains(*i)) return false; return true; }

	inline bool equals(const HashMap<K, T>& t) const
		{ return containsAll(t) && t.containsAll(*this); }
	inline bool operator==(const HashMap<K, T>& t) const { return equals(t); }
	inline bool operator!=(const HashMap<K, T>& t) const { return !equals(t); }

	inline bool includes(const HashMap<K, T>& t) const
		{ return containsAll(t); }
	inline bool operator <=(const HashMap<K, T>& t) const { return t.contains(*this); }
	inline bool operator >=(const HashMap<K, T>& t) const { return contains(t); }

	inline bool operator<(const HashMap<K, T>& t) const { return !equals(t) && t.contains(*this); }
	inline bool operator>(const HashMap<K, T>& t) const { return !equals(t) && contains(t); }

	// MutableMap concept
	inline void put(const K& key, const T& val) { _tab.put(pair(key, val)); }
	inline void remove(const K& k) { _tab.remove(key(k)); }
	inline void remove(const Iter& i) { _tab.remove(i.i); }

	inline const T& operator[](const K& k) const { auto *r = _tab.get(key(k)); ASSERT(r); return (*r).snd; }
	inline StrictMapDelegate<self_t> operator[](const K& key) { return StrictMapDelegate<self_t>(*this, key); }
	inline const T& operator[](const Iter& i) const { auto *r = _tab.get(key(i.key())); ASSERT(r); return (*r).snd; }
	inline StrictMapDelegate<self_t> operator[](const Iter& i) { return StrictMapDelegate<self_t>(*this, i.key()); }

	template <class C> void putAll(const C& c)
		{ for(auto p: c.pairs()) put(p.fst, p.snd); }

#	ifdef ELM_STAT
		int minEntry(void) const { return _tab.minEntry(); }
		int maxEntry(void) const { return _tab.maxEntry(); }
		int zeroEntry(void) const { return _tab.zeroEntry(); }
		int size(void) const { return _tab.size(); }
#	endif

	// deprecated
	inline bool exists(const K& k) const { return hasKey(k); }
	inline Iter operator*(void) const { return begin(); }

private:
	inline Pair<K, T> key(const K& k) const { return pair(k, T()); }
	tab_t _tab;
};

}	// otawa

#endif /* ELM_DATA_HASHTABLE_H_ */
