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

#include "frame.private.h"
#include "frame_list.private.h"
#include "id3v2lib.h"
#include "tag.private.h"
#include "tag_header.private.h"

ID3v2_Tag* ID3v2_read_tag(const char* file_name)
{
    ID3v2_TagHeader* tag_header = ID3v2_TagHeader_read(file_name);

    if (tag_header == NULL) return NULL;

    FILE* file = fopen(file_name, "rb");
    int buffer_length = tag_header->tag_size + ID3v2_TAG_HEADER_LENGTH;
    char* tag_buffer = (char*) malloc((buffer_length) * sizeof(char));

    if (tag_buffer == NULL)
    {
        perror("Could not allocate buffer.");
        fclose(file);
        free(tag_header);
        return NULL;
    }

    fread(tag_buffer, buffer_length, 1, file);
    fclose(file);

    ID3v2_Tag* tag = ID3v2_read_tag_from_buffer(tag_buffer, buffer_length);
    free(tag_buffer);

    return tag;
}

ID3v2_Tag* ID3v2_read_tag_from_buffer(const char* tag_buffer, int buffer_length)
{
    CharStream* tag_cs = CharStream_from_buffer(tag_buffer, buffer_length);
    ID3v2_Tag* tag = Tag_parse(tag_cs);
    CharStream_free(tag_cs);
    return tag;
}
