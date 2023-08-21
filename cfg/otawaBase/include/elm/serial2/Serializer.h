/*
 *	Serializer class interface
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
#ifndef ELM_SERIAL2_SERIALIZER_H
#define ELM_SERIAL2_SERIALIZER_H

namespace elm {

namespace rtti {
	class Type;
};

namespace serial2 {

/*class AbstractType;
class AbstractEnum;*/

// Serializer class
class Serializer {
public:
	virtual ~Serializer(void) { }
	virtual void flush(void) = 0;

	// Complex type serializers
	virtual void beginObject(const rtti::Type& clazz, const void *object) = 0;
	virtual void endObject(const rtti::Type& clazz, const void *object) = 0;
	virtual void beginField(CString name) = 0;
	virtual void endField(void) = 0;
	virtual void onPointer(const rtti::Type& clazz, const void *object) = 0;
	virtual void beginCompound(const void *object) = 0;
	virtual void onItem(void) = 0;
	virtual void endCompound(const void*) = 0;
	virtual void onEnum(const void *address, int value, const rtti::Type& clazz) = 0;

	// Base value serializers
	virtual void onValue(const bool& v) = 0;
	virtual void onValue(const signed int& v) = 0;
	virtual void onValue(const unsigned int& v) = 0;
	virtual void onValue(const signed char& v) = 0;
	virtual void onValue(const unsigned char& v) = 0;
	virtual void onValue(const signed short& v) = 0;
	virtual void onValue(const unsigned short& v) = 0;
	virtual void onValue(const signed long& v) = 0;
	virtual void onValue(const unsigned long& v) = 0;
	virtual void onValue(const signed long long& v) = 0;
	virtual void onValue(const unsigned long long& v) = 0;
	virtual void onValue(const float& v) = 0;
	virtual void onValue(const double& v) = 0;
	virtual void onValue(const long double& v) = 0;
	virtual void onValue(const CString& v) = 0;
	virtual void onValue(const String& v) = 0;
};

} } // elm::serial2

#endif	// ELM_SERIAL2_SERIALIZER_H
