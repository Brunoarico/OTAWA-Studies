/*
 *	RTTI module
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
#ifndef ELM_RTTI_H
#define ELM_RTTI_H

#include <elm/dyndata/AbstractCollection.h>
#include <elm/rtti/Class.h>
#include <elm/rtti/Enum.h>
#include <elm/rtti/Tuple.h>

namespace elm {

// Field class
template <class V>
class Field {
	CString _name;
	V& _value;
public:
	inline Field(CString name, V& value): _name(name), _value(value) { }
	inline CString name(void) const { return _name; };
	inline V& value(void) const { return _value; };
};
template <class T>
inline Field<T> field(CString name, T& value) {
	return Field<T>(name, value);
}


// DefaultField class
template <class T>
class DefaultField: public Field<T> {
	const T& def;
public:
	inline DefaultField(CString name, T& value, const T& _default)
		: Field<T>(name, value), def(_default) { }
	inline const T& defaultValue(void) const { return def; }
};
template <class T>
inline DefaultField<T> field(CString name, T& value, const T& def)
	{ return DefaultField<T>(name, value, def); }
template <class T>
inline DefaultField<T> field(CString name, const T& value, const T& def)
	{ return DefaultField<T>(name, const_cast<T&>(value), def); }


// _unqualify function
inline CString _unqualify(CString name) {
	int pos = name.lastIndexOf(':');
	if(pos < 0)
		return name;
	else
		return name.substring(pos + 1);
}

} // elm

#endif // ELM_RTTI_H
