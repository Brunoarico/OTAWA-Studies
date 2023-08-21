/*
 *	avl::Map class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2011, IRIT UPS.
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
#ifndef ELM_AVL_MAP_CPP_
#define ELM_AVL_MAP_CPP_

#include <elm/delegate.h>
#include <elm/util/Option.h>
#include <elm/avl/GenTree.h>
#include <elm/data/util.h>

namespace elm { namespace avl {

// Map class
template <class K, class T, class C = Comparator<K>, class E = Equiv<T>, class A = DefaultAlloc >
class Map: public E {
	typedef Pair<typename ti<K>::embed_t, typename ti<T>::embed_t> pair_t;
	typedef GenTree<pair_t, PairAdapter<K, T>, C, A > tree_t;

public:
	typedef Map<K, T, C, E, A> self_t;

	inline const C& comparator() const { return tree.comparator(); }
	inline C& comparator() { return tree.comparator(); }
	inline const C& allocatr() const { return tree.allocator(); }
	inline C& allocator() { return tree.allocator(); }
	inline const E& equivalence() const { return *this; }
	inline E& equivalence() { return *this; }

	// Collection concept
	inline int count(void) const { return tree.count(); }
	inline bool contains(const T& x) const
		{ for(const auto y: *this) if(!E::isEqual(x, y)) return false; return true; }
	template <class CC> bool containsAll(const CC& c) const
		{ for(const auto x: c) if(!contains(x)) return false; return true; }
	inline bool isEmpty(void) const { return tree.isEmpty(); }
	inline operator bool() const { return !isEmpty(); }

	class Iter: public PreIterator<Iter, T> {
	public:
		inline Iter() { }
		inline Iter(const self_t& t): i(t.tree) { }
		inline bool ended() const { return i.ended(); }
		inline const T& item() const { return i.item().snd; }
		inline void next() { i.next(); }
		inline bool equals(const Iter& ii) const { return i.equals(ii.i); }
	private:
		typename tree_t::Iter i;
	};
	inline Iter begin() const { return Iter(*this); }
	inline Iter end() const { return Iter(); }

	inline bool equals(const Map<K, T, C>& map) const { return tree.equals(map.tree); }
	inline bool operator==(const Map<K, T, C>& map) const { return equals(map); }
	inline bool operator!=(const Map<K, T, C>& map) const { return !equals(map); }

	// Map concept
	inline Option<T> get(const K& key) const
		{ const pair_t *p = tree.get(key); if(!p) return none; else return some(p->snd); }
	inline const T& get(const K& key, const T& def) const
		{ const pair_t *p = tree.get(key); if(!p) return def; else return p->snd; }
	inline bool hasKey(const K& key) const
		{ const pair_t *p = tree.get(key); return p; }
	inline const T& operator[](const K& k) const
		{ const pair_t *r = tree.get(k); if(r == nullptr) throw KeyException(); return r->snd; }

	class KeyIter: public PreIterator<KeyIter, K> {
	public:
		inline KeyIter() { }
		inline KeyIter(const self_t& map): it(map.tree) { }
		inline bool ended(void) const { return it.ended(); }
		inline void next(void) { it.next(); }
		inline const K& item(void) const { return it.item().fst; }
		inline bool equals(const KeyIter& i) const { return it.equals(i.it); }
	private:
		typename tree_t::Iter it;
	};
	inline Iterable<KeyIter> keys() const { return subiter(KeyIter(*this), KeyIter()); }

	class PairIter: public tree_t::Iter {
	public:
		inline PairIter() { }
		inline PairIter(const self_t& map): tree_t::Iter(map.tree) { }
	};
	inline Iterable<PairIter> pairs() const { return subiter(PairIter(*this), PairIter()); }

	// MutableMap concept
	inline void put(const K &key, const T &value) { tree.set(pair_t(key, value)); }
	inline void remove(const K &key) { tree.removeByKey(key); }
	inline void remove(const Iter &i) { tree.remove(i.item().fst); }

	///
	inline void clear(void) { tree.clear(); }
	inline void copy(const Map<K, T, C>& map) { tree.copy(map.tree); }
	inline Map<K, T, C>& operator=(const Map<K, T, C>& map) { copy(map); return *this; }

private:
	tree_t tree;
};

} }		// elm::avl

#endif /* ELM_AVL_AVLMAP_CPP_ */
