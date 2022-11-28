/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_tag_private_h
#define id3v2lib_tag_private_h

#include "modules/tag.h"
#include "modules/frame.h"
#include "modules/utils.h"

ID3v2_tag_header* tag_header_new();
ID3v2_tag_header* tag_header_load(const char* file_name);
ID3v2_tag_header* tag_header_parse_from_buffer(const char* buffer);

Char_stream* tag_to_char_stream(ID3v2_tag* tag);

#endif
