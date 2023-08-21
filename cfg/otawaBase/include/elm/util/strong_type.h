/*
 *	StrongType class
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-19, IRIT UPS.
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
#ifndef ELM_UTIL_STRONG_TYPE_H
#define ELM_UTIL_STRONG_TYPE_H

// SolidType class
template <class T>
class SolidType {
public:
	inline explicit SolidType(T v = 0): _v(v) { }
	inline T operator*(void) const { return _v; }
	inline T& operator*(void) { return _v; }
private:
	T _v;
};


// OTAWA_STRONG_TYPE macro
#define OTAWA_STRONG_TYPE(N, T) \
	typedef struct N { \
		typedef T _t; \
		_t v; \
		inline N(_t _v = 0): v(_v) { }; \
		inline operator const _t&(void) const { return v; } \
		inline operator _t&(void) { return v; } \
		inline struct N& operator=(const _t& _v) { v = _v; return *this; } \
		inline const _t& operator*(void) const { return v; } \
		inline _t& operator*(void) { return v; } \
	} N

// Shortcut
#ifndef OTAWA_NO_SHORTCUT
#	define STRONG_TYPE(N, T) OTAWA_STRONG_TYPE(N, T)
#endif

#endif // ELM_UTIL_STRONG_TYPE_H

