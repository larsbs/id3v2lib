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

typedef struct _ID3v2_FrameList
{
    ID3v2_frame* frame;
    struct _ID3v2_FrameList* start;
    struct _ID3v2_FrameList* next;
    struct _ID3v2_FrameList* last;
} ID3v2_FrameList;

void ID3v2_FrameList_free(ID3v2_FrameList* list);

#endif
