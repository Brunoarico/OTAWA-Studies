/*
 * GEL++ PE-COFF File interface
 * Copyright (c) 2020, IRIT- université de Toulouse
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

#ifndef GELPP_PECOFF_FILE_H_
#define GELPP_PECOFF_FILE_H_

#include <gel++/File.h>

namespace gel { namespace pecoff {

typedef struct {
	char signature[4];
	t::uint16 machine;
	t::uint16 number_of_sections;
	t::uint32 time_date_stamp;
	t::uint32 pointer_to_symbol_table;	// deprecated
	t::uint32 number_of_symbols;		// deprecated
	t::uint16 size_of_optional_header;
	t::uint16 characteristics;
} coff_header_t;

typedef enum {
	PE32 = 0x10b,
	PE32P = 0x20b
} file_type_t;

typedef struct {
	t::uint16 magic;
	t::uint8 major_linker_version;
	t::uint8 minor_linker_version;
	t::uint32 size_of_code;
	t::uint32 size_of_initialized_data;
	t::uint32 size_of_unitialized_data;
	t::uint32 address_of_entry_point;
	t::uint32 base_of_code;
	t::uint32 base_of_data;				// not in PE32+
} standard_coff_fields_t;

typedef struct {
	t::uint32 image_base;
	t::uint32 section_alignment;
	t::uint32 file_alignment;
	t::uint16 major_operating_system_version;
	t::uint16 minor_operating_system_version;
	t::uint16 major_image_version;
	t::uint16 minor_image_version;
	t::uint16 major_subsystem_version;
	t::uint16 minor_subsystem_version;
	t::uint32 win32_version_value;
	t::uint32 size_of_image;
	t::uint32 size_of_headers;
	t::uint32 checksum;
	t::uint16 subsystem;
	t::uint16 dll_characteristics;
	t::uint32 size_of_stack_reserve;
	t::uint32 size_of_stack_commit;
	t::uint32 size_of_heap_reserve;
	t::uint32 size_of_heap_commit;
	t::uint32 loader_flags;
	t::uint32 number_of_rva_and_sizes;
} windows_specific_fields_32_t;

typedef struct {
	t::uint64 image_base;
	t::uint32 section_alignment;
	t::uint32 file_alignment;
	t::uint16 major_operating_system_version;
	t::uint16 minor_operating_system_version;
	t::uint16 major_image_version;
	t::uint16 minor_image_version;
	t::uint16 major_subsystem_version;
	t::uint16 minor_subsystem_version;
	t::uint32 win32_version_value;
	t::uint32 size_of_image;
	t::uint32 size_of_headers;
	t::uint32 checksum;
	t::uint16 subsystem;
	t::uint16 dll_characteristics;
	t::uint64 size_of_stack_reserve;
	t::uint64 size_of_stack_commit;
	t::uint64 size_of_heap_reserve;
	t::uint64 size_of_heap_commit;
	t::uint32 loader_flags;
	t::uint32 number_of_rva_and_sizes;
} windows_specific_fields_t;


typedef enum {
	IMAGE_RVA_EXPORT_TABLE = 0,
	IMAGE_RVA_IMPORT_TABLE = 1,
	IMAGE_RVA_RESOURCE_TABLE = 2,
	IMAGE_RVA_EXCEPTION_TABLE = 3,
	IMAGE_RVA_CERTIFICATE_TABLE = 4,
	IMAGE_RVA_BASE_RELOCATION_TABLE = 5,
	IMAGE_RVA_DEBUG = 6,
	IMAGE_RVA_ARCHITECTURE = 7,
	IMAGE_RVA_GLOBAL_PTR = 8,
	IMAGE_RVA_TLS_TABLE = 9,
	IMAGE_RVA_LOAD_CONFIG_TABLE = 10,
	IMAGE_RVA_BOUND_IMPORT = 11,
	IMAGE_RVA_IAT = 12,
	IMAGE_RVA_DELAY_IMPORT_DESCRIPTOR = 13,
	IMAGE_RVA_CLR_RUNTIME_HEADER = 14,
	IMAGE_RVA_RESERVED = 15
} rva_t;

typedef struct {
	t::uint32 virtual_address;
	t::uint32 size;
} data_directory_t;

typedef struct {
	char name[8];
	t::uint32 virtual_size;
	t::uint32 virtual_address;
	t::uint32 size_of_raw_data;
	t::uint32 pointer_to_raw_data;
	t::uint32 pointer_to_relocations;
	t::uint32 pointer_to_line_numbers;
	t::uint16 number_of_relocations;
	t::uint16 number_of_line_numbers;
	t::uint32 characteristics;
} section_header_t;

typedef struct {
	union {
		char short_name[8];
		struct {
			t::uint32 zeroes;
			t::uint32 offset;
		} w;
	} name;
	t::uint32 value;
	t::uint16 section_number;
	t::uint16 type;
	t::uint8 storage_class;
	t::uint8 number_of_aux_symbols;
} symbol_t;




class File;
class Section: public gel::Section {
	friend class PECOFF;
public:
	Section(File *file, const section_header_t *header);
	~Section();
	inline const section_header_t&header(void) const { return *hd; }

	cstring name(void) override;
	address_t baseAddress(void) override;
	address_t loadAddress(void) override;
	size_t size(void) override;
	size_t alignment(void) override;
	bool isExecutable(void) override;
	bool isWritable(void) override;
	bool hasContent(void) override;
	Buffer buffer(void) override;
	size_t offset() override;
	size_t fileSize() override;
	flags_t flags() override;

private:
	const section_header_t *hd;
	File *pec;
	string _name;
	t::uint8 *_buf;
	flags_t _flags;
};

class File: public gel::File, public Decoder {
	friend class Section;
public:
	File(Manager& manager, sys::Path path, io::RandomAccessStream *stream);
	~File(void);
	static bool matches(t::uint8 magic[4]);
	
	type_t type(void) override;
	bool isBigEndian(void) override;
	address_type_t addressType(void) override;
	address_t entry(void) override;
	int count() override;
	Segment *segment(int i) override;
	Image *make(const Parameter& params) override;

	string machine() const override;
	string os() const override;
	int elfMachine() const override;
	int elfOS() const override;
	const SymbolTable& symbols() override;

	void fix(t::uint16& w) override;
	void fix(t::int16& w) override;
	void fix(t::uint32& w) override;
	void fix(t::int32& w) override;
	void fix(t::uint64& w) override;
	void fix(t::int64& w) override;

	void unfix(t::uint16& w) override;
	void unfix(t::int16& w) override;
	void unfix(t::uint32& w) override;
	void unfix(t::int32& w) override;
	void unfix(t::uint64& w) override;
	void unfix(t::int64& w) override;

private:
	void raise(const string& msg);
	void read(void *buf, int len);
	void move(offset_t offset);
	cstring getString(t::uint32 offset);
	
	io::RandomAccessStream *stream;
	coff_header_t _coff_header;
	standard_coff_fields_t _standard_coff_fields;
	windows_specific_fields_t _windows_specific_fields;
	data_directory_t *_data_directories;
	section_header_t *_section_table;
	symbol_t *_symbol_table;
	char *_string_table;
	t::uint32 _string_table_size;
	Vector<Section *> sects;
};

} } // gel::pecoff

#endif /* GELPP_PECOFF_FILE_H_ */
