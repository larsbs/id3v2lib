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

ID3v2_text_frame* tag_get_text_frame(ID3v2_tag* tag, char* frame_id)
{
    if (frame_id[0] != 'T')
    {
        fprintf(stderr, "%d : '%s' is not a valid text frame id.\n", __LINE__, frame_id);
        return NULL;
    }

    if (tag == NULL)
    {
        return NULL;
    }

    return (ID3v2_text_frame*) frame_list_get_frame_by_id(tag->frames, frame_id);
}

ID3v2_text_frame* ID3v2_tag_get_artist_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, ARTIST_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_album_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, ALBUM_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_title_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, TITLE_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_track_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, TRACK_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_album_artist_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, ALBUM_ARTIST_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_genre_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, GENRE_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_year_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, YEAR_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_disc_number_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, DISC_NUMBER_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_composer_frame(ID3v2_tag* tag)
{
    return tag_get_text_frame(tag, COMPOSER_FRAME_ID);
}

/**
 * Since the ID3v2 spec allows defining multiple COMM frames in a
 * single tag, this gets the first COMM frame found. To retrieve every
 * COMM frame found use ID3v2_tag_get_comment_frames(tag) instead.
*/
ID3v2_comment_frame* ID3v2_tag_get_comment_frame(ID3v2_tag* tag)
{
    if (tag == NULL)
    {
        return NULL;
    }

    return (ID3v2_comment_frame*) frame_list_get_frame_by_id(tag->frames, COMMENT_FRAME_ID);
}

/**
 * Since the ID3v2 spec allows defining multiple COMM frames in a
 * single tag, this gets every COMM frame found. To retrieve only the
 * first COMM frame found use ID3v2_tag_get_comment_frame(tag) instead.
*/
ID3v2_frame_list* ID3v2_tag_get_comment_frames(ID3v2_tag* tag)
{
    if (tag == NULL)
    {
        return NULL;
    }

    return (ID3v2_frame_list*) frame_list_get_frames_by_id(tag->frames, COMMENT_FRAME_ID);
}

ID3v2_apic_frame* ID3v2_tag_get_album_cover_frame(ID3v2_tag* tag)
{
    if (tag == NULL)
    {
        return NULL;
    }

    return (ID3v2_apic_frame*) frame_list_get_frame_by_id(tag->frames, ALBUM_COVER_FRAME_ID);
}
