/*
 *	serial module interface
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
#ifndef ELM_SERIAL2_TYPE_H
#define ELM_SERIAL2_TYPE_H

#include <elm/rtti.h>
#include <elm/meta.h>
#include <elm/type_info.h>
#include <elm/serial2/Serializer.h>
#include <elm/serial2/Unserializer.h>


namespace elm { namespace serial2 {

// for ascending compatibility only
template <class T>
class Base {
public:
	inline Base(T *p): ptr(p) { }
	inline Base(const T *p): ptr(const_cast<T *>(p)) { }
	T *ptr;
};


// AbstractClass class
class AbstractClass: public rtti::AbstractClass, public rtti::Serializable {
public:
	inline AbstractClass(CString name, const rtti::AbstractClass& base)
		: rtti::AbstractClass(name, base) { }

	virtual bool isSerial(void) const { return true; }
	virtual const Serializable& asSerial(void) const { return *this; }
	virtual const rtti::Type& type(void) const { return *this; }
};

// SerializableClass class
template <class T, class B = void>
class Class: public AbstractClass {
public:
	inline Class(CString name, const rtti::AbstractClass& base = type_of<B>().asClass())
		: AbstractClass(name, base) { };

	void *instantiate() const override { return new T(); }
	void free(void *obj) const override { delete static_cast<T *>(obj); }

	// Serializable interface
	void serialize(elm::serial2::Serializer& ser, const void *data) const override {
		__serialize(ser, *static_cast<const T *>(data));
	}

	void unserialize(Unserializer& uns, void *data) const override {
		__unserialize(uns, *static_cast<T *>(data));
	}
};


template <class T>
class Enum: public rtti::Enum {
public:
	Enum(const make& make): rtti::Enum(make) { }
	Enum(cstring name, const Value values[]): rtti::Enum(name, values) { }

	// Serialize interface
	virtual void *instantiate(void) const { return new T(T(0)); }
	virtual void serialize(serial2::Serializer& ser, const void *data) const { ser.onEnum(data, int(*static_cast<const T *>(data)), *this); }
	virtual void unserialize(serial2::Unserializer& uns, void *data) const { *static_cast<T *>(data) = T(uns.onEnum(*this)); }
};



// Class information
template <class T>
inline void __serialize_body(Serializer& s, const T *v) {
	__serialize_body(s, static_cast<const typename T::__base *>(v));
	v->__visit(s);
}
template <> inline void __serialize_body(Serializer& s, const void *v) { }

template <class T>
inline void __unserialize_body(Unserializer& s, T *v) {
	__unserialize_body(s, static_cast<typename T::__base *>(v));
	v->__visit(s);
}
template <> inline void __unserialize_body(Unserializer& s, void *v) { }

template <class T> using supports_complete =
	decltype(meta::declval<T>().__serial_complete());
template <class T> typename meta::enable_if<meta::is_supported<T, supports_complete>::_>::_
	do_complete(T& v) { v.__serial_complete(); }
template <class T> typename meta::enable_if<!meta::is_supported<T, supports_complete>::_>::_
	do_complete(T& v) { }

template <class T> struct from_class {
	static inline void serialize(Serializer& s, const T& v) {
		s.beginObject(type_of<T>(), &v);
		__serialize_body(s, &v);
		s.endObject(type_of<T>(), &v);
	}
	static inline void unserialize(Unserializer& s, T& v) {
		s.beginObject(type_of<T>(), &v);
		__unserialize_body(s, &v);
		do_complete(v);
		s.endObject(type_of<T>(), &v);
	}
};


// Type information
template <class T> struct from_type {
	static inline void serialize(Serializer& s, const T& v) { s.onValue(v); }
	static inline void unserialize(Unserializer& s, T& v) { s.onValue(v); }
};


// Enum information
template <class T> struct from_enum {
	static inline void serialize(Serializer& s, const T& v);
	static inline void unserialize(Unserializer& s, T& v);
};

// serialization
template <class T>
inline void __serialize(Serializer& s, T *v) {
	s.onPointer(type_of<T>(), v);
}
template <class T>
inline void __serialize(Serializer& s, const T *v) {
	s.onPointer(type_of<T>(), v);
}
template <class T>
inline void __serialize(Serializer& s, const T& v) {
	_if<type_info<T>::is_class, from_class<T>,
			_if<type_info<T>::is_enum, from_enum<T>, from_type<T> >
		>::serialize(s, v);
}
template <class T>
inline Serializer& operator&(Serializer& serializer, const T& data) {
	__serialize(serializer, data);
	return serializer;
}
template <class T>
inline Serializer& operator<<(Serializer& serializer, const T& data) {
	__serialize(serializer, data);
	return serializer;
}


// unserialization
template <class T>
inline void __unserialize(Unserializer& s, T *&v) {
	s.onPointer(type_of<T>(), (void **)&v);
}
template <class T>
inline void __unserialize(Unserializer& s, const T *&v) {
	s.onPointer(type_of<T>(), (void **)&v);
}
template <class T>
inline void __unserialize(Unserializer& s, T& v) {
	_if<type_info<T>::is_class, from_class<T>,
			_if<type_info<T>::is_enum, from_enum<T>, from_type<T> >
		>::unserialize(s, v);
}
template <class T>
inline Unserializer& operator&(Unserializer& serializer, T& data) {
	__unserialize(serializer, data);
	return serializer;
}
template <class T>
inline Unserializer& operator&(Unserializer& s, const Field<T>& field) {
	__unserialize(s, field);
	return s;
}
template <class T>
inline Unserializer& operator&(Unserializer& s, const DefaultField<T>& field) {
	__unserialize(s, field);
	return s;
}
template <class T>
inline Unserializer& operator>>(Unserializer& serializer, T& data) {
	__unserialize(serializer, data);
	return serializer;
}
template <class T>
inline Unserializer& operator>>(Unserializer &s, const Field<T>& field) {
	__unserialize(s, field);
	return s;
}
template <class T>
inline Unserializer& operator>>(Unserializer &s, const DefaultField<T>& field) {
	__unserialize(s, field);
	return s;
}


// ExternalSolver class
class ExternalSolver {
public:
	static ExternalSolver null;
	virtual ~ExternalSolver(void);
	void *solve(string ref);
	string ref(void *object);
};

template <class T>
inline void from_enum<T>::serialize(Serializer& s, const T& v) {
	s.onEnum(&v, v, type_of<T>());
}

template <class T>
inline void from_enum<T>::unserialize(Unserializer& s, T& v) {
	v = (T)s.onEnum(type_of<T>());
}


// Field inlines
template <class T>
inline void __serialize(Serializer& s, const Field<const T>& field) {
	s.beginField(field.name());
	__serialize(s, field.value());
	s.endField();
}
template <class T>
inline void __serialize(Serializer& s, const DefaultField<const T>& field) {
	__serialize(s, (const Field<const T>&)field);
}

template <class T>
inline void __serialize(Serializer& s, const Field<T>& field) {
	s.beginField(field.name());
	__serialize(s, field.value());
	s.endField();
}
template <class T>
inline void __serialize(Serializer& s, const DefaultField<T>& field) {
	__serialize(s, (const Field<T>&)field);
}

template <class T>
inline void __unserialize(Unserializer& s, const Field<T>& field) {
	if(s.beginField(field.name())) {
		__unserialize(s, field.value());
		s.endField();
	}
}

template <class T>
inline void __unserialize(Unserializer& s, const DefaultField<T>& field) {
	if(s.beginField(field.name())) {
		__unserialize(s, field.value());
		s.endField();
	}
	else
		field.value() = field.defaultValue();
}


// for ascending compatibility
template <class T>
inline Unserializer& operator&(Unserializer& s, const Base<T>& base)
	{ __unserialize_body(s, base.ptr); return s; }

template <class T>
inline Unserializer& operator>>(Unserializer& s, const Base<T>& base)
	{ __unserialize_body(s, base.ptr); return s; }

template <class T>
inline Serializer& operator&(Serializer& s, const Base<T>& base)
	{ __serialize_body(s, base.ptr); return s; }

template <class T>
inline Serializer& operator<<(Serializer& s, const Base<T>& base)
	{ __serialize_body(s, base.ptr); return s; }

} } // elm::serial2
	
#endif // ELM_SERIAL2_TYPE_H
