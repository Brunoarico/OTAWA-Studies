/*
 * GEL++ ArchPlugin class interface
 * Copyright (c) 2018, IRIT- University of Toulouse
 *
 * GEL++ is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * GEL++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GEL++; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef GEL___ELF_ARCH_PLUGIN_H_
#define GEL___ELF_ARCH_PLUGIN_H_

#include <elm/sys/Plugin.h>
#include <gel++/elf/defs.h>
#include <gel++/base.h>

#define GEL_ELF_ARCH_HOOK	arch_plugin
#define GEL_ELF_ARCH_VERS	Version(1, 0, 0)

namespace gel { namespace elf {

using namespace elm;

class ArchPlugin: public sys::Plugin {
public:
	static ArchPlugin null;
	static ArchPlugin *plug(Elf32_Half machine);

	ArchPlugin(const make& maker);

	virtual void outputDynTag(io::Output& out, int tag);
	virtual void outputDynValue(io::Output& out, int tag, t::uint64 entry);
};

} }	// gel::elf



#endif /* GEL___ELF_ARCH_PLUGIN_H_ */
