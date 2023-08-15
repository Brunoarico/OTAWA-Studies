/*
 *	Tuple interface and tuple providers.
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2017, IRIT UPS.
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
#ifndef ELM_RTTI_TUPLE_H_
#define ELM_RTTI_TUPLE_H_

#include <elm/rtti/type_of.h>
#include <elm/rtti/Class.h>

namespace elm { namespace rtti {

class Tuple {
public:
	virtual ~Tuple();
	virtual const Vector<const rtti::Type *>& types() const = 0;
	virtual void split(void *ptr, Vector<Variant>& values) const = 0;
	virtual void *make(const Vector<Variant>& values, int i = 0) const = 0;
	inline int count() const { return types().length(); }
};

template <class T>
class AbstractTuple: public AbstractClass, public Tuple {
public:
	AbstractTuple(cstring name): AbstractClass(name) { }
	void *instantiate(void) const override { return new T; }
	void free(void *obj) const override { delete static_cast<T *>(obj); }
	const Tuple *toTuple() const override { return this; }
	const Vector<const rtti::Type *>& types() const override { return _types; }
protected:
	inline void add(const rtti::Type *t) { _types.add(t); }
private:
	Vector<const rtti::Type *> _types;
};

template <class T, class A>
class Tuple1: public AbstractTuple<T> {
public:
	typedef A (T::*acc_t)() const;
	Tuple1(cstring name, acc_t acc): AbstractTuple<T>(name), _acc(acc)
		{ AbstractTuple<T>::add(&type_of<A>()); }
	void split(void *ptr, Vector<Variant>& values) const override
		{ values.add(Variant((static_cast<const T *>(ptr)->*_acc)())); }
	void *make(const Vector<Variant>& values, int i) const override
		{	cerr << "DEBUG: " << values[i].as<A>() << io::endl;
			return new T(values[i].as<A>()); }
private:
	acc_t _acc;
};

template <class T, class A>
AbstractClass *tuple1(cstring name, A (T::*a)() const) { return new Tuple1<T, A>(name, a); }

class Declare {
public:
	inline Declare(const AbstractClass *cls): _cls(cls) { }
	inline const AbstractClass& get() const { return *_cls; }
	inline const AbstractClass& operator*() const { return get(); }
	inline const AbstractClass *operator->() const { return &get(); }
private:
	const AbstractClass *_cls;
};

} }	// elm::rtti

#endif /* ELM_RTTI_TUPLE_H_ */
