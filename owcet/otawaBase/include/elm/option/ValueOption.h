/*
 *	$Id$
 *	ValueOption class interface
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
#ifndef ELM_OPTION_VALUEOPTION_H_
#define ELM_OPTION_VALUEOPTION_H_

#include <elm/option/Option.h>
#include <elm/type_info.h>

namespace elm { namespace option {

// AbstractValueOption class
class AbstractValueOption: public Option {
public:
	class Make: public Option::Make {
		friend class AbstractValueOption;
	public:
		inline Make(Manager *man): Option::Make(man), _usage(arg_required) { }
		inline Make(Manager& man): Option::Make(man), _usage(arg_required) { }
		inline Make& argDescription(cstring s) { arg_desc = s; return *this; }
		inline Make& arg(cstring s) { arg_desc = s; return *this; }
		inline Make& usage(usage_t u) { _usage = u; return *this; }
		inline Make& cmd(string c) { Option::Make::cmd(c); return *this; }
		inline Make& description(cstring d) { Option::Make::description(d); return *this; }
		inline Make& help(cstring d) { Option::Make::description(d); return *this; }
	private:
		cstring arg_desc;
		usage_t _usage;
	};

	AbstractValueOption(const Make& make);
	AbstractValueOption(Manager& man);

	// Option overload
	cstring description(void) override;
	usage_t usage(void) override;
	cstring argDescription(void) override;

private:
	cstring arg_desc;
	usage_t use;
};


// useful inline
template <class T>
inline T read(string arg) { T val; arg >> val; return val; }


// ValueOption<T> class
template <class T>
class ValueOption: public AbstractValueOption {
public:
	class Make: public AbstractValueOption::Make {
		friend class ValueOption<T>;
	public:
		inline Make(Manager *man): AbstractValueOption::Make(man) { }
		inline Make(Manager& man): AbstractValueOption::Make(man) { }
		inline Make& argDescription(cstring s) { AbstractValueOption::Make::argDescription(s); return *this; }
		inline Make& arg(cstring s) { AbstractValueOption::Make::argDescription(s); return *this; }
		inline Make& usage(usage_t u) { AbstractValueOption::Make::usage(u); return *this; }
		inline Make& cmd(string c) { Option::Make::cmd(c); return *this; }
		inline Make& description(cstring d) { Option::Make::description(d); return *this; }
		inline Make& help(cstring d) { Option::Make::description(d); return *this; }
		inline Make& def(const T& d) { _def = d; return *this; }
	private:
		T _def;
	};

	inline ValueOption(void) { }
	inline ValueOption(const Make& make): AbstractValueOption(make), val(make._def) { }

	inline const T& get(void) const { return val; };
	inline void set(const T& value) { val = value; };

	// Operators
	inline operator const T&(void) const { return get(); };
	inline ValueOption<T>& operator=(const T& value) { set(value); return *this; };
	inline const T& operator*(void) const { return get(); }
	inline operator bool(void) const { return get(); }

	// Option overload
	virtual void process(String arg) { val = read<T>(arg); }

	// deprecated
	inline const T& value(void) const { return val; };

private:
	T val;
	inline T get(VarArg& args) { return args.next<T>(); }
};


// alias
template <class T> class Value: public ValueOption<T> {
public:
	inline Value(const typename ValueOption<T>::Make& make): ValueOption<T>(make) { }
};

// read specializations
template <> string read<string>(string arg);
template <> cstring read<cstring>(string arg);

} }	// elm::option

#endif /* ELM_OPTION_VALUEOPTION_H_ */
