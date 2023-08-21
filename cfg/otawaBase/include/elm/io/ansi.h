/*
 *	ANSI codes for output (interface)
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
#ifndef ELM_IO_ANSI_H_
#define ELM_IO_ANSI_H_

#include <elm/io/Output.h>

namespace elm { namespace io {

class ANSIManager {
public:
	typedef cstring t;
	static void print(io::Output& out, cstring text);
};

typedef Tag<ANSIManager> ANSICode;

extern ANSICode
	PLAIN,
	BOLD,
	FAINT,
	ITALIC,
	UNDERLINE,
	BLINK,
	RAPID_BLINK,
	REVERSE;

extern ANSICode
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
	BRIGHT_BLACK,
	BRIGHT_RED,
	BRIGHT_GREEN,
	BRIGHT_YELLOW,
	BRIGHT_BLUE,
	BRIGHT_MAGENTA,
	BRIGHT_CYAN,
	BRIGHT_WHITE;

extern ANSICode
	BACK_BLACK,
	BACK_RED,
	BACK_GREEN,
	BACK_YELLOW,
	BACK_BLUE,
	BACK_MAGENTA,
	BACK_CYAN,
	BACK_WHITE,
	BACK_BRIGHT_BLACK,
	BACK_BRIGHT_RED,
	BACK_BRIGHT_GREEN,
	BACK_BRIGHT_YELLOW,
	BACK_BRIGHT_BLUE,
	BACK_BRIGHT_MAGENTA,
	BACK_BRIGHT_CYAN,
	BACK_BRIGHT_WHITE;

} }		// elm::io

#endif /* ELM_IO_ANSI_H_ */
