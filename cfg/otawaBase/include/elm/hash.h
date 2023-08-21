/*
 *	hash classes
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006, IRIT UPS.
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
#ifndef ELM_HASH_H_
#define ELM_HASH_H_


#include <elm/types.h>
#include <elm/string.h>
#include <elm/util/Option.h>
#include <elm/util/Pair.h>
#include <elm/equiv.h>

namespace elm {

namespace sys { class Path; }
namespace t { typedef t::intptr hash; }

// Useful hash functions
t::hash hash_string(const char *chars, int length);
t::hash hash_cstring(const char *chars);
t::hash hash_jenkins(const void *block, int size);
inline t::hash hash_ptr(const void *p) {
#	ifdef ELM_32
		return t::hash(p) >> 2;
#	else
		return t::hash(p) >> 3;
#	endif
}
bool hash_equals(const void *p1, const void *p2, int size);

// HashKey class
template <class T> class HashKey {
public:
	static t::hash hash(const T& key) { return hash_jenkins(&key, sizeof(T)); };
	static inline bool equals(const T& key1, const T& key2) { return &key1 == &key2 || Equiv<T>::equals(key1, key2); }
	inline t::hash computeHash(const T& key) const { return hash(key); }
	inline bool isEqual(const T& key1, const T& key2) const { return equals(key1, key2); }
};

// Predefined hash keys
template <> class HashKey<int> {
public:
	static inline t::hash hash(int key) { return t::hash(key); }
	static inline bool equals(int key1, int key2) { return key1 == key2; }
	inline t::hash computeHash(int key) const { return hash(key); }
	inline bool isEqual(int key1, int key2) const { return equals(key1, key2); }
};

template <class T>
class HashKey<T *> {
public:
	static inline t::hash hash(T *key) { return hash_ptr(key); }
	static inline bool equals(T *key1, T *key2) { return key1 == key2; }
	inline t::hash computeHash(T *key) const { return hash(key); }
	inline bool isEqual(T *key1, T *key2) const { return equals(key1, key2); }
};

template <class T>
class HashKey<const T *> {
public:
	static inline t::hash hash(const T *key) { return hash_ptr(key); }
	static inline bool equals(const T *key1, const void *key2)
		{ return key1 == key2; }
	inline t::hash computeHash(const T *key) const { return hash(key); }
	inline bool isEqual(const T *key1, const T *key2) const { return equals(key1, key2); }
};

template <> class HashKey<CString> {
public:
	static t::hash hash(CString key) { return hash_cstring(key.chars()); }
	static inline bool equals(CString key1, CString key2) { return key1 == key2; }
	inline t::hash computeHash(cstring key) const { return hash(key); }
	inline bool isEqual(cstring key1, cstring key2) const { return equals(key1, key2); }
};

template <> class HashKey<String> {
public:
	static t::hash hash(const String& key) { return hash_string(key.chars(), key.length()); };
	static inline bool equals(const String& key1, const String& key2) { return key1 == key2; };
	inline t::hash computeHash(string key) const { return hash(key); }
	inline bool isEqual(string key1, string key2) const { return equals(key1, key2); }
};

template <class T1, class T2> class HashKey<Pair<T1, T2> > {
public:
	typedef Pair<T1, T2> T;
	static t::hash hash(const T& p) { return HashKey<T1>::hash(p.fst) + HashKey<T2>::hash(p.snd); };
	static inline bool equals(const T& p1, const T& p2) { return p1 == p2; };
	inline t::hash computeHash(const T& key) const { return hash(key); }
	inline bool isEqual(const T& key1, const T& key2) const { return equals(key1, key2); }
};


// Hasher class
class Hasher {
public:
	inline Hasher(void): h(0) { }
	template <class T> void add(const T& value) { h = h ^ HashKey<T>::hash(value); }
	template <class T> Hasher& operator+=(const T& value) { add<T>(value); return *this; }
	template <class T> Hasher& operator<<(const T& value) { add<T>(value); return *this; }
	inline t::hash hash(void) const { return h; }
	inline operator t::hash(void) const { return h; }
private:
	t::hash h;
};

// SelfHashKey class
template <class T>
class SelfHashKey {
public:
	static t::hash hash(const T& v) { return v.hash(); }
	static bool equals(const T& v1, const T& v2) { return v1 == v2; }
	inline t::hash computeHash(const T& key) const { return hash(key); }
	inline bool isEqual(const T& key1, const T& key2) const { return equals(key1, key2); }
};


template <class K, class T, class H = HashKey<K> >
class AssocHashKey: public H {
public:
	typedef Pair<K, T> t;
	const H& keyHash() const { return *this; }
	H& keyHash() { return *this; }

	inline elm::t::hash computeHash(const t& k) const { return H::computeHash(k.fst); }
	inline bool isEqual(const t& k1, const t& k2) const { return H::isEqual(k1.fst, k2.fst); }
};

template <> class HashKey<sys::Path> {
public:
	static t::hash hash(const sys::Path& key);
	static bool equals(const sys::Path& key1, const sys::Path& key2);
	inline t::hash computeHash(const sys::Path& key) const { return hash(key); }
	inline bool isEqual(const sys::Path& key1, const sys::Path& key2) const { return equals(key1, key2); }
};

template <class T> inline t::hash hash(const T& x) { return HashKey<T>::hash(x); }

};	// elm

#endif /* ELM_HASH_H_ */
