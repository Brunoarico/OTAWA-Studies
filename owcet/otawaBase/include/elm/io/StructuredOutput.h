/*
 *	StructuredOutput class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2021, IRIT UPS.
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
#ifndef ELM_IO_STRUCTURED_OUTPUT_H
#define ELM_IO_STRUCTURED_OUTPUT_H

#include <elm/types.h>

namespace elm { namespace io { 

class StructuredOutput {
public:
	virtual ~StructuredOutput();
	virtual void write(bool x) = 0;
	virtual void write(char c) = 0;
	virtual void write(signed char) = 0;
	virtual void write(unsigned char) = 0;
	virtual void write(short x) = 0;
	virtual void write(unsigned short x) = 0;
	virtual void write(int x) = 0;
	virtual void write(unsigned int x) = 0;
	virtual void write(long x) = 0;
	virtual void write(unsigned long x) = 0;
	virtual void write(long long int x) = 0;
	virtual void write(long long unsigned int x) = 0;
	virtual void write(float x) = 0;
	virtual void write(double x) = 0;
	virtual void write(long double x) = 0;
	virtual void write(const char *s) = 0;
	virtual void write(cstring x) = 0;
	virtual void write(const string& x) = 0;
	inline void key(const char *x) { key(cstring(x)); }
	virtual void key(cstring x) = 0;
	virtual void key(const string& x) = 0;
	virtual void beginMap() = 0;
	virtual void endMap() = 0;
	virtual void beginList() = 0;
	virtual void endList() = 0;
};
	
}}	// otawa::io

#endif 	// ELM_IO_STRUCTURED_OUTPUT_H
