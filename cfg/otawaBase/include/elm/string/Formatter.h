/*
 *	Formatter class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2007-09, IRIT UPS.
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
#ifndef ELM_STRING_FORMATTER_H
#define ELM_STRING_FORMATTER_H

#include <elm/io.h>

namespace elm {

// Formatter class
class Formatter {
	char esc;
protected:
	static const int DONE = 0;
	static const int CONTINUE = 1;
	static const int REJECT = 2;
	virtual int process(io::OutStream& out, char chr) = 0;
public:
	Formatter(char esc = '%');
	virtual ~Formatter(void) { }
	int format(io::InStream& in, io::OutStream& out = io::out);
	inline char escape(void) const { return esc; }
	inline void escape(char c) { esc = c; }
};

} // elm

#endif	// ELM_STRING_FORMATTER_H
