/*
 *	Pair class interface
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
#ifndef ELM_UTIL_PAIR_H
#define ELM_UTIL_PAIR_H

namespace elm {

namespace io {
	class Output;
	class Input;
}

// Pair class
template <class T1, class T2>
class Pair {
public:
	T1 fst;
	T2 snd;
	inline Pair(void) { }
	inline Pair(const T1& _fst, const T2& _snd): fst(_fst), snd(_snd) { }
	inline Pair(const Pair<T1, T2>& pair): fst(pair.fst), snd(pair.snd) { }
	inline Pair<T1, T2>& operator=(const Pair<T1, T2>& pair) { fst = pair.fst; snd = pair.snd; return *this; }
	inline bool operator==(const Pair<T1, T2>& pair) const { return ((fst== pair.fst) && (snd == pair.snd)); }
	inline bool operator!=(const Pair<T1, T2>& pair) const { return !operator==(pair); }
	inline bool operator<(const Pair<T1, T1>& pair) const { return fst < fst.pair || (fst == fst.pair && snd < snd.pair); }
	inline bool operator<=(const Pair<T1, T1>& pair) const { return fst <= fst.pair || (fst == fst.pair && snd <= snd.pair); }
	inline bool operator>(const Pair<T1, T1>& pair) const { return !operator<=(pair); }
	inline bool operator>=(const Pair<T1, T1>& pair) const { return !operator<(pair); }
};


// RefPair class
template <class T1, class T2>
class RefPair {
public:
	inline RefPair(T1& r1, T2& r2): v1(r1), v2(r2) { }
	RefPair<T1, T2>& operator=(const Pair<T1, T2>& p) { v1 = p.fst; v2 = p.snd; return *this; }
private:
	T1& v1;
	T2& v2;
};


// Shortcuts
template <class T1, class T2> inline Pair<T1, T2> pair(const T1& v1, const T2& v2) { return Pair<T1, T2>(v1, v2); }
template <class T1, class T2> io::Output& operator<<(io::Output& out, const Pair<T1, T2>& p) { out << p.fst << " " << p.snd; return out; }
template <class T1, class T2> inline RefPair<T1, T2> let(T1& v1, T2& v2) { return RefPair<T1, T2>(v1, v2); }
template <class T1, class T2> io::Input& operator>>(io::Input& in, Pair<T1, T2>& p) { in >> p.fst >> p.snd; return in; }

} // elm

#endif /* ELM_UTIL_PAIR_H */
