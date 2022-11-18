/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_test_utils_h
#define id3v2lib_test_utils_h

#include "id3v2lib.h"

// This makes MSVC happy without having to download msinttypes
#ifndef _MSC_VER
  #include <inttypes.h>
#else
  typedef unsigned short uint16_t;
#endif

void print_text_frame_text(char* text, int size);
void print_text_frame(ID3v2_text_frame* frame);
void print_comment_frame(ID3v2_comment_frame* frame);
void save_album_cover(ID3v2_apic_frame* frame, char* dir_path);

#endif