/*
 *	ListSet class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
 *
 *	ELM is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	ELM is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with ELM; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef ELM_DATA_LISTMAP_H_
#define ELM_DATA_LISTMAP_H_

#include "SortedList.h"

namespace elm {

template <class K, class T, class C = Comparator<K>, class E = Equiv<T>, class A = DefaultAlloc >
class ListMap: private SortedList<Pair<K, T>, AssocComparator<K, T, C>, A>, public E {
	typedef SortedList<Pair<K, T>, AssocComparator<K, T, C>, A> base_t;
public:
	typedef ListMap<K, T, C, E, A> self_t;
	typedef typename base_t::Iter PairIter;

	inline ListMap() { }
	inline ListMap(const self_t& l): base_t(l) { }
	inline E& equivalence() { return *this; }

	class PreIter: public elm::PreIter<PreIter, T> {
		friend class ListMap;
	public:
		inline PreIter() { }
		inline PreIter(const self_t& l): i(l) { }
		inline bool ended(void) const { return i.ended(); }
		inline void next(void) { i.next(); }
		inline bool equals(const PreIter& ii) const { return i.equals(ii.i); }
	protected:
		PairIter i;
	};

	class Iter: public PreIter, public ConstPreIter<Iter, T> {
	public:
		using PreIter::PreIter;
		inline const T& item() const { return (*PreIter::i).snd; }
	};

	class MutIter: public PreIter, public MutPreIter<MutIter, T> {
	public:
		using PreIter::PreIter;
		inline T& item() const { return const_cast<Pair<K, T>&>(*PreIter::i).snd; }
	};
	
	class KeyIter: public PreIterator<KeyIter, K> {
	public:
		inline KeyIter(void) { }
		inline KeyIter(const ListMap<K, T>& l): i(l) { }
		inline bool ended(void) const { return i.ended(); }
		inline const K& item(void) const { return (*i).fst; }
		inline void next(void) { i.next(); }
		inline bool equals(const KeyIter& ii) { return i.equals(ii.i); }
	private:
		PairIter i;
	};

	// Collection concept
	inline Iter begin() const { return Iter(*this); }
	inline Iter end() const { return Iter(); }
	inline int count(void) const { return base_t::count(); }
	inline bool contains(const T& v) const
		{ for(const auto& i: *this) if(E::isEqual(i, v)) return true; return false; }
	template <class CC> inline bool containsAll(const CC& c) const
		{ for(const auto i: c) if(!contains(i)) return false; return true; }
	inline bool isEmpty(void) const { return base_t::isEmpty(); }
	inline operator bool(void) const { return !isEmpty(); }
	inline Iter items(void) const { return begin(); }
	inline Iter operator*(void) const { return begin(); }
	inline operator Iter(void) const { return begin(); }
	inline bool equals(const self_t& m) const { return base_t::equals(m); }
	inline bool operator==(const self_t& m) const { return equals(m); }
	inline bool operator!=(const self_t& m) const { return !equals(m); }
	inline bool contains(const self_t& m) const { return base_t::contains(m); }
	inline bool operator<=(const self_t& m) const { return m.contains(*this); }
	inline bool operator<(const self_t& m) const { return m.contains(*this) && !equals(m); }
	inline bool operator>=(const self_t& m) const { return contains(m); }
	inline bool operator>(const self_t& m) const { return contains(m) && !equals(m); }

	// Map concept
	inline Option<T> get(const K &k) const
		{ PairIter i = lookup(k); if(i()) return some((*i).snd); else return none; }
	inline const T &get(const K &k, const T &d) const
		{ PairIter i = lookup(k); if(i()) return (*i).snd; else return d; }
	inline bool hasKey(const K &k) const { return lookup(k)(); }

	inline Iterable<KeyIter> keys() const { return subiter(KeyIter(*this), KeyIter()); }
	inline Iterable<PairIter> pairs() const { return subiter(PairIter(*this), PairIter()); }

	// MutableMap concept
	inline MutIter begin() { return MutIter(*this); }
	inline MutIter end() { return MutIter(); }
	void put(const K& k, const T& v) {
		PairIter i = lookup(k);
		if(i())
			base_t::set(i, pair(k, v));
		else
			base_t::add(pair(k, v));
	}
	inline void remove(const MutIter& i) { base_t::remove(i.i); }
	inline void remove(const K& k)
		{ PairIter i = lookup(k); if(i()) base_t::remove(*i); }

private:
	PairIter lookup(const K& k) const {
		for(PairIter i = base_t::begin(); i(); i++) {
			int cmp = base_t::comparator().compareKey(k, (*i).fst);
			if(cmp == 0)
				return i;
			else if(cmp < 0)
				break;
		}
		return base_t::end();
	}
};

template <class K, class T, class M>
inline bool operator<=(const T& v, const ListMap<K, T, M>& m) { return m.contains(v); }

}	// elm

#endif /* ELM_DATA_LISTMAP_H_ */
