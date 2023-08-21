/*
 *	$Id$
 *	delegate classes interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007-08, IRIT UPS.
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
#ifndef ELM_UTIL_DELEGATE_H
#define ELM_UTIL_DELEGATE_H

#include <elm/assert.h>
#include <elm/util/Option.h>
#include <elm/util/Exception.h>

namespace elm {

// ArrayDelegate class
template <class C, class I, class T>
class ArrayDelegate {
public:
	inline ArrayDelegate(C& container, const I& identifier)
		: cont(&container), id(identifier) { }
	inline ArrayDelegate(const ArrayDelegate& delegate)
		: cont(delegate.cont), id(delegate.id) { }
	
	inline operator T(void) const { return ((const C *)cont)->get(id); }
	inline const T& operator*(void) const { return ((const C *)cont)->get(id); }
	inline ArrayDelegate<C, I, T>& operator=(const T& value)
		{ cont->set(id, value); return *this; }
	inline ArrayDelegate<C, I, T>& operator=(const ArrayDelegate<C, I, T>& delegate)
		{ cont = delegate.cont; id = delegate.id; return *this; }
	
private:
	C *cont;
	I id;
};


// Default value
template <class T>
class Default {
public:
	static T def; 
};
template <class T> T Default<T>::def = 0;


// MapDelegate class
template <class C, class I, class T, class D = Default<T> >
class MapDelegate {
public:
	inline MapDelegate(C& container, const I& identifier)
		: cont(&container), id(identifier) { }
	inline MapDelegate(const MapDelegate& delegate)
		: cont(delegate.cont), id(delegate.id) { }
	
	inline operator T(void) const
		{ return ((const C *)cont)->get(id, D::def); }
	inline const T& operator*(void) const
		{ return ((const C *)cont)->get(id, D::def); }
	inline MapDelegate<C, I, T>& operator=(const T& value)
		{ cont->put(id, value); return *this; }
	inline MapDelegate<C, I, T>& operator=(const MapDelegate<C, I, T>& delegate)
		{ cont = delegate.cont; id = delegate.id; return *this; }
	
private:
	C *cont;
	I id;
};


// KeyException class
class KeyException: public elm::Exception {
public:
	string message(void) override;
};


// MapDelegate class
template <class C>
class StrictMapDelegate {
public:
	typedef StrictMapDelegate<C> self_t;
	typedef typename C::key_t key_t;
	typedef typename C::val_t val_t;

	inline StrictMapDelegate(C& map, const key_t& key)
		: _map(map), _key(key) { }

	inline val_t get(void) const {
		Option<val_t> v = _map.get(_key);
		if(v.isNone()) throw KeyException();
		return *v;
	}
	inline void put(const val_t& val) { _map.put(_key, val); }

	inline operator val_t(void) const { return get(); }
	inline val_t operator*(void) const { return get(); }
	inline self_t& operator=(const val_t& val) { put(val); return *this; }

private:
	C& _map;
	const key_t& _key;
};

} // elm

#endif /* ELM_UTIL_DELEGATE_H */
