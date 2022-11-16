/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdlib.h>

#include "frame.private.h"
#include "utils.private.h"

ID3v2_frame_header* frame_header_new()
{
    ID3v2_frame_header* frame_header = (ID3v2_frame_header*) malloc(sizeof(ID3v2_frame_header));
    return frame_header;
}

ID3v2_frame* frame_new()
{
    ID3v2_frame* frame = (ID3v2_frame*) malloc(sizeof(ID3v2_frame));
    return frame;
}

ID3v2_frame* frame_parse(char* buffer, int id3_major_version)
{
    ID3v2_frame_header* frame_header = frame_header_new();

    // Start parsing the frame header
    memcpy(frame_header->id, buffer, ID3v2_FRAME_HEADER_ID_LENGTH);

    // If the id is 0000, that means we already reached the end and we're inside the padding
    if (memcmp(frame_header->id, "\0\0\0\0", 4) == 0)
    {
        free(frame_header);
        return NULL;
    }

    int cursor = 0;

    frame_header->size = btoi(buffer + (cursor += ID3v2_FRAME_HEADER_ID_LENGTH), ID3v2_FRAME_HEADER_SIZE_LENGTH);

    if (id3_major_version == 4)
    {
        frame_header->size = syncint_decode(frame_header->size);
    }

    memcpy(frame_header->flags, buffer + (cursor += ID3v2_FRAME_HEADER_SIZE_LENGTH), 2);

    // Parse the actual frame data
    ID3v2_frame* frame = frame_new();
    frame->header = frame_header;
    frame->data = (char*) malloc(frame->header->size * sizeof(char));
    memcpy(frame->data, buffer + (cursor += ID3v2_FRAME_HEADER_FLAGS_LENGTH), frame->header->size);

    return frame;
}
