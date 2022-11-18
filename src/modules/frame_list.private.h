/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_frame_list_private_h
#define id3v2lib_frame_list_private_h

#include "modules/frame_list.h"
#include "modules/frame.h"

ID3v2_frame_list* frame_list_new();
void frame_list_add_frame(ID3v2_frame_list* list, ID3v2_frame* frame);
ID3v2_frame* frame_list_get_frame_by_id(ID3v2_frame_list* list, char* frame_id);
ID3v2_frame_list* frame_list_get_frames_by_id(ID3v2_frame_list* list, char* frame_id);

#endif
