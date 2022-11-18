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

ID3v2_frame* frame_parse(const char* buffer, int id3_major_version)
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
    char* raw_data = (char*) malloc(frame_header->size * sizeof(char));
    memcpy(raw_data, buffer + (cursor += ID3v2_FRAME_HEADER_FLAGS_LENGTH), frame_header->size);

    ID3v2_frame* frame = frame_new();
    frame->header = frame_header;

    if (frame->header->id[0] == 'T')
    {
        // It's a text information frame
        frame->data = text_frame_data_parse(raw_data, frame->header->size);
    }
    else if (frame->header->id[0] == 'C')
    {
        // It's a comment information frame
        frame->data = comment_frame_data_parse(raw_data, frame->header->size);
    }
    else if (frame->header->id[0] == 'A')
    {
        // It's an attached picture frame
        frame->data = apic_frame_data_parse(raw_data, frame->header->size);
    }
    else
    {
        // Unknown frame type, simply copy the raw data into the data property
        frame->data = (char*) malloc(frame_header->size * sizeof(char));
        memcpy(frame->data, raw_data, frame_header->size);
    }

    free(raw_data);

    return frame;
}

ID3v2_text_frame_data* text_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    ID3v2_text_frame_data* text_data = (ID3v2_text_frame_data*) malloc(sizeof(ID3v2_text_frame_data));
    text_data->encoding = buffer[0];
    text_data->size = frame_size - ID3v2_FRAME_ENCODING_LENGTH;
    text_data->text = (char*) malloc(text_data->size * sizeof(char));
    memcpy(text_data->text, buffer + ID3v2_FRAME_ENCODING_LENGTH, text_data->size);

    return text_data;
}

ID3v2_comment_frame_data* comment_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    ID3v2_comment_frame_data* comment_data = (ID3v2_comment_frame_data*) malloc(sizeof(ID3v2_comment_frame_data));
    comment_data->encoding = buffer[0];
    comment_data->size = frame_size - ID3v2_FRAME_ENCODING_LENGTH - ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH - ID3v2_COMMENT_FRAME_SHORT_DESCRIPTION_LENGTH;
    comment_data->language = (char*) malloc(ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH * sizeof(char));
    memcpy(comment_data->language, buffer + ID3v2_FRAME_ENCODING_LENGTH, ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH);
    comment_data->short_description = comment_data->encoding == ISO_ENCODING ? "\0" : "\0\0"; // Ignore short description for now.
    int short_description_offset = ISO_ENCODING ? 1 : 2;
    comment_data->comment = (char*) malloc(comment_data->size * sizeof(char));
    memcpy(comment_data->comment, buffer + ID3v2_FRAME_ENCODING_LENGTH + ID3v2_COMMENT_FRAME_LANGUAGE_LENGTH + short_description_offset, comment_data->size);

    return comment_data;
}

ID3v2_apic_frame_data* apic_frame_data_parse(const char* buffer, int frame_size)
{
    if (buffer == NULL)
    {
        return NULL;
    }

    int cursor = 0;
    ID3v2_apic_frame_data* apic_data = (ID3v2_apic_frame_data*) malloc(sizeof(ID3v2_apic_frame_data));

    apic_data->encoding = buffer[cursor];

    int mime_type_size = strlen(buffer + (cursor += ID3v2_FRAME_ENCODING_LENGTH)) + 1; // Add one to take into account the termination character
    apic_data->mime_type = (char*) malloc(mime_type_size * sizeof(char));
    memcpy(apic_data->mime_type, buffer + cursor, mime_type_size);

    apic_data->picture_type = buffer[cursor += mime_type_size];
    cursor += ID3v2_APIC_FRAME_PICTURE_TYPE_LENGTH;

    apic_data->description = apic_data->encoding == ISO_ENCODING ? "\0" : "\0\0";

    // Move forward cursor to skip description.
    if (apic_data->encoding == ISO_ENCODING)
    {
        cursor += strlen(buffer + cursor);
        cursor++; // Add termination string character
    }
    else
    {
        char prev = buffer[cursor++];
        while (1)
        {
            char curr = buffer[cursor++];

            if (prev == 0x00 && curr == 0x00)
            {
                cursor++;
                break;
            }

            prev = curr;
        }
    }

    apic_data->picture_size = frame_size - cursor;
    apic_data->data = (char*) malloc(apic_data->picture_size * sizeof(char));
    memcpy(apic_data->data, buffer + cursor, apic_data->picture_size);

    return apic_data;
}
