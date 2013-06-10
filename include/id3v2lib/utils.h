/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_utils_h
#define id3v2lib_utils_h

#include <inttypes.h>

#include "types.h"

unsigned int btoi(char* bytes, int size, int offset);
char* itob(int integer);
int syncint_encode(int value);
int syncint_decode(int value);
void add_to_list(ID3v2_frame_list* list, ID3v2_frame* frame);
ID3v2_frame* get_from_list(ID3v2_frame_list* list, char* frame_id);
void free_tag(ID3v2_tag* tag);
char* get_mime_type_from_filename(const char* filename);

// String functions
int has_bom(uint16_t* string);
uint16_t* char_to_utf16(char* string, int size);
void println_utf16(uint16_t* string, int size);
char* get_path_to_file(const char* file);

#endif
