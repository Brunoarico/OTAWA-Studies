/*
 *	BlockAllocator class interface
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
#ifndef ELM_ALLOC_BLOCKALLOCATOR_H_
#define ELM_ALLOC_BLOCKALLOCATOR_H_

#include <elm/alloc/StackAllocator.h>
#include <elm/compare.h>

namespace elm {

// RawBlockAllocator class
template <class T>
class BlockAllocator {

	typedef struct block_t {
		struct block_t *next;
	} block_t;

public:
	static const int default_block_per_chunk = 32;

	BlockAllocator(int block_per_chunk = default_block_per_chunk)
		: alloc(min(sizeof(T), sizeof(block_t)) * block_per_chunk), list(0) { }

	inline T *allocate() {
		if(list != nullptr) {
			block_t *res = list;
			list = list->next;
			return reinterpret_cast<T *>(res);
		}
		else
			return reinterpret_cast<T *>(alloc.allocate(sizeof(T)));
	}

	void free(T *p) {
		block_t *b = reinterpret_cast<block_t *>(p);
		b->next = list;
		list = b;
	}

private:
	StackAllocator alloc;
	block_t *list;
};

} // elm

#endif /* ELM_ALLOC_BLOCKALLOCATOR_H_ */
