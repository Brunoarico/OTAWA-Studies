/*
 *	ListGC class interface
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
#ifndef ELM_ALLOC_LISTGC__H_
#define ELM_ALLOC_LISTGC__H_

#include <elm/alloc/AbstractGC.h>

namespace elm {

class ListGC: public AbstractGC {
	class block_t;
public:

	ListGC(GCManager& m, int limit = 1024);
	~ListGC();

	void *allocate(t::size size) override;
	void runGC() override;
	bool mark(void *data, t::size size) override;
	void disable() override;
	void enable() override;
	void clean() override;

private:
	inline bool gcNeeded() const { return !dis && cnt - lcnt > lim; }
	int lcnt, cnt, lim;
	block_t *head;
	bool dis;
};

} // elm

#endif /* ELM_ALLOC_LISTGC__H_ */
