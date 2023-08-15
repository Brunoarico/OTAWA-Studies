/*
 * serial support for data structures
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
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
#ifndef ELM_SERIAL2_DATA_H_
#define ELM_SERIAL2_DATA_H_

#include <elm/assert.h>
#include <elm/serial2/serial.h>

namespace elm {

template <class T, class E, class A> class Vector;

namespace serial2 {

template <class C, class T>
class DataSerializer: public rtti::Type {

public:
	static DataSerializer __type;

	static inline rtti::Type& type(void) { return __type; }
	static inline rtti::Type& type(const C& v) { return __type; }

	inline DataSerializer(void): rtti::Type("") { }
	virtual void *instantiate(void) const { return new C(); };

	virtual void unserialize(Unserializer& unserializer, void *object) {
		C &coll = *static_cast<C *>(object);
		int cnt = unserializer.countItems();
		unserializer.beginCompound(&coll);
		for (int i = 0; i < cnt; i++) {
			__unserialize(unserializer, coll.addNew());
			unserializer.nextItem();
		}
		unserializer.endCompound(&coll);
	}

	virtual void serialize(Serializer& serializer, const void *object) {
		const C &coll = *static_cast<const C *>(object);
		serializer.beginCompound(&coll);
		for (typename C::Iter iter(coll); iter(); iter++) {
			serializer.onItem();
			__serialize(serializer, *iter);
		}
		serializer.endCompound(&coll);
	}

	static inline void serialize(Serializer& s, const C& v) { __type.serialize(s, &v); }
	static inline void unserialize(Unserializer& s, C& v) { __type.unserialize(s, &v); }
};

template <class C, class T>
DataSerializer<C, T> DataSerializer<C, T>::__type;

// specialization
template <class T, class M> struct from_class<elm::Vector<T, M> >: public DataSerializer<Vector<T, M>, T> { };

} }		// elm::serial2

#endif /* INCLUDE_ELM_SERIAL2_DATA_H_ */
