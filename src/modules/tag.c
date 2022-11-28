/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frame.private.h"
#include "frame_list.private.h"
#include "modules/frame_ids.h"
#include "modules/picture_types.h"
#include "tag.private.h"
#include "utils.private.h"

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

    if (!has_id3v2_tag)
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
    tag_header->tag_size = syncint_decode(
        btoi(buffer + (cursor += ID3v2_TAG_HEADER_FLAGS_LENGTH), ID3v2_TAG_HEADER_TAG_SIZE_LENGTH)
    );
    tag_header->extended_header_size = 0;

    if ((tag_header->flags & (1 << 6)) == (1 << 6))
    {
        // An extended header exists, retrieve its size and update the tag_header
        tag_header->extended_header_size = syncint_decode(btoi(
            buffer + (cursor += ID3v2_TAG_HEADER_TAG_SIZE_LENGTH),
            ID3v2_EXTENDED_HEADED_SIZE_LENGTH
        ));
    }

    return tag_header;
}

Char_stream* tag_to_char_stream(ID3v2_tag* tag)
{
    Char_stream* tag_cs = char_stream_new(tag->header->tag_size + ID3v2_TAG_HEADER_LENGTH);

    // Write header
    cswrite(tag->header->identifier, ID3v2_TAG_HEADER_IDENTIFIER_LENGTH, tag_cs);
    cswrite(&tag->header->major_version, ID3v2_TAG_HEADER_MAJOR_VERSION_LENGTH, tag_cs);
    cswrite(&tag->header->minor_version, ID3v2_TAG_HEADER_MINOR_VERSION_LENGTH, tag_cs);
    cswrite(&tag->header->flags, ID3v2_TAG_HEADER_FLAGS_LENGTH, tag_cs);
    cswrite(itob(syncint_encode(tag->header->tag_size)), ID3v2_TAG_HEADER_TAG_SIZE_LENGTH, tag_cs);

    // Write frames
    ID3v2_frame_list* frames = tag->frames;
    while (frames != NULL)
    {
        Char_stream* frame_cs = frame_to_char_stream(frames->frame);

        if (frame_cs == NULL)
        {
            exit(1);
        }

        cswrite(frame_cs->stream, frame_cs->size, tag_cs);
        char_stream_free(frame_cs);

        frames = frames->next;
    }

    return tag_cs;
}

ID3v2_tag* ID3v2_tag_new()
{
    ID3v2_tag* tag = (ID3v2_tag*) malloc(sizeof(ID3v2_tag));
    tag->header = tag_header_new();
    tag->frames = frame_list_new();
    tag->padding_size = 0;

    return tag;
}

void ID3v2_tag_free(ID3v2_tag* tag)
{
    free(tag->header);
    ID3v2_frame_list_free(tag->frames);
    free(tag);
}

void ID3v2_tag_write(ID3v2_tag* tag, const char* dest)
{
    if (tag == NULL)
    {
        return;
    }

    ID3v2_tag_header* existing_tag_header = tag_header_load(dest);
    const int original_size =
        existing_tag_header != NULL ? existing_tag_header->tag_size + ID3v2_TAG_HEADER_LENGTH : 0;
    free(existing_tag_header);

    const int extra_padding_length = clamp_int(
        ID3v2_TAG_DEFAULT_PADDING_LENGTH - tag->padding_size,
        0,
        ID3v2_TAG_DEFAULT_PADDING_LENGTH
    );
    Char_stream* tag_cs = tag_to_char_stream(tag + extra_padding_length);

    // Perform operations on a temp file in case things go wrong
    FILE* temp_fp = tmpfile();

    // First write the tag to the temp file
    fwrite(tag_cs->stream, sizeof(char), tag_cs->size, temp_fp);

    // And then, read the original audio data and copy it to
    // the temp file so it's located after the tag
    FILE* dest_fp = fopen(dest, "r+b");
    fseek(dest_fp, original_size, SEEK_SET); // move to the end of the original tag

    int c = 0;

    while ((c = getc(dest_fp)) != EOF)
    {
        putc(c, temp_fp);
    }

    // Finally copy the temp file back into the destination file
    fclose(dest_fp);
    dest_fp = fopen(dest, "w+b");
    fseek(temp_fp, 0L, SEEK_SET);

    while ((c = getc(temp_fp)) != EOF)
    {
        putc(c, dest_fp);
    }

    fclose(temp_fp);
    fclose(dest_fp);

    char_stream_free(tag_cs);
}

/**
 * Getter functions
 */
ID3v2_frame* ID3v2_tag_get_frame(ID3v2_tag* tag, char* frame_id)
{
    if (tag == NULL)
    {
        return NULL;
    }

    return frame_list_get_frame_by_id(tag->frames, frame_id);
}

ID3v2_frame_list* ID3v2_tag_get_frames(ID3v2_tag* tag, char* frame_id)
{
    if (tag == NULL)
    {
        return NULL;
    }

    return frame_list_get_frames_by_id(tag->frames, frame_id);
}

ID3v2_text_frame* ID3v2_tag_get_artist_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_ARTIST_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_album_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_ALBUM_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_title_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_TITLE_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_track_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_TRACK_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_album_artist_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_ALBUM_ARTIST_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_genre_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_GENRE_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_year_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_YEAR_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_disc_number_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_DISC_NUMBER_FRAME_ID);
}

ID3v2_text_frame* ID3v2_tag_get_composer_frame(ID3v2_tag* tag)
{
    return (ID3v2_text_frame*) ID3v2_tag_get_frame(tag, ID3v2_COMPOSER_FRAME_ID);
}

/**
 * Since the ID3v2 spec allows defining multiple COMM frames in a
 * single tag, this gets the first COMM frame found. To retrieve every
 * COMM frame found use ID3v2_tag_get_comment_frames(tag) instead.
 */
ID3v2_comment_frame* ID3v2_tag_get_comment_frame(ID3v2_tag* tag)
{
    return (ID3v2_comment_frame*) ID3v2_tag_get_frame(tag, ID3v2_COMMENT_FRAME_ID);
}

/**
 * Since the ID3v2 spec allows defining multiple COMM frames in a
 * single tag, this gets every COMM frame found. To retrieve only the
 * first COMM frame found use ID3v2_tag_get_comment_frame(tag) instead.
 */
ID3v2_frame_list* ID3v2_tag_get_comment_frames(ID3v2_tag* tag)
{
    return ID3v2_tag_get_frames(tag, ID3v2_COMMENT_FRAME_ID);
}

ID3v2_apic_frame* ID3v2_tag_get_album_cover_frame(ID3v2_tag* tag)
{
    return (ID3v2_apic_frame*) ID3v2_tag_get_frame(tag, ID3v2_ALBUM_COVER_FRAME_ID);
}

ID3v2_frame_list* ID3v2_tag_get_apic_frames(ID3v2_tag* tag)
{
    return ID3v2_tag_get_frames(tag, ID3v2_ALBUM_COVER_FRAME_ID);
}

/**
 * Setter functions
 */
void ID3v2_tag_set_text_frame(ID3v2_tag* tag, ID3v2_text_frame_input* input)
{
    ID3v2_text_frame* new_frame = text_frame_new(input->id, input->flags, input->text);
    ID3v2_text_frame* existing_frame =
        (ID3v2_text_frame*) frame_list_get_frame_by_id(tag->frames, input->id);

    if (existing_frame == NULL)
    {
        frame_list_add_frame(tag->frames, (ID3v2_frame*) new_frame);
        tag->header->tag_size += new_frame->header->size;
    }
    else
    {
        frame_list_replace_frame(
            tag->frames,
            (ID3v2_frame*) existing_frame,
            (ID3v2_frame*) new_frame
        );
        tag->header->tag_size += (new_frame->header->size - existing_frame->header->size);
        frame_free((ID3v2_frame*) existing_frame);
    }
}

void ID3v2_tag_set_artist(ID3v2_tag* tag, const char* artist)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_ARTIST_FRAME_ID,
            .flags = "\0\0",
            .text = artist,
        }
    );
}

void ID3v2_tag_set_album(ID3v2_tag* tag, const char* album)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_ALBUM_FRAME_ID,
            .flags = "\0\0",
            .text = album,
        }
    );
}

void ID3v2_tag_set_title(ID3v2_tag* tag, const char* title)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_TITLE_FRAME_ID,
            .flags = "\0\0",
            .text = title,
        }
    );
}

void ID3v2_tag_set_track(ID3v2_tag* tag, const char* track)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_TRACK_FRAME_ID,
            .flags = "\0\0",
            .text = track,
        }
    );
}

void ID3v2_tag_set_album_artist(ID3v2_tag* tag, const char* album_artist)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_ALBUM_ARTIST_FRAME_ID,
            .flags = "\0\0",
            .text = album_artist,
        }
    );
}

void ID3v2_tag_set_genre(ID3v2_tag* tag, const char* genre)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_GENRE_FRAME_ID,
            .flags = "\0\0",
            .text = genre,
        }
    );
}

void ID3v2_tag_set_year(ID3v2_tag* tag, const char* year)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_YEAR_FRAME_ID,
            .flags = "\0\0",
            .text = year,
        }
    );
}

void ID3v2_tag_set_disc_number(ID3v2_tag* tag, const char* disc_number)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_DISC_NUMBER_FRAME_ID,
            .flags = "\0\0",
            .text = disc_number,
        }
    );
}

void ID3v2_tag_set_composer(ID3v2_tag* tag, const char* composer)
{
    ID3v2_tag_set_text_frame(
        tag,
        &(ID3v2_text_frame_input){
            .id = ID3v2_COMPOSER_FRAME_ID,
            .flags = "\0\0",
            .text = composer,
        }
    );
}

/**
 * This only sets the first COMM frame found.
 */
void ID3v2_tag_set_comment_frame(ID3v2_tag* tag, ID3v2_comment_frame_input* input)
{
    ID3v2_comment_frame* new_frame =
        comment_frame_new(input->flags, input->language, input->short_description, input->comment);
    ID3v2_comment_frame* existing_frame = ID3v2_tag_get_comment_frame(tag);

    if (existing_frame == NULL)
    {
        frame_list_add_frame(tag->frames, (ID3v2_frame*) new_frame);
        tag->header->tag_size += new_frame->header->size;
    }
    else
    {
        frame_list_replace_frame(
            tag->frames,
            (ID3v2_frame*) existing_frame,
            (ID3v2_frame*) new_frame
        );
        tag->header->tag_size += (new_frame->header->size - existing_frame->header->size);
        frame_free((ID3v2_frame*) existing_frame);
    }
}

void ID3v2_tag_add_comment_frame(ID3v2_tag* tag, ID3v2_comment_frame_input* input)
{
    ID3v2_comment_frame* new_frame =
        comment_frame_new(input->flags, input->language, input->short_description, input->comment);
    frame_list_add_frame(tag->frames, (ID3v2_frame*) new_frame);
    tag->header->tag_size += new_frame->header->size;
}

void ID3v2_tag_set_comment(ID3v2_tag* tag, const char* lang, const char* comment)
{
    ID3v2_tag_set_comment_frame(
        tag,
        &(ID3v2_comment_frame_input){
            .flags = "\0\0",
            .language = lang,
            .short_description = ID3v2_to_unicode(""),
            .comment = comment,
        }
    );
}

/**
 * This only sets the first APIC frame found.
 */
void ID3v2_tag_set_apic_frame(ID3v2_tag* tag, ID3v2_apic_frame_input* input)
{
    ID3v2_apic_frame* new_frame = apic_frame_new(
        input->flags,
        input->description,
        input->picture_type,
        input->mime_type,
        input->picture_size,
        input->data
    );
    ID3v2_apic_frame* existing_frame = ID3v2_tag_get_album_cover_frame(tag);

    if (existing_frame == NULL)
    {
        frame_list_add_frame(tag->frames, (ID3v2_frame*) new_frame);
        tag->header->tag_size += new_frame->header->size;
    }
    else
    {
        frame_list_replace_frame(
            tag->frames,
            (ID3v2_frame*) existing_frame,
            (ID3v2_frame*) new_frame
        );
        tag->header->tag_size += (new_frame->header->size - existing_frame->header->size);
        frame_free((ID3v2_frame*) existing_frame);
    }
}

void ID3v2_tag_add_apic_frame(ID3v2_tag* tag, ID3v2_apic_frame_input* input)
{
    ID3v2_apic_frame* new_frame = apic_frame_new(
        input->flags,
        input->description,
        input->picture_type,
        input->mime_type,
        input->picture_size,
        input->data
    );
    frame_list_add_frame(tag->frames, (ID3v2_frame*) new_frame);
    tag->header->tag_size += new_frame->header->size;
}

/**
 * This only sets the front album cover (picture_type = 0x03)
 */
void ID3v2_tag_set_album_cover(
    ID3v2_tag* tag,
    const char* mime_type,
    const int size,
    const char* data
)
{
    ID3v2_tag_set_apic_frame(
        tag,
        &(ID3v2_apic_frame_input){
            .flags = "\0\0",
            .mime_type = mime_type,
            .description = ID3v2_to_unicode(""),
            .picture_size = size,
            .picture_type = ID3v2_PIC_TYPE_FRONT_COVER,
            .data = data,
        }
    );
}
