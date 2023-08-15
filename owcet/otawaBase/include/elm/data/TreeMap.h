/*
 *	TreeMap class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-07, IRIT UPS.
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
#ifndef ELM_DATA_TREEMAP_H
#define ELM_DATA_TREEMAP_H

#include <elm/data/TreeBag.h>
#include <elm/data/util.h>

namespace elm {

// SortedBinMap class
template <class K, class T, class C = Comparator<K>, class E = Equiv<T>, class A = DefaultAlloc >
class TreeMap: public E {
	typedef Pair<K, T> value_t;
	typedef TreeBag<value_t, AssocComparator<K, T, C> > tree_t;
public:
	inline TreeMap() { }
	inline TreeMap(const TreeMap<K, T, C>& map): tree(map.tree) { }
	inline const Comparator<K>& comparator() const { return tree.comparator(); }
	inline Comparator<K>& comparator() { return tree.comparator(); }
	inline E& equivalence() { return *this; }
	inline A& allocator() { return tree.allocator(); }
	
	// Collection concept
	inline int count(void) const { return tree.count(); }
	inline bool contains(const K &k) const { return tree.find(key(k)) != nullptr; }
	inline bool isEmpty(void) const { return tree.isEmpty(); }
 	inline operator bool(void) const { return !isEmpty(); }

	class Iter: public PreIterator<Iter, const T&> {
		friend class TreeMap;
	public:
		inline Iter(const TreeMap& map): iter(map.tree) { }
		inline bool ended(void) const { return iter.ended(); }
		inline void next(void) { iter.next(); }
		inline const T &item(void) const { return iter.item().snd; }
		inline bool equals(const Iter& ii) const { return iter.equals(ii.iter); }
	private:
		inline Iter(const typename  tree_t::Iter& i): iter(i) { }
		typename tree_t::Iter iter;
	};
	inline Iter begin() const { return Iter(tree.begin()); }
	inline Iter end() const { return Iter(tree.end()); }
	
	// Map concept
	inline const T& get(const K &key, const T &def) const {
		const value_t *val = tree.find(pair(key, T()));
		return val ? val->snd : def;
	}
	inline Option<T> get(const K &key) const {
		const value_t *res = tree.find(pair(key, T()));
		return res ? Option<T>(res->snd) : none;
	}
	inline bool hasKey(const K &k) const { return tree.contains(key(k)); }

	class KeyIter: public PreIterator<KeyIter, const K&> {
		friend class TreeMap;
	public:
		inline KeyIter(const TreeMap& map): iter(map.tree) { }
		inline bool ended(void) const { return iter.ended(); }
		inline void next(void) { iter.next(); }
		const K &item(void) const { return iter.item().fst; }
		inline bool equals(const KeyIter& ii) const { return iter.equals(ii.iter); }
	private:
		inline KeyIter(const Iter& i): iter(i.iter) { }
		typename tree_t::Iter iter;
	};
	inline Iterable<KeyIter> keys() const { return subiter(KeyIter(*this), KeyIter(end())); }
	
	class PairIter: public PreIterator<PairIter, const value_t&> {
		friend class TreeMap;
	public:
		inline PairIter(const TreeMap& map): iter(map.tree) { }
		inline bool ended(void) const { return iter.ended(); }
		inline void next(void) { iter.next(); }
		const value_t &item(void) const { return iter.item(); }
		inline bool equals(const PairIter& ii) const { return iter.equals(ii.iter); }
	private:
		inline PairIter(const Iter& i): iter(i.iter) { }
		typename tree_t::Iter iter;
	};
	inline Iterable<PairIter> pairs() const { return subiter(PairIter(*this), PairIter(end())); }
	
	// MutableMap concept
	inline void put(const K& key, const T& value)
		{ tree.add(value_t(key, value)); }
	inline void remove(const K& key)
		{ tree.remove(pair(key, T())); }
	inline void remove(const Iter& iter)
		{ tree.remove(iter.iter); }

private:
	inline Pair<K, T> key(const K& k) const { return pair(k, T()); }
	tree_t tree;
};

} // elm

#endif // ELM_DATA_TREEMAP_H
