/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "assertion_utils.h"

void assert_frame_header(ID3v2_frame* frame, Frame_header_assertion comparison)
{
    assert(memcmp(frame->header->id, comparison.id, ID3v2_FRAME_HEADER_ID_LENGTH) == 0);
    assert(frame->header->size == comparison.size);
    assert(memcmp(frame->header->flags, comparison.flags, ID3v2_FRAME_HEADER_FLAGS_LENGTH) == 0);
}

void assert_text_frame(ID3v2_text_frame* frame, Text_frame_assertion comparison)
{
    const int bom_length = comparison.encoding == ISO_ENCODING ? 0 : 2;
    const int str_termination_length = comparison.encoding == ISO_ENCODING ? 1 : 2;
    const int scale = comparison.encoding == ISO_ENCODING ? 1 : 2;
    const int text_size = (strlen(comparison.text) * scale) + bom_length + str_termination_length;
    const int frame_size = text_size + ID3v2_FRAME_ENCODING_LENGTH;

    // Header
    assert_frame_header((ID3v2_frame*) frame, (Frame_header_assertion) {
        .id = comparison.id,
        .size = frame_size,
        .flags = comparison.flags,
    });

    // Data
    assert(frame->data->size == (frame_size - ID3v2_FRAME_ENCODING_LENGTH));
    assert(frame->data->encoding == comparison.encoding);
    assert(memcmp(frame->data->text, comparison.encoding == ISO_ENCODING ? comparison.text : to_unicode(comparison.text), frame->data->size) == 0);
}

void assert_comment_frame(ID3v2_comment_frame* frame, Comment_frame_assertion comparison)
{
    const int bom_length = comparison.encoding == ISO_ENCODING ? 0 : 2;
    const int str_termination_length = comparison.encoding == ISO_ENCODING ? 1 : 2;
    const int scale = comparison.encoding == ISO_ENCODING ? 1 : 2;

    const int comment_size = (strlen(comparison.comment) * scale) + bom_length + str_termination_length;
    const int short_description_size = (strlen(comparison.short_description) * scale) + bom_length + str_termination_length;
    const int frame_size = comment_size + short_description_size + ID3v2_FRAME_ENCODING_LENGTH + ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH;

    // Header
    assert_frame_header((ID3v2_frame*) frame, (Frame_header_assertion) {
        .id = COMMENT_FRAME_ID,
        .size = frame_size,
        .flags = comparison.flags,
    });

    // Data
    assert(frame->data->size == (frame_size - ID3v2_FRAME_ENCODING_LENGTH - ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH - short_description_size));
    assert(frame->data->encoding == comparison.encoding);
    assert(memcmp(frame->data->short_description, comparison.encoding == ISO_ENCODING ? comparison.short_description : to_unicode(comparison.short_description), short_description_size) == 0);
    assert(memcmp(frame->data->comment, comparison.encoding == ISO_ENCODING ? comparison.comment : to_unicode(comparison.comment), frame->data->size) == 0);
}

void assert_apic_frame(ID3v2_apic_frame* frame, Apic_frame_assertion comparison)
{
    const int bom_length = comparison.encoding == ISO_ENCODING ? 0 : 2;
    const int str_termination_length = comparison.encoding == ISO_ENCODING ? 1 : 2;
    const int scale = comparison.encoding == ISO_ENCODING ? 1 : 2;

    assert(comparison.description != NULL);
    const int description_size = (strlen(comparison.description) * scale) + bom_length + str_termination_length;

    // Compute picture_size from test album cover
    FILE* album_cover = fopen(comparison.album_cover_file_path, "rb");
    fseek(album_cover, 0L, SEEK_END);
    const int picture_size = ftell(album_cover);
    rewind(album_cover);

    // Read picture data
    char* picture_data = (char*) malloc(picture_size * sizeof(char));
    fread(picture_data, picture_size, 1, album_cover);
    fclose(album_cover);

    assert(comparison.mime_type != NULL);
    const int mime_type_size = strlen(comparison.mime_type) + 1; // +1 = string termination

    int frame_size = ID3v2_FRAME_ENCODING_LENGTH + mime_type_size + ID3v2_APIC_FRAME_PICTURE_TYPE_LENGTH + description_size + picture_size;

    // Header
    assert_frame_header((ID3v2_frame*) frame, (Frame_header_assertion) {
        .id = ALBUM_COVER_FRAME_ID,
        .size = frame_size,
        .flags = comparison.flags,
    });

    // Data
    assert(frame->data->encoding == comparison.encoding);
    assert(frame->data->picture_size == picture_size);
    assert(memcmp(frame->data->data, picture_data, picture_size) == 0);
    assert(memcmp(frame->data->mime_type, comparison.mime_type, strlen(comparison.mime_type)) == 0);
    assert(memcmp(frame->data->description, comparison.encoding == ISO_ENCODING ? comparison.description : to_unicode(comparison.description), description_size) == 0);
}
