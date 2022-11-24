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
    ID3v2_tag_header* header;
    ID3v2_frame_list* frames;
} ID3v2_tag;

ID3v2_tag* ID3v2_tag_new();
void ID3v2_tag_free(ID3v2_tag* tag);

/**
 * Getter functions
*/

/**
 * Returns the first frame found with the provided id. If you need
 * more than the first occurrence, then, use ID3v2_tag_get_frames(tag, frame_id);
*/
ID3v2_frame* ID3v2_tag_get_frame(ID3v2_tag* tag, char* frame_id);

/**
 * Returns all the frames matching frame_id in a sublist. If you only need
 * the first occurrence, then, use ID3v2_tag_get_frame(tag, frame_id);
*/
ID3v2_frame_list* ID3v2_tag_get_frames(ID3v2_tag* tag, char* frame_id);

ID3v2_text_frame* ID3v2_tag_get_artist_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_album_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_title_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_track_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_album_artist_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_genre_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_year_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_disc_number_frame(ID3v2_tag* tag);
ID3v2_text_frame* ID3v2_tag_get_composer_frame(ID3v2_tag* tag);
ID3v2_comment_frame* ID3v2_tag_get_comment_frame(ID3v2_tag* tag);
ID3v2_frame_list* ID3v2_tag_get_comment_frames(ID3v2_tag* tag);
ID3v2_apic_frame* ID3v2_tag_get_album_cover_frame(ID3v2_tag* tag);
ID3v2_frame_list* ID3v2_tag_get_apic_frames(ID3v2_tag* tag);

/**
 * Setter functions
*/
typedef struct _ID3v2_text_frame_input
{
    char* id;
    char* flags;
    char* text;
} ID3v2_text_frame_input;

void ID3v2_tag_set_text_frame(ID3v2_tag* tag, ID3v2_text_frame_input* input);

void ID3v2_tag_set_artist(ID3v2_tag* tag, const char* artist);
void ID3v2_tag_set_album(ID3v2_tag* tag, const char* album);
void ID3v2_tag_set_title(ID3v2_tag* tag, const char* title);
void ID3v2_tag_set_track(ID3v2_tag* tag, const char* track);
void ID3v2_tag_set_album_artist(ID3v2_tag* tag, const char* album_artist);
void ID3v2_tag_set_genre(ID3v2_tag* tag, const char* genre);
void ID3v2_tag_set_year(ID3v2_tag* tag, const char* year);
void ID3v2_tag_set_disc_number(ID3v2_tag* tag, const char* disc_number);
void ID3v2_tag_set_composer(ID3v2_tag* tag, const char* composer);

typedef struct _ID3v2_comment_frame_input
{
    char* flags;
    char* language;
    char* short_description;
    char* comment;
} ID3v2_comment_frame_input;

void ID3v2_tag_set_comment_frame(ID3v2_tag* tag, ID3v2_comment_frame_input* input);
void ID3v2_tag_add_comment_frame(ID3v2_tag* tag, ID3v2_comment_frame_input* input);

void ID3v2_tag_set_comment(ID3v2_tag* tag, const char* language, const char* comment);

typedef struct _ID3v2_apic_frame_input
{
    char* flags;
    char* mime_type;
    char* description;
    char picture_type;
    int picture_size;
    char* data;
} ID3v2_apic_frame_input;

void ID3v2_tag_add_apic_frame(ID3v2_tag* tag, ID3v2_apic_frame_input* input);
void ID3v2_tag_set_apic_frame(ID3v2_tag* tag, ID3v2_apic_frame_input* input);

void ID3v2_tag_set_album_cover(ID3v2_tag* tag, const char* mime_type, const int size, const char* data);

#endif
