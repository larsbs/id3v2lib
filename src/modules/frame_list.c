/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdlib.h>
#include <string.h>

#include "frame_list.private.h"

ID3v2_frame_list* frame_list_new()
{
    ID3v2_frame_list* list = (ID3v2_frame_list*) malloc(sizeof(ID3v2_frame_list));

    if (list != NULL)
    {
        list->frame = NULL;
        list->next = NULL;
        list->start = NULL;
    }

    return list;
}

void frame_list_add_frame(ID3v2_frame_list* list, ID3v2_frame* frame)
{
    // If the list is empty
    if(list->start == NULL)
    {
        list->start = list;
        list->last = list;
        list->frame = frame;
    }
    else
    {
        ID3v2_frame_list* current = frame_list_new();
        current->frame = frame;
        current->start = list->start;
        list->last->next = current;
        list->last = current;
    }
}

/**
 * Returns the first frame matching frame_id
*/
ID3v2_frame* frame_list_get_frame_by_id(ID3v2_frame_list* list, char* frame_id)
{
    while(list != NULL && list->frame != NULL)
    {
        if(strncmp(list->frame->header->id, frame_id, 4) == 0)
        {
            return list->frame;
        }

        list = list->next;
    }

    return NULL;
}

/**
 * Returns all the frames matching frame_id in a sublist
*/
ID3v2_frame_list* frame_list_get_frames_by_id(ID3v2_frame_list* list, char* frame_id)
{
    ID3v2_frame_list* sublist = frame_list_new();

    while(list != NULL && list->frame != NULL)
    {
        if(strncmp(list->frame->header->id, frame_id, 4) == 0)
        {
            frame_list_add_frame(sublist, list->frame);
        }

        list = list->next;
    }

    return sublist;
}

void ID3v2_frame_list_free(ID3v2_frame_list* list)
{
    while(list->next != NULL)
    {
        free(list->frame);
        list = list->next;
    }
}
