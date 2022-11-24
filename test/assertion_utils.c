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

#include "test_utils.h"
#include "assertion_utils.h"

void assert_frame_header(ID3v2_frame* frame, Frame_header_assertion comparison)
{
    assert(memcmp(frame->header->id, comparison.id, ID3v2_FRAME_HEADER_ID_LENGTH) == 0);
    assert(frame->header->size == comparison.size);
    assert(memcmp(frame->header->flags, comparison.flags, ID3v2_FRAME_HEADER_FLAGS_LENGTH) == 0);
}

void assert_text_frame(ID3v2_text_frame* frame, ID3v2_text_frame_input* comparison)
{
    const char encoding = has_bom(comparison->text) ? ID3v2_ENCODING_UNICODE : ID3v2_ENCODING_ISO;
    const int str_termination_length = encoding == ID3v2_ENCODING_ISO ? 1 : 2;
    const int text_size = ID3v2_strlen(comparison->text) + str_termination_length;
    const int frame_size = text_size + ID3v2_FRAME_ENCODING_LENGTH;

    // Header
    assert_frame_header((ID3v2_frame*) frame, (Frame_header_assertion) {
        .id = comparison->id,
        .size = frame_size,
        .flags = comparison->flags,
    });

    // Data
    assert(frame->data->size == (frame_size - ID3v2_FRAME_ENCODING_LENGTH));
    assert(frame->data->encoding == encoding);
    assert(memcmp(frame->data->text, comparison->text, frame->data->size) == 0);
}

void assert_comment_frame(ID3v2_comment_frame* frame, ID3v2_comment_frame_input* comparison)
{
    const char comment_encoding = has_bom(comparison->comment) ? ID3v2_ENCODING_UNICODE : ID3v2_ENCODING_ISO;
    const int short_desc_str_termination_length = has_bom(comparison->short_description) ? 2 : 1;
    const int str_termination_length = comment_encoding == ID3v2_ENCODING_ISO ? 1 : 2;

    const int comment_size = ID3v2_strlen(comparison->comment) + str_termination_length;
    const int short_description_size = ID3v2_strlen(comparison->short_description) + short_desc_str_termination_length;
    const int frame_size = comment_size + short_description_size + ID3v2_FRAME_ENCODING_LENGTH + ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH;

    // Header
    assert_frame_header((ID3v2_frame*) frame, (Frame_header_assertion) {
        .id = ID3v2_COMMENT_FRAME_ID,
        .size = frame_size,
        .flags = comparison->flags,
    });

    // Data
    assert(frame->data->size == (frame_size - ID3v2_FRAME_ENCODING_LENGTH - ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH - short_description_size));
    assert(frame->data->encoding == comment_encoding);
    assert(memcmp(frame->data->short_description, comparison->short_description, short_description_size) == 0);
    assert(memcmp(frame->data->comment, comparison->comment, frame->data->size) == 0);
}

void assert_apic_frame(ID3v2_apic_frame* frame, Apic_frame_assertion comparison)
{
    const int bom_length = comparison.encoding == ID3v2_ENCODING_ISO ? 0 : 2;
    const int str_termination_length = comparison.encoding == ID3v2_ENCODING_ISO ? 1 : 2;
    const int scale = comparison.encoding == ID3v2_ENCODING_ISO ? 1 : 2;

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
        .id = ID3v2_ALBUM_COVER_FRAME_ID,
        .size = frame_size,
        .flags = comparison.flags,
    });

    // Data
    assert(frame->data->encoding == comparison.encoding);
    assert(frame->data->picture_size == picture_size);
    assert(memcmp(frame->data->data, picture_data, picture_size) == 0);
    assert(memcmp(frame->data->mime_type, comparison.mime_type, strlen(comparison.mime_type)) == 0);
    assert(memcmp(frame->data->description, comparison.encoding == ID3v2_ENCODING_ISO ? comparison.description : to_unicode(comparison.description), description_size) == 0);

    free(picture_data);
}
