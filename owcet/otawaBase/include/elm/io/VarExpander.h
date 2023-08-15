/*
 *	VarExpander class interface
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
#ifndef ELM_IO_VAREXPANDER_H_
#define ELM_IO_VAREXPANDER_H_

#include <elm/string.h>
#include <elm/io/OutStream.h>
#include <elm/avl/Map.h>

namespace elm { namespace io {

class VarExpander: public OutStream {
public:
	VarExpander();
	VarExpander(OutStream& out);
	inline char escape() const { return esc; }
	inline VarExpander& escape(char e) { esc = e; return *this; }
	inline VarExpander& resetBrace() { open = ""; close = ""; return *this; }
	inline VarExpander& brace(char o, char c)
		{ open = open + o; close = close + c; return *this; }
	string expand(string s);

	// OutStream overload
	int write(const char *buffer, int size) override;
	int write(char byte) override;
	int flush(void) override;
	CString lastErrorMessage(void) override;
	bool supportsANSI() const override;

protected:
	virtual string resolve(string name, bool& record);

private:
	char esc;
	string open, close;
	avl::Map<string, string> map;
	enum {
		COPY,
		ESC,
		OPEN,
	} state;
	char expected;
	StringBuffer name;
	OutStream *_out;
};

class OSExpander: public VarExpander {
protected:
	string resolve(string name, bool& record) override;
};

string expandOSVars(string t);

} }	// elm::io

#endif /* ELM_IO_VAREXPANDER_H_ */
