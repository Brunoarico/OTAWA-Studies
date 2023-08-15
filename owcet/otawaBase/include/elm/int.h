/*
 *	$Id$
 *	int module interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007-10, IRIT UPS.
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
#ifndef ELM_INT_H_
#define ELM_INT_H_

#include <elm/arch.h>
#include <stdint.h>

namespace elm {

// bits operations
#ifdef __GNUC__
inline int countOnes(t::uint8 i) { return __builtin_popcount(i); }
inline int countOnes(t::uint16 i) { return __builtin_popcount(i); }
inline int countOnes(t::uint32 i) { return __builtin_popcountl(i); }
inline int countOnes(t::uint64 i) { return __builtin_popcountll(i); }
#else
int ones(t::uint8 i);
inline int countOnes(t::uint16 i) { return ones(t::uint8(i)) + ones(t::uint8(i >> 8)); }
inline int countOnes(t::uint32 i) { return ones(t::uint16(i)) + ones(t::uint16(i >> 16)); }
inline int countOnes(t::uint64 i) { return ones(t::uint32(i)) + ones(t::uint32(i >> 32)); }
#endif

int msb(t::uint32 i);
inline int msb(t::int32 i) { return msb(t::uint32(i)); }
int msb(t::uint64 i);
inline int msb(t::int64 i) { return msb(t::uint64(i)); }
t::uint32 leastUpperPowerOf2(t::uint32 v);
t::uint64 leastUpperPowerOf2(t::uint64 v);


// arithmetic operations
inline t::uint32 abs(t::int32 v) { return  v >= 0 ? v : (-v); }
inline t::uint64 abs(t::int64 v) { return  v >= 0 ? v : (-v); }
inline t::uint32 roundup(t::uint32 v, t::uint32 m)
	{ if(countOnes(m) == 1) return (v + m - 1) & ~(m - 1); else return ((v - 1) / m + 1) * m; }
inline t::uint32 rounddown(t::uint32 v, t::uint32 m)
	{ if(countOnes(m) == 1) return v & ~(m - 1); else return v / m * m; }


// operations with overflow
t::uint32 mult(t::uint32 a, t::uint32, bool& over);
t::uint64 mult(t::uint64 a, t::uint64, bool& over);

// deprecated
inline int ones(t::uint8 i) { return countOnes(i); }
inline int ones(t::uint16 i) { return countOnes(i); }
inline int ones(t::uint32 i) { return countOnes(i); }
inline int ones(t::uint64 i) { return countOnes(i); }

} // elms

#endif /* ELM_INT_H_ */
