/*
 *	Time class interface
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
#ifndef INCLUDE_ELM_UTIL_TIME_H_
#define INCLUDE_ELM_UTIL_TIME_H_

#include <elm/types.h>
#include <elm/io/Output.h>

namespace elm {

class Time {
public:
	const static t::int64
		ONE_MS = 1000,
		ONE_S = ONE_MS * 1000,
		ONE_M = ONE_S * 60,
		ONE_H = ONE_M * 60,
		ONE_D = ONE_H * 24,
		ONE_Y = ONE_D * 365;

	inline Time(): _t(0) { }
	inline Time(int t): _t(t) { }
	inline Time(t::int64 t): _t(t) { }
	inline Time(t::uint64 t): _t(t) { }
	inline Time(const Time& t): _t(t._t) { }

	inline t::int64 time() const { return _t; }
	inline t::int64 micros() const { return _t; }
	inline t::int64 millis() const { return _t / ONE_MS; }
	inline t::int64 seconds() const { return _t / ONE_S; }
	inline t::int64 mins() const { return _t / ONE_M; }
	inline t::int64 hours() const { return _t / ONE_H; }
	inline t::int64 days() const { return _t / ONE_D; }
	inline t::int64 years() const { return _t / ONE_Y; }

	inline Time operator+(const Time& t) const { return _t + t._t; }
	inline Time operator-(const Time& t) const { return _t - t._t; }
	inline Time operator*(int n) const { return _t * n; }
	inline Time operator/(const Time& t) const { return _t / t._t; }

	inline bool operator==(const Time& t) const { return _t == t._t; }
	inline bool operator!=(const Time& t) const { return _t != t._t; }
	inline bool operator<(const Time& t) const { return _t < t._t; }
	inline bool operator<=(const Time& t) const { return _t <= t._t; }
	inline bool operator>(const Time& t) const { return _t > t._t; }
	inline bool operator>=(const Time& t) const { return _t >= t._t; }

private:
	t::int64 _t;
};
io::Output& operator<<(io::Output& out, const Time& t);

};	// elm

#endif /* INCLUDE_ELM_UTIL_TIME_H_ */
