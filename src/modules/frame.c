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

#include "modules/frame_ids.h"
#include "frame.private.h"
#include "utils.private.h"

ID3v2_frame_header* frame_header_new(const char* id, const char* flags, const int size)
{
    ID3v2_frame_header* frame_header = (ID3v2_frame_header*) malloc(sizeof(ID3v2_frame_header));

    memcpy(frame_header->id, id, ID3v2_FRAME_HEADER_ID_LENGTH);
    memcpy(frame_header->flags, flags, ID3v2_FRAME_HEADER_FLAGS_LENGTH);
    frame_header->size = size;

    return frame_header;
}

ID3v2_frame* frame_parse(const char* buffer, int id3_major_version)
{
    // Start parsing the frame header
    const char id[ID3v2_FRAME_HEADER_ID_LENGTH];
    memcpy(id, buffer, ID3v2_FRAME_HEADER_ID_LENGTH);

    // If the id is 0000, that means we already reached the end and we're inside the padding
    if (memcmp(id, "\0\0\0\0", 4) == 0)
    {
        return NULL;
    }

    int cursor = 0;

    int size = btoi(buffer + (cursor += ID3v2_FRAME_HEADER_ID_LENGTH), ID3v2_FRAME_HEADER_SIZE_LENGTH);

    if (id3_major_version == 4)
    {
        size = syncint_decode(size);
    }

    const char flags[ID3v2_FRAME_HEADER_FLAGS_LENGTH];
    memcpy(flags, buffer + (cursor += ID3v2_FRAME_HEADER_SIZE_LENGTH), ID3v2_FRAME_HEADER_FLAGS_LENGTH);
    cursor += ID3v2_FRAME_HEADER_FLAGS_LENGTH;

    ID3v2_frame* frame = (ID3v2_frame*) malloc(sizeof(ID3v2_frame));

    frame->header = frame_header_new(id, flags, size);

    if (frame->header->id[0] == 'T')
    {
        // It's a text information frame
        frame->data = text_frame_data_parse(buffer + cursor, frame->header->size);
    }
    else if (frame->header->id[0] == 'C')
    {
        // It's a comment information frame
        frame->data = comment_frame_data_parse(buffer + cursor, frame->header->size);
    }
    else if (frame->header->id[0] == 'A')
    {
        // It's an attached picture frame
        frame->data = apic_frame_data_parse(buffer + cursor, frame->header->size);
    }
    else
    {
        // Unknown frame type, simply copy the raw data into the data property
        frame->data = (char*) malloc(size * sizeof(char));
        memcpy(frame->data, buffer + cursor, size);
    }

    return frame;
}

void frame_free(ID3v2_frame* frame)
{
    if (frame->header->id[0] == 'T')
    {
        text_frame_free((ID3v2_text_frame*) frame);
    }
    else if (frame->header->id[0] == 'C')
    {
        comment_frame_free((ID3v2_comment_frame*) frame);
    }
    else if (frame->header->id[0] == 'A')
    {
        // apic_frame_free((ID3v2_apic_frame*) frame);
    }
    else
    {
        // Unknown frame id, so naivelly try our best to free it
        free(frame->header);
        free(frame->data);
    }
}

ID3v2_text_frame* text_frame_new(const char* id, const char* flags, const char* text)
{
    ID3v2_text_frame* frame = (ID3v2_text_frame*) malloc(sizeof(ID3v2_text_frame));

    frame->data = text_frame_data_new(text);

    const int frame_size = ID3v2_FRAME_ENCODING_LENGTH + frame->data->size;
    frame->header = frame_header_new(id, flags, frame_size);

    return frame;
}

void text_frame_free(ID3v2_text_frame* frame)
{
    free(frame->header);
    free(frame->data->text);
    free(frame->data);
    free(frame);
}

ID3v2_text_frame_data* text_frame_data_new(const char* text)
{
    const char encoding = string_has_bom(text) ? ID3v2_ENCODING_UNICODE : ID3v2_ENCODING_ISO;
    const int str_termination_size = encoding == ID3v2_ENCODING_ISO ? 1 : 2;
    const int size = ID3v2_strlen(text) + str_termination_size;

    ID3v2_text_frame_data* data = (ID3v2_text_frame_data*) malloc(sizeof(ID3v2_text_frame_data));
    data->text = (char*) malloc(size * sizeof(char));

    data->encoding = encoding;
    data->size = size;
    data->text = text;

    return data;
}

ID3v2_text_frame_data* text_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    const int size = frame_size - ID3v2_FRAME_ENCODING_LENGTH;
    char* text = (char*) malloc(size * sizeof(char));
    memcpy(text, buffer + ID3v2_FRAME_ENCODING_LENGTH, size);

    return text_frame_data_new(text);
}

ID3v2_comment_frame* comment_frame_new(const char* flags, const char* lang, const char* short_desc, const char* comment)
{
    ID3v2_comment_frame* frame = (ID3v2_comment_frame*) malloc(sizeof(ID3v2_comment_frame));

    frame->data = comment_frame_data_new(lang, short_desc, comment);

    const int short_desc_size = ID3v2_strlent(short_desc);
    const int frame_size = frame->data->size + short_desc_size + ID3v2_FRAME_ENCODING_LENGTH + ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH;

    frame->header = frame_header_new(ID3v2_COMMENT_FRAME_ID, flags, frame_size);

    return frame;
}

void comment_frame_free(ID3v2_comment_frame* frame)
{
    free(frame->header);
    free(frame->data->comment);
    free(frame->data->language);
    free(frame->data->short_description);
    free(frame->data);
    free(frame);
}

ID3v2_comment_frame_data* comment_frame_data_new(const char* lang, const char* short_desc, const char* comment)
{
    const char encoding = string_has_bom(comment) ? ID3v2_ENCODING_UNICODE : ID3v2_ENCODING_ISO;
    const int comment_size = ID3v2_strlent(comment);
    const int short_desc_size = ID3v2_strlent(short_desc);

    ID3v2_comment_frame_data* data = (ID3v2_comment_frame_data*) malloc(sizeof(ID3v2_comment_frame_data));

    data->encoding = encoding;

    data->size = comment_size;

    data->language = (char*) malloc(ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH * sizeof(char));
    memcpy(data->language, lang, ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH);

    data->comment = (char*) malloc(comment_size * sizeof(char));
    memcpy(data->comment, comment, comment_size);

    data->short_description = (char*) malloc(short_desc_size * sizeof(char));
    memcpy(data->short_description, short_desc, short_desc_size);

    return data;
}

ID3v2_comment_frame_data* comment_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    int cursor = 0;

    const char* language = (char*) malloc(ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH * sizeof(char));
    memcpy(language, buffer + (cursor += ID3v2_FRAME_ENCODING_LENGTH), ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH);

    int short_desc_size = ID3v2_strlent(buffer + (cursor += ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH));
    const char* short_description = (char*) malloc(short_desc_size * sizeof(char));
    memcpy(short_description, buffer + cursor, short_desc_size);

    int comment_length = ID3v2_strlent(buffer + (cursor += short_desc_size));
    const char* comment = (char*) malloc(comment_length * sizeof(char));
    memcpy(comment, buffer + cursor, comment_length);

    return comment_frame_data_new(language, short_description, comment);
}

ID3v2_apic_frame* apic_frame_new(const char* flags, const char* description, const char picture_type, const char* mime_type, const int picture_size, const char* data)
{
    ID3v2_apic_frame* frame = (ID3v2_apic_frame*) malloc(sizeof(ID3v2_apic_frame));

    frame->data = apic_frame_data_new(description, picture_type, mime_type, picture_size, data);

    const int description_size = ID3v2_strlent(description);
    const int mime_type_size = ID3v2_strlent(mime_type);
    const int frame_size = ID3v2_FRAME_ENCODING_LENGTH + mime_type_size + ID3v2_APIC_FRAME_PICTURE_TYPE_LENGTH + description_size + frame->data->picture_size;

    frame->header = frame_header_new(ID3v2_ALBUM_COVER_FRAME_ID, flags, frame_size);

    return frame;
}

void apic_frame_free(ID3v2_apic_frame* frame)
{
    free(frame->header);
    free(frame->data->data);
    free(frame->data->description);
    free(frame->data->mime_type);
    free(frame->data);
    free(frame);
}

ID3v2_apic_frame_data* apic_frame_data_new(const char* description, const char picture_type, const char* mime_type, const int picture_size, const char* picture_data)
{
    ID3v2_apic_frame_data* data = (ID3v2_apic_frame_data*) malloc(sizeof(ID3v2_apic_frame_data));

    const char encoding = string_has_bom(description) ? ID3v2_ENCODING_UNICODE : ID3v2_ENCODING_ISO;
    data->encoding = encoding;

    const int desc_size = ID3v2_strlent(description);
    data->description = (char*) malloc(desc_size * sizeof(char));
    memcpy(data->description, description, desc_size);

    const int mime_type_size = ID3v2_strlent(mime_type);
    data->mime_type = (char*) malloc(mime_type_size * sizeof(char));
    memcpy(data->mime_type, mime_type, mime_type_size);

    data->picture_type = picture_type;

    data->picture_size = picture_size;

    data->data = (char*) malloc(picture_size * sizeof(char));
    memcpy(data->data, picture_data, picture_size);

    return data;
}

ID3v2_apic_frame_data* apic_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    int cursor = 0;

    const int mime_type_size = ID3v2_strlent(buffer + (cursor += ID3v2_FRAME_ENCODING_LENGTH));
    const char* mime_type = (char*) malloc(mime_type_size * sizeof(char));
    memcpy(mime_type, buffer + cursor, mime_type_size);

    const char picture_type = buffer[cursor += mime_type_size];

    const int description_size = ID3v2_strlent(buffer + (cursor += ID3v2_APIC_FRAME_PICTURE_TYPE_LENGTH));
    const char* description = (char*) malloc(description_size * sizeof(char));
    memcpy(description, buffer + cursor, description_size);

    const int picture_size = frame_size - (cursor += description_size);
    const char* data = (char*) malloc(picture_size * sizeof(char));
    memcpy(data, buffer + cursor, picture_size);

    return apic_frame_data_new(description, picture_type, mime_type, picture_size, data);
}
