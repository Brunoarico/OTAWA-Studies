/*
 *	Input class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-09, IRIT UPS.
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
#ifndef ELM_IO_INPUT_H
#define ELM_IO_INPUT_H

#include <elm/enum_info.h>
#include <elm/meta.h>
#include <elm/types.h>
#include <elm/string/String.h>
#include <elm/string/CString.h>
#include <elm/io/InStream.h>

namespace elm { namespace io {

// Input class
class Input {
public:
	Input(void);
	Input(InStream& stream);
	inline InStream& stream(void) const { return *strm; };
	inline void setStream(InStream& stream) { strm = &stream; buf = -1; };
	inline bool ended() const { return state & ENDED; }
	inline bool failed() const { return state & FAILED; }
	inline bool error() const { return state & IO_ERROR; }
	inline bool ok() const { return state == 0; }
	inline void resetState() { state &= ~(FAILED | IO_ERROR); }

	bool scanBool(void);
	char scanChar(void);
	t::uint32 scanULong(int base = 0);
	t::int32 scanLong(int base = 0);
	t::uint64 scanULLong(int base = 0);
	t::int64 scanLLong(int base = 0);
	double scanDouble(void);
	String scanWord(void);
	String scanLine(void);
	void swallow(char chr);
	void swallow(CString str);
	void swallow(const String& str);
	void swallowBlank(void);

	inline Input& operator>>(bool& value) { value = scanBool(); return *this; };
	inline Input& operator>>(char& value) { value = scanChar(); return *this; };
	inline Input& operator>>(unsigned char& value) { value = scanULong(); return *this; };
	inline Input& operator>>(short& value) { value = scanLong(); return *this; };
	inline Input& operator>>(unsigned short& value) { value = scanULong(); return *this; };
	inline Input& operator>>(int& value) { value = scanLong(); return *this; };
	inline Input& operator>>(unsigned int& value) { value = scanULong(); return *this; };
#	ifndef __LP64__
		inline Input& operator>>(signed long& value) { value = scanLong(); return *this; };
		inline Input& operator>>(unsigned long& value) { value = scanULong(); return *this; };
#	else
		inline Input& operator>>(signed long& value) { value = scanLLong(); return *this; };
		inline Input& operator>>(unsigned long& value) { value = scanULLong(); return *this; };
#	endif
	inline Input& operator>>(signed long long& value) { value = scanLLong(); return *this; };
	inline Input& operator>>(unsigned long long& value) { value= scanULLong(); return *this; };
	inline Input& operator>>(float& value) { value = scanDouble(); return *this; };
	inline Input& operator>>(double& value) { value = scanDouble(); return *this; };
	inline Input& operator>>(String& value) { value = scanLine(); return *this; };

	inline Input& operator>>(const string& text) { swallow(text); return *this; }
	inline Input& operator>>(cstring text) { swallow(text); return *this; }
	inline Input& operator>>(const char *text) { swallow(cstring(text)); return *this; }

	template <class T> struct def_scanner { static inline void scan(Input& in, T& v) { unsupported(); } };
	template <class T> struct enum_scanner { static inline void scan(Input& in, T& v) { string s; in >> s; v = enum_info<T>::fromString(s); } };
	template <class T> Input& operator>>(T& v)
		{ _if<type_info<T>::is_defined_enum, enum_scanner<T>, def_scanner<T> >::scan(*this, v); return *this; }

	class LineIter: public PreIterator<LineIter, string> {
	public:
		inline LineIter(Input& input, bool end = false): in(input), e(end) { if(!e) next(); }
		inline bool ended() const { return e; }
		inline string item() const { return l; }
		inline void next() { if(in.ended()) e = true; else l = in.scanLine(); }
		inline bool equals(const LineIter& i) const { return &in == &i.in && e == i.e; }
	private:
		Input& in;
		bool e;
		string l;
	};

	class LineRange {
	public:
		inline LineRange(Input& in): i(in) { }
		inline LineIter begin() { return LineIter(i); }
		inline LineIter end() { return LineIter(i, true); }
	private:
		Input& i;
	};
	inline LineRange lines() { return LineRange(*this); }


private:
	 [[noreturn]] static void unsupported(void);
	InStream *strm;
	t::int16 buf;
	t::uint16 state;
	int get();
	int skip();
	void back(int chr);
	static const t::uint16
		ENDED = 0x01,
		FAILED = 0x02,
		IO_ERROR = 0x04;
};

} } // elm::io

#endif	// ELM_IO_INPUT_H
