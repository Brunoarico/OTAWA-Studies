/*
 *	serial module macors
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006, IRIT UPS.
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
#ifndef ELM_SERIAL2_MACROS_H
#define ELM_SERIAL2_MACROS_H

#include <elm/rtti/Enum.h>
#include <elm/serial2/serial.h>

#define ELM_SERIALIZABLE_EXTEND(name, base, fields) \
	public: \
		typedef base __base; \
		static const elm::serial2::AbstractClass& __class(void); \
		static const elm::serial2::AbstractClass& __type; \
		virtual const elm::serial2::AbstractClass& __actual_class(void) const { return __type; } \
		template <class T> void __visit(T& e) { e & fields; } \
		template <class T> void __visit(T& e) const { e & fields; } \
	private:

#define ELM_SERIALIZABLE(name, fields) ELM_SERIALIZABLE_EXTEND(name, void, fields)

#define ELM_SERIALIZE(name) \
		const elm::serial2::AbstractClass& name::__class(void) { static elm::serial2::Class<name> _(#name); return _; } \
		const elm::serial2::AbstractClass& name::__type = __class();

#define ELM_SERIALIZE_EXTENDED(name, ext) \
		const elm::serial2::AbstractClass& name::__class(void) { static elm::serial2::Class<name, ext> _(#name); return _; } \
		const elm::serial2::AbstractClass& name::__type = __class();

#define ELM_FIELD(name) elm::field(#name, name)
#define ELM_DFIELD(name, def) elm::field(#name, name, def)
#define ELM_BASE(name) elm::serial2::Base<name>(static_cast<const name *>(this))


#define ELM_ENUM(type) \
	namespace elm { \
		template <> struct type_info<type>: public enum_t<type> { \
			static elm::rtti::Enum::Value __vals[]; \
			static inline CString name(void) { return "<enum " #type ">"; } \
			static inline elm::rtti::Enum::Value *values(void) { return __vals; } \
		}; \
		namespace rtti { template <> inline const Type& _type<type>::_(void){ static Enum _("", elm::type_info<type>::__vals); return _; } } \
		inline elm::io::Output& operator<<(io::Output& out, type value) { out << elm::type_of<type>().asEnum().nameFor(value); return out; } \
	}
#define ELM_ENUM_BEGIN(type) \
	namespace elm { \
		elm::rtti::Enum::Value type_info<type>::__vals[] = {
#define ELM_ENUM_END \
			, rtti::value("", 0) \
		}; \
	}
#define ELM_VALUE(name) elm::rtti::Enum::Value(elm::_unqualify(#name), name)


#ifndef ELM_NO_SHORTCUT
#	define SERIALIZABLE(name, fields) ELM_SERIALIZABLE(name, fields)
#	define SERIALIZABLE_EXTEND(name, base, fields) ELM_SERIALIZABLE_EXTEND(name, base, fields)
#	define SERIALIZE(name) ELM_SERIALIZE(name)
#	define SERIALIZE_EXTENDED(name, ext) ELM_SERIALIZE_EXTENDED(name, ext)
#	define FIELD(name) ELM_FIELD(name)
#	define DFIELD(name, def) ELM_DFIELD(name, def)
#	define BASE(name) ELM_BASE(name)
#	define ENUM(type) ELM_ENUM(type)
#	define VALUE(name) ELM_VALUE(name)
#	define ENUM_BEGIN(type) ELM_ENUM_BEGIN(type)
#	define ENUM_END ELM_ENUM_END
#endif

#endif // ELM_SERIAL2_MACROS_H
