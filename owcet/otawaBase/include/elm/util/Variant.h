/*
 *	Variant class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2005-17, IRIT UPS.
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
#ifndef ELM_UTIL_VARIANT_H_
#define ELM_UTIL_VARIANT_H_

#include <elm/types.h>

namespace elm {

namespace variant {

	typedef union {
		bool b;
		t::int8 i8;
		t::int16 i16;
		t::int32 i32;
		t::int64 i64;
		t::uint8 u8;
		t::uint16 u16;
		t::uint32 u32;
		t::uint64 u64;
		float f;
		double d;
		const char *s;
		void *p;
		const void *cp;
	} data_t;

	template <typename T>
	struct access_t {
		typedef const T& rt;
		static const T& get(const data_t& d) 	{ return *static_cast<const T *>(d.cp); }
		static void set(data_t& d, const T& x)	{ d.cp = &x; }
	};

	template <typename T>
	struct access_t<T &> {
		typedef T& rt;
		static T& get(const data_t& d) 	{ return *static_cast<T *>(d.p); }
		static void set(data_t& d, T& x)	{ d.p = &x; }
	};

	template <> struct access_t<bool> 		{ typedef bool      rt; static bool 	 get(const data_t& d) { return d.b; } static void set(data_t& d, bool x)	{ d.b = x; } };
	template <> struct access_t<t::int8 >	{ typedef t::int8   rt; static t::int8   get(const data_t& d) { return d.i8 ; } static void set(data_t& d, t::int8   x) { d.i8  = x; } };
	template <> struct access_t<t::int16>  	{ typedef t::int16  rt; static t::int16  get(const data_t& d) { return d.i16; } static void set(data_t& d, t::int16  x) { d.i16 = x; } };
	template <> struct access_t<t::int32>  	{ typedef t::int32  rt; static t::int32  get(const data_t& d) { return d.i32; } static void set(data_t& d, t::int32  x) { d.i32 = x; } };
	template <> struct access_t<t::int64>  	{ typedef t::int64  rt; static t::int64  get(const data_t& d) { return d.i64; } static void set(data_t& d, t::int64  x) { d.i64 = x; } };
	template <> struct access_t<t::uint8 > 	{ typedef t::uint8  rt; static t::uint8  get(const data_t& d) { return d.u8 ; } static void set(data_t& d, t::uint8  x) { d.u8  = x; } };
	template <> struct access_t<t::uint16> 	{ typedef t::uint16 rt; static t::uint16 get(const data_t& d) { return d.u16; } static void set(data_t& d, t::uint16 x) { d.u16 = x; } };
	template <> struct access_t<t::uint32> 	{ typedef t::uint32 rt; static t::uint32 get(const data_t& d) { return d.u32; } static void set(data_t& d, t::uint32 x) { d.u32 = x; } };
	template <> struct access_t<t::uint64> 	{ typedef t::uint64 rt; static t::uint64 get(const data_t& d) { return d.u64; } static void set(data_t& d, t::uint64 x) { d.u64 = x; } };
	template <> struct access_t<float>	   	{ typedef float     rt; static float     get(const data_t& d) { return d.f;   } static void set(data_t& d, float     x) { d.f   = x; } };
	template <> struct access_t<double>	   	{ typedef double    rt; static double    get(const data_t& d) { return d.d;   } static void set(data_t& d, double    x) { d.d   = x; } };

	template <> struct access_t<cstring> 		{ typedef cstring rt; static cstring get(const data_t& d) { return static_cast<const char *>(d.cp); } static void set(data_t& d, cstring x) { d.cp = x.chars(); } };
	template <> struct access_t<string> 		{ typedef string  rt; static string get(const data_t& d)  { return static_cast<const char *>(d.cp); } static void set(data_t& d, string x)	 { d.cp = x.toCString().chars(); } };
	template <> struct access_t<const cstring&> { typedef cstring rt; static cstring get(const data_t& d) { return static_cast<const char *>(d.cp); } static void set(data_t& d, cstring x) { d.cp = x.chars(); } };
	template <> struct access_t<const string&>	{ typedef string  rt; static string get(const data_t& d)  { return static_cast<const char *>(d.cp); } static void set(data_t& d, string x)	 { d.cp = x.toCString().chars(); } };

	template <class T> struct access_t<T *> {
		typedef T *rt;
		static T *get(const data_t& d) { return static_cast<T *>(d.p); }
		static void set(data_t& d, T *x)	{ d.p = x; }
	};
	template <class T> struct access_t<const T *> {
		typedef const T *rt;
		static const T *get(const data_t& d) { return static_cast<const T *>(d.cp); }
		static void set(data_t& d, const T *x)	{ d.cp = x; }
	};

}	// variant


class Variant {
public:
	inline Variant() { }
	template <class T> Variant(const T& x) { variant::access_t<T>::set(_data, x); }
	inline void *asPointer(void) const { return _data.p; }
	template <class T> typename variant::access_t<T>::rt as() const { return variant::access_t<T>::get(_data); }
private:
	variant::data_t _data;
};

}	// elm

#endif /* ELM_UTIL_VARIANT_H_ */
