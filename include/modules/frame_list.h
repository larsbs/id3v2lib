/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_frame_list_h
#define id3v2lib_frame_list_h

#include "frame.h"

typedef struct _ID3v2_frame_list
{
    ID3v2_frame* frame;
    struct _ID3v2_frame_list* start;
    struct _ID3v2_frame_list* last;
    struct _ID3v2_frame_list* next;
} ID3v2_frame_list;

#endif
