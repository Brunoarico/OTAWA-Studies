/*
 *	type_info class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2008-13, IRIT UPS.
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
#ifndef ELM_TYPE_INFO_H_
#define ELM_TYPE_INFO_H_

#include <elm/arch.h>
#include <elm/string/String.h>
#include <elm/int.h>
#include <elm/meta.h>

namespace elm {

namespace intern {
	template <class T> char test_class(int T::*);
	template <class T> int test_class(...);

	template <class T>
	struct is_scalar {
		enum { _ = sizeof(test_class<T>(0)) != sizeof(char) };
	};
}

// type_info trait
typedef struct default_t {
	enum { is_type = 0 };
	enum { is_scalar = 0 };
	enum { is_enum = 0, is_defined_enum = 0 };
	enum { is_class = 0 };
	enum { is_ptr = 0 };
	enum { is_ref = 0 };
	enum { is_deep = 0 };
	enum { is_virtual = 0 };
	enum { is_void = 0 };
} default_t;


// generic class
template <class T> class type_info: public default_t {
public:
	enum { is_class = 1 };
	enum { is_deep = 1 };
	enum { is_virtual = 1 };
	enum { is_enum = intern::is_scalar<T>::_ };
	enum { is_scalar = intern::is_scalar<T>::_ };
	static cstring name(void) { return ""; }

	typedef T var_t;
	typedef var_t embed_t;
	static inline T& ref(T& v) { return v; }
	static inline const T& get(const T& v) { return v; }
	static inline void put(T& x, const T& v) { x = v; }

	typedef const T& in_t;
	typedef T& out_t;
	typedef const T& ret_t;
	typedef T& mut_t;
};


// generic type
typedef struct type_t: public default_t {
	enum { is_type = 1 };
	enum { is_deep = 1 };
} type_t;

// scalar type
template <class T>
struct scalar_t: public default_t {
	enum { is_enum = 0 };
	enum { is_scalar = 1 };
	enum { is_deep = 0 };

	typedef T var_t;
	typedef var_t embed_t;
	static inline T& ref(T& v) { return v; }
	static inline T get(const T& v) { return v; }
	static inline void put(T& x, T v) { x = v; }

	typedef T in_t;
	typedef T& out_t;
	typedef T ret_t;
	typedef T& mut_t;
};


// enum type
template <class T>
struct enum_t: public scalar_t<T> {
	enum { is_enum = 1 };
};


// bool specialization
template <> struct type_info<bool>: public scalar_t<bool> {
	static const bool min = false;
	static const bool max = true;
	static const bool null = false;
	static inline CString name(void) { return "bool"; }
};


// void specialization
template <> struct type_info<void>: public default_t {
	enum { is_void = 1 };
};


// integer specialization
template <class I>
struct signed_info: public scalar_t<I> {
	static const int size = sizeof(I) * 8;
	static const bool is_signed = true;
	static const I min = I(-1) << (size - 1);
	static const I max = ~min;
	static const I null = 0;
	static const int shift = 0;
};
template <class I> const I signed_info<I>::null;
template <class I> const I signed_info<I>::min;
template <class I> const I signed_info<I>::max;

template <class I>
struct unsigned_info: public scalar_t<I> {
	static const int size = sizeof(I) * 8;
	static const bool is_signed = false;
	static const I min = 0;
	static const I max = I(-1);
	static const I null = 0;
	static const int shift = 0;
};
template <class I> const I unsigned_info<I>::null;
template <class I> const I unsigned_info<I>::min;
template <class I> const I unsigned_info<I>::max;

template <> struct type_info<char>: public signed_info<char> { static cstring name(void); static const int shift = 0; };
template <> struct type_info<t::int8>: public signed_info<t::int8> { static cstring name(void); static const int shift = 0; };
template <> struct type_info<t::uint8>: public unsigned_info<t::uint8>  { static cstring name(void); static const int shift = 0; };
template <> struct type_info<t::int16>: public signed_info<t::int16> { static cstring name(void); static const int shift = 1; };
template <> struct type_info<t::uint16>: public unsigned_info<t::uint16> { static cstring name(void); static const int shift = 1; };
template <> struct type_info<t::int32>: public signed_info<t::int32> { static cstring name(void); static const int shift = 2; };
template <> struct type_info<t::uint32>: public unsigned_info<t::uint32> { static cstring name(void); static const int shift = 2; };
template <> struct type_info<t::int64>: public signed_info<t::int64> { static cstring name(void); static const int shift = 3; };
template <> struct type_info<t::uint64>: public unsigned_info<t::uint64> { static cstring name(void); static const int shift = 3; };

	
// float specialization
template <> struct type_info<float>: public scalar_t<float> {
	static const float min;
	static const float max;
	static const float null;
	static cstring name(void);
};
template <> struct type_info<double>: public scalar_t<double> {
	static const double min;
	static const double max;
	static const double null;
	static cstring name(void);
};
template <> struct type_info<long double>: public scalar_t<long double> {
	static const long double min;
	static const long double max;
	static const long double null;
	static cstring name(void);
};


// String specialization
template <> struct type_info<cstring>: public default_t {
	static const cstring null;
	static cstring name(void);

	typedef cstring var_t;
	typedef var_t embed_t;
	static inline cstring& ref(cstring& v) { return v; }
	static inline cstring get(const cstring& v) { return v; }
	static inline void put(cstring& x, cstring v) { x = v; }

	typedef cstring in_t;
	typedef cstring& out_t;
	typedef cstring ret_t;
	typedef cstring& mut_t;
};

template <> struct type_info<string>: public default_t {
	static const string null;
	static cstring name(void);
	enum { is_virtual = 1 };
	enum { is_deep = 1 };

	typedef string var_t;
	typedef var_t embed_t;
	typedef const string& in_t;
	typedef string& out_t;
	typedef const string& ret_t;
	typedef string& mut_t;

	static inline mut_t ref(var_t& v) { return v; }
	static inline ret_t get(const var_t& v) { return v; }
	static inline void put(var_t& x, in_t v) { x = v; }
};


// pointer specialization
template <class T> struct type_info<const T *>: public scalar_t<const T *> {
	typedef T of;
	enum { is_const = 1 };
	enum { is_ptr = 1 };
	static const T * const null;
	static string name(void) { return "const " + type_info<T>::name() + " *"; }
};
template <class T> const T *const type_info<const T *>::null = nullptr;


template <class T> struct type_info<T *>: public scalar_t<T *> {
	typedef T of;
	enum { is_const = 0 };
	enum { is_ptr = 1 };
	static T * const null;
	static string name(void) { return type_info<T>::name() + " *"; }
};
template <class T> T *const type_info<T *>::null = nullptr;


// reference specialization
template <class T>
class ref_t: public default_t {
public:
	struct delegate {
	public:
		inline delegate(T *&ptr): p(ptr) { }
		inline operator T&() const { return *p; }
		inline delegate& operator=(T &r) { p = &r; return *this; }
	private:
		T *& p;
	};

	typedef T *var_t;
	typedef var_t embed_t;
	static inline delegate ref(T *&v) { return delegate(v); }
	static inline T& get(T *v) { return *v; }
	static inline void put(T *& x, T& v) { x = &v; }

	typedef T& in_t;
	typedef T& out_t;
	typedef T& ret_t;
	typedef delegate mut_t;
};

template <class T> struct type_info<const T&>: public ref_t<const T> {
	typedef T of;
	enum { is_const = 1 };
	static string name(void) { return "const " + type_info<T>::name() + "& "; }
};

template <class T> struct type_info<T&>: public ref_t<T> {
	typedef T of;
	enum { is_const = 0 };
	static string name(void) { return type_info<T>::name() + "& "; }
};

template <class T> struct ti: type_info<T> { };

namespace t {
	template <class T> using var = typename type_info<T>::var_t;
	template <class T> using in = typename type_info<T>::in_t;
	template <class T> using out = typename type_info<T>::out_t;
	template <class T> using ret = typename type_info<T>::ret_t;
	template <class T> using mut = typename type_info<T>::mut_t;
	template <class T> inline void put(var<T>& x, in<T> v) { type_info<T>::put(x, v); }
	template <class T> inline ret<T> get(const var<T>& v) { return type_info<T>::get(v); }
	template <class T> inline mut<T> ref(var<T>& x) { return type_info<T>::ref(x); }
} // t

} // elm

#endif /* ELM_TYPE_INFO_H_ */
