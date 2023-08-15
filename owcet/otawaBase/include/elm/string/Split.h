/*
 *	Split class
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-08, IRIT UPS.
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
#ifndef ELM_STRING_SPLIT_H_
#define ELM_STRING_SPLIT_H_

#include <elm/string/String.h>

namespace elm {

class StringSplit {
public:
	inline StringSplit(void): l(-1), p(-1) { }
	inline StringSplit(const String& str, char chr): s(str), ss(String::make(chr)), l(-1), p(-1) { find(); }
	inline StringSplit(const String& str, String sub): s(str), ss(sub), l(-1), p(-1) { find(); }

	inline bool ended(void) const { return l >= s.length(); }
	inline String item(void) const { return s.substring(l + 1, p - l - 1); }
	inline void next(void) { if(p >= s.length()) l = s.length(); else find(); }
	inline bool equals(const StringSplit& sp) const { return l == sp.l && p == sp.p; }

	inline operator bool() const { return !ended(); }
	inline operator String() const { return item(); }
	inline String operator*() const { return item(); }
	inline StringSplit& operator++() { next(); return *this; }
	inline StringSplit operator++(int) { StringSplit o = *this; next(); return o; }
	inline bool operator==(const StringSplit& sp) const { return equals(sp); }
	inline bool operator!=(const StringSplit& sp) const { return !equals(sp); }

private:
	inline void find(void)
		{ l = p; p = s.indexOf(ss, l + 1); if(p < 0) p = s.length(); }
	String s;
	String ss;
	int l, p;
};

}	// elm

#endif /* ELM_STRING_SPLIT_H_ */
