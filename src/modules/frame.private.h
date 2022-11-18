/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_frame_private_h
#define id3v2lib_frame_private_h

#include "modules/frame.h"

ID3v2_frame* frame_parse(const char* buffer, int id3_major_version);
ID3v2_text_frame_data* text_frame_data_parse(const char* buffer, int frame_size);
ID3v2_comment_frame_data* comment_frame_data_parse(const char* buffer, int frame_size);
ID3v2_apic_frame_data* apic_frame_data_parse(const char* buffer, int frame_size);

#endif
