/*
 *	dtd module interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2020, IRIT UPS.
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
#ifndef INCLUDE_ELM_XOM_DTD_H_
#define INCLUDE_ELM_XOM_DTD_H_

#include <elm/data/HashMap.h>
#include <elm/data/List.h>
#include <elm/io/InStream.h>
#include <elm/xom.h>

namespace elm { namespace dtd {

using namespace elm;

class Exception: public elm::MessageException {
public:
	Exception(xom::Element *element, const string& message);
};

class Element;
class AbstractAttribute;
class Factory;

class Parser {
	friend class Element;
public:

	static const t::uint32 CROP = 0x01;

	Parser(Factory& factory, Element& element, t::uint32 flags = CROP);

	inline bool doesCrop() const { return _flags & CROP; }
	inline Factory& factory(void) const { return _fact; }
	inline Element& element(void) const { return *cur.elt; }
	inline Option<xom::String> get(xom::String name) const
		{ return asElement()->getAttributeValue(name); }
	inline xom::Element *asElement() const
		{ return static_cast<xom::Element *>(cur.node); }
	inline xom::Element *parent() const
		{ ASSERT(!stack.isEmpty()); return static_cast<xom::Element *>(stack.top().node); }
	bool isEmpty();

	inline void raise(const string& msg) const { throw Exception(asElement(), msg); }
	inline void fail(const string& msg) const { throw Exception(_last_error, msg); }
	inline void recordError() { _last_error = asElement(); }

	inline bool ended() const { return cur.node == nullptr; }
	inline xom::Node *current(void) const { return cur.node; }
	inline void next(void) { cur.i++; setNode(); }

	typedef int mark_t;
	inline mark_t mark(void) { return cur.i; }
	inline bool backtrack(mark_t m) { cur.i = m; setNode(); return false; }

	void begin(Element& element);
	void end(bool success);

	inline bool hasID(xom::String id) const { return _map.hasKey(id); };
	inline void *getID(xom::String id) const { return _map.get(id, nullptr); };
	inline void addPost(AbstractAttribute *att) { _posts.add(att); }
	void recordPatch(xom::String id, AbstractAttribute& attr);
	void recordID(xom::String id, Element& element);

	void parse(xom::Element *xelt);

private:
	void setNode();

	struct backpatch_t {
		inline backpatch_t(AbstractAttribute *a, void *o, xom::Element *xe)
			: attr(a), object(o), xelt(xe) { }
		AbstractAttribute *attr;
		void *object;
		xom::Element *xelt;
	};

	Factory& _fact;
	Element& _elt;
	typedef struct {
		int i;
		Element *elt;
		xom::Node *node;
	} context_t;
	context_t cur;
	Vector<context_t> stack;
	HashMap<cstring, void *> _map;
	HashMap<cstring, List<backpatch_t> > _patches;
	t::uint32 _flags;
	xom::Element *_last_error;
	Vector<AbstractAttribute *> _posts;
};


class Factory {
public:
	virtual ~Factory();
	virtual void begin(Element& element);
	virtual void end(Element& element);
	virtual void backtrack(Element& element);
	virtual void *getRef(Element& element);
	virtual void getPCDATA(xom::String data);
	virtual void patch(AbstractAttribute& attr, void *object, void *ref);
	virtual void *getPatchRef(AbstractAttribute& attr);
	virtual void getAny(xom::Node *node);
};

class AbstractAttribute {
	friend class Element;
	friend class Parser;
public:
	static const t::uint32
		REQUIRED = 0x01,
		STRICT = 0x02,
		FORWARD = 0x04;
	AbstractAttribute(Element& element, xom::String name, t::uint32 flags = 0);
	virtual ~AbstractAttribute();
	inline xom::String name() const { return _name; }

	inline Element& element() const { return _elt; }
	inline bool isRequired() const { return _flags & REQUIRED; }
	inline bool isStrict() const { return _flags & STRICT; }
	inline bool isForward() const { return _flags & FORWARD; }
	inline bool isSet() const { return _set; }

	inline bool operator==(const AbstractAttribute& a) const { return  this == &a; }
	inline bool operator!=(const AbstractAttribute& a) const { return  !operator==(a); }

protected:
	bool parse(Parser& parser);
	virtual bool process(Parser& parser, xom::String value);
	virtual void reset();
	virtual void postprocess(Parser& parser);

private:
	Element& _elt;
	xom::String _name;
	t::uint32 _flags;
	bool _set;
};


class Content {
	friend class Element;
public:
	virtual ~Content(void);
	virtual void reset() = 0;

protected:
	inline bool doParse(Content& content, Parser& parser) { return content.parse(parser); }
	virtual bool parse(Parser& parser) = 0;
};

class Element: public Content {
	friend class Parser;
	friend class AbstractAttribute;
public:
	Element(xom::String name);
	Element(xom::String name, Content& content);
	~Element();

	inline xom::String name() const { return _name; }
	inline Content& content() const { return _content; }
	inline const Vector<AbstractAttribute *>& attributes() const { return attrs; }
	inline bool operator==(const Element& element) const { return this == &element; }
	inline bool operator!=(const Element& element) const { return !operator==(element); }

	void parse(Factory& factory, xom::Element *element, t::uint32 flags = Parser::CROP);
	void parse(Factory& factory, xom::Document *doc, t::uint32 flags = Parser::CROP);
	void parse(Factory& factory, string uri, t::uint32 flags = Parser::CROP);
	void parse(Factory& factory, io::InStream& in, t::uint32 flags = Parser::CROP);

protected:
	void reset() override;
	bool parse(Parser& parser) override;
private:
	xom::String _name;
	Vector<AbstractAttribute *> attrs;
	Content& _content;
};


class Optional: public Content {
public:
	Optional(Content& content);
protected:
	void reset() override;
	bool parse(Parser& parser) override;
private:
	Content& con;
};


class Alt: public Content {
public:
	Alt(Content& content1, Content& content2);
protected:
	bool parse(Parser& parser) override;
	void reset() override;
private:
	Content &con1, &con2;
};

class Seq: public Content {
public:
	Seq(Content& content1, Content& content2);
protected:
	bool parse(Parser& parser) override;
	void reset() override;
private:
	Content &con1, &con2;
};


class Repeat: public Content {
public:
	Repeat(Content& content);
protected:
	bool parse(Parser& parser) override;
	void reset() override;
private:
	Content& con;
};


template <class T>
class Attribute: public AbstractAttribute {
public:
	Attribute(Element& element, xom::String name, const T& init, t::uint32 flags = 0)
		: AbstractAttribute(element, name, flags), i(init) { }
	inline const T& operator*() const { return v; }
protected:
	bool process(Parser& parser, xom::String value) override
		{ io::StringInput in(value); in >> v; return !in.failed(); }
	void reset() override
		{ AbstractAttribute::reset(); v = i; }
private:
	T v, i;
};

template <>
class Attribute<xom::String>: public AbstractAttribute {
public:
	Attribute(Element& element, xom::String name, const xom::String& init, t::uint32 flags = 0)
		: AbstractAttribute(element, name, flags), i(init) { }
	inline xom::String operator*() const { return v; }
protected:
	bool process(Parser& parser, xom::String value) override { v = value; return true; }
	void reset() override { v = i; }
private:
	xom::String v, i;
};


class IDAttribute: public AbstractAttribute {
public:
	IDAttribute(Element& element, xom::String name, t::uint32 flags = 0);
	inline xom::String operator*() const { return _id; }
protected:
	bool process(Parser& parser, xom::String value) override;
	void reset() override;
	void postprocess(Parser& parser) override;
private:
	xom::String _id;
};


template <class T>
class RefAttribute: public AbstractAttribute {
public:
	inline RefAttribute(Element& element, xom::String name, t::uint32 flags = 0)
		: AbstractAttribute(element, name, flags), _done(false), ref(nullptr) { }
	inline T *operator*() const { return ref; }
	inline bool done() const { return _done; }
	inline xom::String id() const { return _id; }
protected:
	bool process(Parser& parser, xom::String value) override {
		_id = value;
		if(parser.hasID(value)) {
			_done = true;
			ref = static_cast<T *>(parser.getID(value));
			return true;
		}
		else {
			_done = false;
			ref = nullptr;
			if(isForward())
				{ parser.addPost(this); return true; }
			else if(isStrict())
				parser.raise(_ << "undefined reference \"" << value << "\" in " << name());
			return false;
		}
	}

	void reset() override
		{ AbstractAttribute::reset(); _done = false; ref = nullptr; }

	void postprocess(Parser& parser) override
		{ parser.recordPatch(_id, *this); }

private:
	bool _done;
	T *ref;
	xom::String _id;
};

typedef xom::String CDATA;

Content& operator*(Content& c);
Content& operator!(Content& c);
Content& operator+(Content& c1, Content& c2);
Content& operator|(Content& c1, Content& c2);
Content& operator,(Content& c1, Content& c2);
Content& operator&(Content& c1, Content& c2);

const t::uint32 STRICT = AbstractAttribute::STRICT;
const t::uint32 REQUIRED = AbstractAttribute::REQUIRED;
const t::uint32 FORWARD = AbstractAttribute::FORWARD;
const t::uint32 CROP = Parser::CROP;

extern Content& EMPTY;
extern Content& ignored;
extern Content& PCDATA;
extern Content& ANY;

} }		// dtd::elm

#endif /* INCLUDE_ELM_XOM_DTD_H_ */
