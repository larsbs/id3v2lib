/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_id3v2lib_h
#define id3v2lib_id3v2lib_h

#ifdef __cplusplus
extern "C" {
#endif

#include "modules/tag.h"

ID3v2_tag* ID3v2_read_tag(const char* file_name);
ID3v2_tag* ID3v2_read_tag_from_buffer(const char* tag_buffer, int buffer_size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
