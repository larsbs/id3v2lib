/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_tag_h
#define id3v2lib_tag_h

#include "frame.h"
#include "frame_list.h"

#define ID3v2_TAG_HEADER_LENGTH 10
#define ID3v2_TAG_HEADER_IDENTIFIER_LENGTH 3
#define ID3v2_TAG_HEADER_MAJOR_VERSION_LENGTH 1
#define ID3v2_TAG_HEADER_MINOR_VERSION_LENGTH 1
#define ID3v2_TAG_HEADER_FLAGS_LENGTH 1
#define ID3v2_TAG_HEADER_TAG_SIZE_LENGTH 4
#define ID3v2_EXTENDED_HEADED_SIZE_LENGTH 4

typedef struct _ID3v2_tag_header
{
    char identifier[ID3v2_TAG_HEADER_IDENTIFIER_LENGTH];
    short major_version;
    short minor_version;
    short flags;
    unsigned int tag_size;
    unsigned int extended_header_size;
} ID3v2_tag_header;

typedef struct _ID3v2_tag
{
    char* raw;
    ID3v2_tag_header* header;
    ID3v2_frame_list* frames;
} ID3v2_tag;

ID3v2_text_frame* ID3v2_tag_get_artist(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_album(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_title(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_track(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_album_artist(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_genre(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_year(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_disc_number(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_composer(ID3v2_tag* tag);
ID3v2_comment_frame* ID3v2_tag_get_comment(ID3v2_tag* tag);
ID3v2_apic_frame* ID3v2_tag_get_album_cover(ID3v2_tag* tag);

#endif
