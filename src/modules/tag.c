/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tag.private.h"
#include "frame_list.private.h"
#include "utils.private.h"
#include "id3v2lib.h"

ID3v2_tag_header* tag_header_new()
{
    ID3v2_tag_header* tag_header = (ID3v2_tag_header*) malloc(sizeof(ID3v2_tag_header));

    if (tag_header != NULL)
    {
        memcpy(tag_header->identifier, "ID3", 3);
        tag_header->minor_version = 0;
        tag_header->major_version = 0;
        tag_header->flags = 0;
        tag_header->tag_size = 0;
        tag_header->extended_header_size = 0;
    }

    return tag_header;
}

ID3v2_tag_header* tag_header_load(const char* file_name)
{
    FILE* file = fopen(file_name, "rb");
    char tag_header_buffer[ID3v2_TAG_HEADER_LENGTH];
    size_t bytes_read = 0;

    if (file == NULL)
    {
        perror("Error opening file.");
        return NULL;
    }

    bytes_read = fread(tag_header_buffer, ID3v2_TAG_HEADER_LENGTH, 1, file);

    if (bytes_read < 1)
    {
        // Less than ID3v2_TAG_HEADER_LENGTH bytes in file
        return NULL;
    }

    return tag_header_parse_from_buffer(tag_header_buffer);
}

ID3v2_tag_header* tag_header_parse_from_buffer(const char* buffer)
{
    bool has_id3v2_tag = memcmp(buffer, "ID3", 3) == 0;

    if (! has_id3v2_tag)
    {
        return NULL;
    }

    int cursor = 0;

    ID3v2_tag_header* tag_header = tag_header_new();
    tag_header->major_version = buffer[cursor += ID3v2_TAG_HEADER_IDENTIFIER_LENGTH];
    tag_header->minor_version = buffer[cursor += ID3v2_TAG_HEADER_MAJOR_VERSION_LENGTH];

    if (tag_header->major_version != 3 && tag_header->major_version != 4)
    {
        // No supported id3 tag found
        free(tag_header);
        return NULL;
    }

    tag_header->flags = buffer[cursor += ID3v2_TAG_HEADER_MINOR_VERSION_LENGTH];
    tag_header->tag_size = syncint_decode(btoi(buffer + (cursor += ID3v2_TAG_HEADER_FLAGS_LENGTH), ID3v2_TAG_HEADER_TAG_SIZE_LENGTH));
    tag_header->extended_header_size = 0;

    if ((tag_header->flags & (1 << 6)) == (1 << 6))
    {
        // An extended header exists, retrieve its size and update the tag_header
        tag_header->extended_header_size = syncint_decode(btoi(buffer + (cursor += ID3v2_TAG_HEADER_TAG_SIZE_LENGTH), ID3v2_EXTENDED_HEADED_SIZE_LENGTH));
    }

    return tag_header;
}

ID3v2_tag* tag_new()
{
    ID3v2_tag* tag = (ID3v2_tag*) malloc(sizeof(ID3v2_tag));
    tag->header = tag_header_new();
    tag->frames = frame_list_new();

    return tag;
}

char* ID3v2_tag_get_artist(ID3v2_tag* tag)
{
    if (tag == NULL)
    {
        return NULL;
    }

    ID3v2_text_frame* artist_frame = (ID3v2_text_frame*) frame_list_get_frame_by_id(tag->frames, ARTIST_FRAME_ID);

    if (artist_frame == NULL)
    {
        return NULL;
    }

    return artist_frame->data->text;
}
