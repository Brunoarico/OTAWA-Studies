/*
 *	XOMUnserializer class interface
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
#ifndef ELM_SERIAL2_XOM_UNSERIALIZER_H
#define ELM_SERIAL2_XOM_UNSERIALIZER_H

#include <elm/data/HashMap.h>
#include <elm/data/Vector.h>
#include <elm/serial2/serial.h>
#include <elm/serial2/Unserializer.h>
#include <elm/sys/Path.h>
#include <elm/util/Pair.h>

namespace elm {

// Extern classes
namespace xom {
	class Document;
	class Element;
}

namespace serial2 {

// XOMUnserializer class
class XOMUnserializer: public Unserializer {
public:
	const static cstring id_tag, ref_tag, null_tag, class_tag;

	XOMUnserializer(xom::Element *element);
	XOMUnserializer(const char *path);
	XOMUnserializer(cstring path);
	XOMUnserializer(sys::Path path);
	~XOMUnserializer(void);
	inline ExternalSolver& solver(void) const { return *_solver; }
	inline void setSolver(ExternalSolver& solver) { _solver = &solver; }

	// XOMUnserializer overload
	virtual void flush(void);
	virtual void beginObject(const rtti::Type& clazz, void *object);
	virtual void endObject(const rtti::Type& clazz, void *object);
	virtual bool beginField(CString name);
	virtual void endField(void);
	virtual void onPointer(const rtti::Type& clazz, void **object);
	virtual bool beginCompound(void*);
	virtual int countItems(void);
	virtual bool nextItem(void);
	virtual void endCompound(void*);
	virtual int onEnum(const rtti::Type & clazz);
	virtual void onValue(bool& v);
	virtual void onValue(signed int& v);
	virtual void onValue(unsigned int& v);
	virtual void onValue(char& v);
	virtual void onValue(signed char& v);
	virtual void onValue(unsigned char& v);
	virtual void onValue(signed short& v);
	virtual void onValue(unsigned short& v);
	virtual void onValue(signed long& v);
	virtual void onValue(unsigned long& v);
	virtual void onValue(signed long long& v);
	virtual void onValue(unsigned long long& v);
	virtual void onValue(float& v);
	virtual void onValue(double& v);
	virtual void onValue(long double& v);
	virtual void onValue(CString& v);
	virtual void onValue(String& v);

private:
	typedef struct patch_t {
		struct patch_t *next;
		void **ptr;
		inline patch_t(void **_ptr): next(0), ptr(_ptr) { };
	} patch_t;

	typedef struct ref_t {
		const rtti::Type& t;
		void *ptr;
		patch_t *patches;
		inline ref_t(const rtti::Type& type, void *_ptr = 0) : t(type), ptr(_ptr), patches(0) { };
		void put(void **_ptr);
		void record(void *_ptr);
		inline bool isRecorded(void) const { return ptr; }
	} ref_t;

	typedef struct context_t {
		int i;
		xom::Element *elem;
	} context_t;

	xom::Document *doc;
	bool opened;
	context_t ctx;
	elm::io::Input in;
	HashMap<CString,  ref_t *> refs;
	elm::Vector<context_t> stack;
	elm::Vector<Pair<cstring, ref_t *> > pending;
	ExternalSolver *_solver;

	void init(cstring path);
	void embed(const rtti::Type& clazz, void **ptr);
	string xline(xom::Element *element);
	void lookupID(const rtti::Type& type, void *ptr);
};

} } // elm::serial2

#endif // ELM_SERIAL2_XOM_UNSERIALIZER_H

