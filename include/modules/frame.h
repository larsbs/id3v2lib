/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_frame_h
#define id3v2lib_frame_h

#define ID3v2_FRAME_HEADER_LENGTH 10
#define ID3v2_FRAME_HEADER_ID_LENGTH 4
#define ID3v2_FRAME_HEADER_SIZE_LENGTH 4
#define ID3v2_FRAME_HEADER_FLAGS_LENGTH 2
#define ID3v2_FRAME_ENCODING_LENGTH 1
#define ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH 3
#define ID3v2_APIC_FRAME_PICTURE_TYPE_LENGTH 1

#define ID3v2_ENCODING_ISO 0
#define ID3v2_ENCODING_UNICODE 1

#define ID3v2_MIME_TYPE_JPG "image/jpeg"
#define ID3v2_MIME_TYPE_PNG "image/png"

typedef struct _ID3v2_frame_header
{
    char id[ID3v2_FRAME_HEADER_ID_LENGTH];
    int size;
    char flags[ID3v2_FRAME_HEADER_FLAGS_LENGTH];
} ID3v2_frame_header;

typedef struct _ID3v2_text_frame_data
{
    int size;
    char encoding;
    const char* text;
} ID3v2_text_frame_data;

typedef struct _ID3v2_comment_frame_data
{
    int size;
    char encoding;
    char* comment;
    char* language;
    char* short_description;
} ID3v2_comment_frame_data;

typedef struct _ID3v2_apic_frame_data
{
    char encoding;
    char* data;
    char* mime_type;
    char picture_type;
    char* description;
    int picture_size;
} ID3v2_apic_frame_data;

typedef struct _ID3v2_frame
{
    ID3v2_frame_header* header;
    void* data;
} ID3v2_frame;

typedef struct _ID3v2_text_frame
{
    ID3v2_frame_header* header;
    ID3v2_text_frame_data* data;
} ID3v2_text_frame;

typedef struct _ID3v2_comment_frame
{
    ID3v2_frame_header* header;
    ID3v2_comment_frame_data* data;
} ID3v2_comment_frame;

typedef struct _ID3v2_apic_frame
{
    ID3v2_frame_header* header;
    ID3v2_apic_frame_data* data;
} ID3v2_apic_frame;

#endif
