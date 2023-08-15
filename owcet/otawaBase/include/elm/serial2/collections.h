/*
 * $Id$
 * Copyright (c) 2006, IRIT UPS.
 *
 * Serialization of usual collections.
 */
#ifndef ELM_SERIAL2_COLLECTION_H
#define ELM_SERIAL2_COLLECTION_H

#include <elm/assert.h>
#include <elm/serial2/serial.h>
#include <elm/data/Vector.h>

namespace elm { namespace serial2 {

template <class T> class Vector;

template <class C, class T>
class CollectionSerializer: public rtti::Type, public rtti::Serializable {

public:
	static CollectionSerializer __type;

	//static inline rtti::Type& type(void) { return __type; }
	static inline rtti::Type& type(const C& v) { return __type; }

	inline CollectionSerializer(void): rtti::Type("") { }
	virtual void *instantiate(void) const { return new C(); };

	virtual void unserialize(Unserializer& unserializer, void *object) const {
		C &coll = *static_cast<C *>(object);
		int cnt = unserializer.countItems();
		unserializer.beginCompound(&coll);
		for (int i = 0; i < cnt; i++) {
			T item;
			__unserialize(unserializer, item);
			coll.add(item);
		}
		unserializer.endCompound(&coll);
	}

	virtual void serialize(Serializer& serializer, const void *object) const {
		const C &coll = *static_cast<const C *>(object);
		serializer.beginCompound(&coll);
		for (typename C::Iter iter(coll); iter; iter++) {
			serializer.onItem();
			__serialize(serializer, *iter);
		}
		serializer.endCompound(&coll);
	}

	static inline void serialize(Serializer& s, const C& v) { serialize(s, &v); }
	static inline void unserialize(Unserializer& s, C& v) { unserialize(s, &v); }
};

template <class C, class T>
CollectionSerializer<C, T> CollectionSerializer<C, T>::__type;

// specialization for Vector
template <class T> struct from_class<Vector<T> >: public CollectionSerializer<Vector<T>, T> { };


// special case for arrays
template <class T>
void __serialize(Serializer& serializer, const Array<T>& tab) {
	serializer.beginCompound(&tab);
	for(int i = 0; i < tab.count(); i++) {
		serializer.onItem();
		__serialize(serializer, tab[i]);
	}
	serializer.endCompound(&tab);
}

template <class T>
void __unserialize(Unserializer& serializer, AllocArray<T>& tab) {
	int cnt = serializer.countItems();
	if(cnt != 0) {
		tab = AllocArray<T>(cnt);
		ASSERT(serializer.beginCompound(&tab));
		for(int i = 0; i < cnt; i++) {
			__unserialize(serializer, tab[i]);
			serializer.nextItem();
		}
		serializer.endCompound(&tab);
	}
}

template <class T>
void __serialize(Serializer& serializer, const AllocArray<T>& tab)
	{ __serialize(serializer, static_cast<const Array<T> &>(tab)); }




// Collection serialization

// common template
template <template <class I> class Coll, class T>
class CollecAC: public rtti::Type {

public:
	static CollecAC __type;
	
	static inline rtti::Type& type(void) { return __type; }
	static inline rtti::Type& type(const Coll<T>& v) { return __type; }

	inline CollecAC(): rtti::Type("Collection") {
	};
	
	virtual void *instantiate(void) { return new Coll<T>(); }; 
	
	virtual void unserialize(Unserializer& unserializer, void *object) {
	
		Coll<T> &coll = *static_cast<Coll<T> *>(object);
		int cnt = unserializer.countItems();
		unserializer.beginCompound(&coll);
		for (int i = 0; i < cnt; i++) {
			T item;
			__unserialize(unserializer, item);
			coll.add(item);
		}
		unserializer.endCompound(&coll);
	}
	
	virtual void serialize(Serializer& serializer, const void *object) {
		const Coll<T> &coll = *static_cast<const Coll<T> *>(object);
		serializer.beginCompound(&coll);
		
		for (typename Coll<T>::Iterator iter(coll); iter; iter++) {
			serializer.onItem();
			__serialize(serializer, *iter);
		}
		
		serializer.endCompound(&coll);
	}
	
	static inline void serialize(Serializer& s, const Coll<T>& v) { serialize(s, &v); }
	static inline void unserialize(Unserializer& s, Coll<T>& v) { unserialize(s, &v); }
};

template <template <class I> class Coll, class T>
CollecAC<Coll,T> CollecAC<Coll,T>::__type;



} } // elm::serial2

#endif // ELM_SERIAL2_COLLECTION_H


