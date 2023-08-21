/*
 *	$Id$
 *	EnumOption class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-07, IRIT UPS.
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
#ifndef ELM_OPTION_ENUM_OPTION_H
#define ELM_OPTION_ENUM_OPTION_H

#include <elm/option/Option.h>

namespace elm { namespace option {

// BoolOption class
template <class T>
class EnumOption: public Option {
public:
	typedef Pair<cstring, T> value_t;

	class Make: public Option::Make {
		friend class EnumOption;
	public:
		Make(Manager *m): Option::Make(m), _def(), _def_set(false) { }
		Make(Manager& m): Option::Make(m), _def(), _def_set(false) { }
		inline Make& cmd(string cmd) { Option::Make::cmd(cmd); return *this; }
		inline Make& description(cstring desc) { Option::Make::description(desc); return *this; }
		inline Make& val(cstring n, T v) { _values.add(pair(n, v)); return *this; }
		inline Make& def(T v) { _def = v; _def_set = true; return *this; }
		inline Make& arg(cstring d) { _arg = d; return *this; }
	private:
		Vector<value_t> _values;
		T _def;
		bool _def_set;
		cstring _arg;
	};

	inline EnumOption(const Make& m): Option(m),
		val(m._def_set ? m._def : m._values[0].snd), vals(m._values), arg(m._arg) { }

	inline T value(void) const { return val; }
	inline void set(const T& value) { val = value; }

	// Option overload
	usage_t usage(void) override { return arg_required; }
	
	void process(String arg) override {
		for(auto v: vals) if(arg == v.fst) { val = v.snd; return; }
		throw OptionException(_ << "bad value \"" << arg << "\"");
	}

	cstring argDescription(void) override { return arg; }

	// Operators
	inline operator T(void) const { return value(); }
	inline EnumOption& operator=(const T& value) { set(value); return *this; }

private:
	T val;
	Vector<value_t> vals;
	cstring arg;
};

} } // elm::option

#endif // ELM_OPTION_ENUM_OPTION_H
