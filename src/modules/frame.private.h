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
#include "modules/utils.h"

ID3v2_frame_header* frame_header_new(const char* id, const char* flags, const int size);
Char_stream* frame_header_to_char_stream(ID3v2_frame_header* header);

ID3v2_frame* frame_parse(const char* buffer, int id3_major_version);
void frame_free(ID3v2_frame* frame);
Char_stream* frame_to_char_stream(ID3v2_frame* frame);

ID3v2_text_frame* text_frame_new(const char* id, const char* flags, const char* text);
void text_frame_free(ID3v2_text_frame* frame);
Char_stream* text_frame_to_char_stream(ID3v2_text_frame* frame);
ID3v2_text_frame_data* text_frame_data_new(const char* text);
ID3v2_text_frame_data* text_frame_data_parse(const char* buffer, int frame_size);

ID3v2_comment_frame* comment_frame_new(const char* flags, const char* lang, const char* short_desc, const char* comment);
void comment_frame_free(ID3v2_comment_frame* frame);
Char_stream* comment_frame_to_char_stream(ID3v2_comment_frame* frame);
ID3v2_comment_frame_data* comment_frame_data_new(const char* lang, const char* short_desc, const char* comment);
ID3v2_comment_frame_data* comment_frame_data_parse(const char* buffer, int frame_size);

ID3v2_apic_frame* apic_frame_new(const char* flags, const char* description, const char picture_type, const char* mime_type, const int picture_size, const char* data);
void apic_frame_free(ID3v2_apic_frame* frame);
Char_stream* apic_frame_to_char_stream(ID3v2_apic_frame* frame);
ID3v2_apic_frame_data* apic_frame_data_new(const char* description, const char picture_type, const char* mime_type, const int picture_size, const char* picture_data);
ID3v2_apic_frame_data* apic_frame_data_parse(const char* buffer, int frame_size);

#endif
