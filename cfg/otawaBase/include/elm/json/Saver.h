/*
 *	json::Saver class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2016, IRIT UPS.
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
#ifndef ELM_JSON_SAVER_H_
#define ELM_JSON_SAVER_H_

#include <elm/data/Vector.h>
#include <elm/io.h>
#include <elm/io/BufferedOutStream.h>
#include <elm/io/StructuredOutput.h>
#include <elm/string/utf8.h>
#include <elm/sys/Path.h>
#include "common.h"

namespace elm { namespace json {

class Saver: public io::StructuredOutput {
public:
	Saver(io::OutStream& out = io::out);
	Saver(StringBuffer& buf);
	Saver(sys::Path& path);
	~Saver(void);
	void close(void);

	inline bool isReadable(void) const { return readable; }
	inline void setReadable(bool read) { readable = read; }
	inline string getIndent(void) const { return indent; }
	inline void setIndent(string i) { indent = i; }

	// deprecated
	inline void beginObject(void) { beginMap(); }
	inline void endObject(void) { endMap(); }
	inline void beginArray(void) { beginList(); }
	inline void endArray(void) { endList(); }
	inline void addField(string id) { key(id); }

	// deprecated
	void put(void);
	inline void put(const char *val) { put(cstring(val)); }
	inline void put(cstring val) { write(val); }
	inline void put(string val) { write(val); }
	inline void put(t::uint64 val) { write(val); }
	inline void put(t::int64 val) { write(val); }
	inline void put(int val) { put(t::int64(val)); }
	inline void put(double val) { write(val); }
	inline void put(bool val) { write(val); }

	// StructuredOutput interface
	virtual void write(bool x) override;
	virtual void write(char c) override;
	virtual void write(signed char x) override;
	virtual void write(unsigned char x) override;
	virtual void write(short x) override;
	virtual void write(unsigned short x) override;
	virtual void write(int x) override;
	virtual void write(unsigned int x) override;
	virtual void write(long x) override;
	virtual void write(unsigned long x) override;
	virtual void write(long long int x) override;
	virtual void write(long long unsigned int x) override;
	virtual void write(float x) override;
	virtual void write(double x) override;
	virtual void write(long double x) override;
	virtual void write(const char *s) override;
	virtual void write(cstring x) override;
	virtual void write(const string& x) override;
	virtual void key(cstring x) override;
	virtual void key(const string& x) override;
	virtual void beginMap() override;
	virtual void endMap() override;
	virtual void beginList() override;
	virtual void endList() override;

private:
	typedef enum {
		BEGIN,
		OBJECT,
		IN_OBJECT,
		FIELD,
		ARRAY,
		IN_ARRAY,
		END
	} state_t;

	void doIndent(bool close = false);
	static state_t next(state_t s);
	static bool isObject(state_t s);
	static bool isArray(state_t s);
	void escape(utf8::char_t c);
	inline void nextByValue(void);
	
	state_t state;
	Vector<state_t> stack;
	io::Output _out;
	bool readable;
	string indent;
	io::BufferedOutStream *buf;
	io::OutStream *str;
};

} }	// elm::json

#endif /* ELM_JSON_SAVER_H_ */
