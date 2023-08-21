/*
 *	Enum class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007, IRIT UPS.
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
#ifndef ELM_RTTI_ENUM_H_
#define ELM_RTTI_ENUM_H_

#include <elm/data/Vector.h>
#include "Type.h"

namespace elm { namespace rtti {

// value_t structure

class Enum: public Type, public Enumerable {
public:

	class Value {
	public:
		static inline Value end(void) { return Value("", 0); }
		inline Value(void): _value(0) { }
		inline Value(cstring name, int value): _name(name), _value(value) { }
		inline cstring name(void) const { return _name; }
		inline int value(void) const { return _value; }
	private:
		cstring _name;
		int _value;
	};

	class make {
		friend class Enum;
	public:
		inline make(cstring name): _name(name) { }
		inline make& value(cstring name, int value) { _values.add(Value(name, value)); return *this; }
		inline make& alias(cstring name, int value) { _aliases.add(Value(name, value)); return *this; }
	private:
		cstring _name;
		Vector<Value> _values;
		Vector<Value> _aliases;
	};

	Enum(const make& make);
	Enum(cstring name, const Value values[]);

	typedef Vector<Value>::Iter Iter;
	inline Iter values(void) const { return Iter(_values); }

	// Enumerable interface
	virtual const Type& type(void) const;
	virtual int valueFor(string text) const;
	virtual cstring nameFor(int value) const;

	// Type interface
	virtual bool canCast(const Type *t) const;
	virtual bool isEnum(void) const;
	virtual const Enumerable& asEnum(void) const;

private:
	Vector<Value> _values;
	Vector<Value> _map;
};

inline rtti::Enum::Value value(cstring name, int value)
	{ return rtti::Enum::Value(name, value); }

// New support for enumeration
#define ELM_DECLARE_ENUM(name) \
	namespace elm { namespace rtti { template <> const elm::rtti::Type& _type<name>::_(void); } } \
	inline elm::io::Output& operator<<(elm::io::Output& out, name value) { out << elm::type_of<name>().asEnum().nameFor(value); return out; } \
	inline elm::io::Input& operator>>(elm::io::Input& in, name& value) \
		{ value = static_cast<name>(elm::type_of<name>().asEnum().valueFor(in.scanWord())); return in; }

#define ELM_DEFINE_ENUM(type, desc) \
	namespace elm { namespace rtti { template <> const elm::rtti::Type& _type<type>::_(void) { return desc; } } }

#define ELM_BEGIN_ENUM(type) \
	namespace elm { namespace rtti { template <> const elm::rtti::Type& _type<type>::_(void) { \
		static elm::rtti::Enum _(Enum::make(#type)

#define ELM_END_ENUM \
		); \
		return _; \
	} } }


#ifndef ELM_NO_SHORTCUT
#	define DECLARE_ENUM(name) 		ELM_DECLARE_ENUM(name)
#	define DEFINE_ENUM(type, desc)	ELM_DEFINE_ENUM	(type, desc)
#	define BEGIN_ENUM(type)			ELM_BEGIN_ENUM(type)
#	define END_ENUM					ELM_END_ENUM
#endif

} }		// elm::rtti

#endif /* INCLUDE_ELM_RTTI_ENUM_H_ */
