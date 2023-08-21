/*
 *	Option class interface
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
#ifndef ELM_UTIL_OPTION_H
#define ELM_UTIL_OPTION_H

#include <elm/assert.h>
#include <elm/type_info.h>

namespace elm {

// OptionalNone value
class OptionalNone { };
extern const OptionalNone none;


// Optional value
template <class T> class Option {
public:
	inline Option(): one(false) { }
	inline Option(const OptionalNone& none): one(false) { }
	inline Option(t::in<T> value): one(true) { t::put<T>(val, value); }
	inline Option(const Option<T> &opt): one(opt.one) { t::put<T>(val, t::get<T>(opt.val)); }

	inline bool some() const { return one; }
	inline bool none() const { return !one; }

	inline t::ret<T> value() const
		{ ASSERTP(one, "no value in option"); return t::get<T>(val); }
	inline t::ret<T> operator*() const { return value(); }

	template <class F> const Option<T>& if_one(const F& f) const
		{ if(one) f(t::get<T>(val)); return *this; }
	template <class F> const Option<T>& if_else(const F& f) const
		{ if(!one) f(); return *this; }

	inline Option<T>& operator=(const Option<T>& opt)
		{ one = opt.one; if(opt.one) t::put<T>(val, opt.val); return *this; }
	inline Option<T>& operator=(t::in<T> value)
		{ one = true; type_info<T>::put(val, value); return *this; }

	inline bool equals(const OptionalNone& _) const { return none(); }
	inline bool equals(const Option<T> &opt) const
		{ return (this->none() && opt.none()) || (this->some() && opt.some() && val == opt.val); }
	inline bool operator==(const OptionalNone& _) const { return none(); }
	inline bool operator!=(const OptionalNone& _) const { return !none(); }
	inline bool operator==(const Option<T>& o) const { return equals(o); }
	inline bool operator!=(const Option<T>& o) const { return !equals(o); }

	// deprecated
	inline operator bool(void) const { return some(); }
	inline operator t::ret<T>() const { return value(); }
	inline bool isOne() const { return one; }
	inline bool isNone() const { return !one; }

private:
	bool one;
	typename t::var<T> val;
};


// Fast Option building
template <class T>
inline Option<T> some(const T& val) { return Option<T>(val); };

} // elm

#endif /* ELM_UTIL_OPTION_H */
