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

#define ID3v2_FRAME 10
#define ID3v2_FRAME_HEADER_ID_LENGTH 4
#define ID3v2_FRAME_HEADER_SIZE_LENGTH 4
#define ID3v2_FRAME_HEADER_FLAGS_LENGTH 2
#define ID3v2_FRAME_ENCODING_LENGTH 1
#define ID3v2_FRAME_LANGUAGE_LENGTH 3
#define ID3v2_FRAME_SHORT_DESCRIPTION_LENGTH 1
#define ID3v2_FRAME_PICTURE_TYPE_LENGTH 1

#define INVALID_FRAME 0
#define TEXT_FRAME 1
#define COMMENT_FRAME 2
#define APIC_FRAME 3

#define ISO_ENCODING 0
#define UTF_16_ENCODING 1

#define JPG_MIME_TYPE "image/jpeg"
#define PNG_MIME_TYPE "image/png"

typedef struct _ID3v2_frame_header
{
    char id[ID3v2_FRAME_HEADER_ID_LENGTH];
    int size;
    char flags[ID3v2_FRAME_HEADER_FLAGS_LENGTH];
} ID3v2_frame_header;

typedef struct _ID3v2_frame
{
    ID3v2_frame_header* header;
    char* data;
} ID3v2_frame;

#endif
