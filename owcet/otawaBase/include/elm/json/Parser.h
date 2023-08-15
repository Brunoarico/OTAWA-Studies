/*
 *	json::Parser class interface
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
#ifndef ELM_JSON_PARSER_H_
#define ELM_JSON_PARSER_H_

#include "common.h"
#include <elm/io.h>
#include <elm/sys/Path.h>

namespace elm { namespace json {

class Maker {
public:
	virtual ~Maker(void);
	virtual void beginObject(void);
	virtual void endObject(void);
	virtual void beginArray(void);
	virtual void endArray(void);
	virtual void onField(string name);
	virtual void onNull(void);
	virtual void onValue(bool value);
	virtual void onValue(int value);
	virtual void onValue(double value);
	virtual void onValue(string value);
};

class Parser {
public:
	Parser(Maker& maker);
	void parse(string s);
	inline void parse(cstring s) { parse(string(s)); }
	inline void parse(const char *s) { parse(string(s)); }
	void parse(io::InStream& in);
	void parse(sys::Path path);

private:
	typedef enum {
		NONE = 0,
		LBRACE,
		RBRACE,
		LBRACK,
		RBRACK,
		NULL_TOKEN,
		TRUE,
		FALSE,
		INT,
		FLOAT,
		STRING,
		COMMA,
		COLON
	} token_t;

	void doParsing(io::InStream& in);
	token_t parseNumber(io::InStream& in, char c, int base = 10);
	void parseObject(io::InStream& in);
	void parseArray(io::InStream& in);
	void parseValue(io::InStream& in, token_t t);
	void parseString(io::InStream& in, char c);
	void parseLitt(io::InStream& in, cstring litt);
	void parseComment(io::InStream& in);
	token_t parseBasedNumber(io::InStream& in);

	void error(string message);
	token_t next(io::InStream& in);
	char nextChar(io::InStream& in);
	void pushBack(char c);

	Maker& m;
	int line, col;
	String text;
	char prev;
};

} }		// elm::json

#endif /* ELM_JSON_PARSER_H_ */
