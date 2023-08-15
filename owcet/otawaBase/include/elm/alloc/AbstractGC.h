/*
 *	AbstractGC class interface
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2010, IRIT UPS.
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
#ifndef ELM_ALLOC_ABSTRACTGC_H_
#define ELM_ALLOC_ABSTRACTGC_H_

#include <elm/data/List.h>
#include <elm/types.h>

namespace elm {

class AbstractGC;

class GCManager {
public:
	virtual ~GCManager();
	virtual void collect(AbstractGC& gc) = 0;
	virtual void clean(void *p);
};

class AbstractGC {
public:
	inline AbstractGC(GCManager& m): manager(m) { }
	virtual ~AbstractGC();

	// allocator interface
	virtual void *allocate(t::size size) = 0;
	virtual void free(void *block);
	template <class T> void *alloc() { return allocate(sizeof(T)); }

	// GC part
	virtual void runGC() = 0;
	virtual bool mark(void *data, t::size size) = 0;
	virtual void disable() = 0;
	virtual void enable() = 0;
	virtual void clean() = 0;

protected:
	GCManager& manager;
};

}	// elm

#endif /* ELM_ALLOC_ABSTRACTGC_H_ */
