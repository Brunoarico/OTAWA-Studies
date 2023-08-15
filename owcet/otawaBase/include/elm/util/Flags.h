/*
 *	Flags class
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2019, IRIT UPS.
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
#ifndef ELM_UTIL_FLAGS_H_
#define ELM_UTIL_FLAGS_H_

#include <elm/types.h>

namespace elm {

template <class T = t::uint32>
class Flags {
public:
	inline Flags(T i = 0): m(i) { }
	inline bool bit(int i) const { return (m & (1 << i)) != 0; }
	inline void set(int i) { m |= 1 << i; }
	inline void clear(int i) { m &= ~(1 << i); }
	inline bool operator()(int i) const { return bit(i); }
	inline bool operator[](int i) const { return bit(i); }
private:
	T m;
};

}	// elm

#endif /* ELM_UTIL_FLAGS_H_ */
