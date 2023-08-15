/*
 * quicksort implementation
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2004-08, IRIT UPS.
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
#ifndef ELM_QUICKSORT_H_
#define ELM_QUICKSORT_H_

#include <elm/compare.h>

namespace elm {

// quick sort
template <class A, class C = Comparator<typename A::t> >
void quicksort(A& array, const C& c = Comparator<typename A::t>()) {
	static const int max = 300;
	int beg[max], end[max], i = 0, L, R, swap;
	typename A::t piv;

	beg[0] = 0; end[0] = array.count();
	while(i >= 0) {
		L = beg[i]; R = end[i] - 1;
		if(L < R) {
			piv = array[L];
			while(L < R) {
				while(c.doCompare(array[R], piv) >= 0 && L < R) R--; if(L < R) array[L++] = array[R];
				while(c.doCompare(array[L], piv) <= 0 && L<R) L++; if (L < R) array[R--] = array[L];
			}
			array[L] = piv; beg[i+1] = L+1; end[i+1] = end[i]; end[i++] = L;
			if(end[i] - beg[i] > end[i-1] - beg[i-1]) {
				swap = beg[i]; beg[i] = beg[i-1]; beg[i-1] = swap;
				swap = end[i]; end[i] = end[i-1]; end[i-1] = swap;
			}
		}
		else
			i--;
	}
}

} // elm

#endif /* ELM_QUICKSORT_H_ */
