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

#include "modules/char_stream.private.h"
#include "modules/frame.private.h"
#include "modules/frame_header.private.h"
#include "modules/frame_list.private.h"
#include "modules/tag.private.h"
#include "modules/tag_header.private.h"

#include "id3v2lib.compat.h"

ID3v2_frame_list* new_frame_list()
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

ID3v2_frame* frame_to_compat_frame(ID3v2_Frame* frame)
{
    ID3v2_frame* compat_frame = (ID3v2_frame*) malloc(sizeof(ID3v2_frame));

    // Frame header
    memcpy(compat_frame->frame_id, frame->header->id, ID3v2_FRAME_HEADER_ID_LENGTH);
    compat_frame->size = frame->header->size;
    memcpy(compat_frame->flags, frame->header->flags, ID3v2_TAG_HEADER_FLAGS_LENGTH);

    // Frame data
    CharStream* frame_cs = Frame_to_char_stream(frame);
    CharStream_seek(frame_cs, ID3v2_FRAME_HEADER_LENGTH, SEEK_SET);
    compat_frame->data = CharStream_get_cur(frame_cs);

    compat_frame->frame = frame;

    return compat_frame;
}

void add_to_list(ID3v2_frame_list* main, ID3v2_frame* frame)
{
    ID3v2_frame_list* current;

    // if empty list
    if (main->start == NULL)
    {
        main->start = main;
        main->last = main;
        main->frame = frame;
    }
    else
    {
        current = new_frame_list();
        current->frame = frame;
        current->start = main->start;
        main->last->next = current;
        main->last = current;
    }
}

ID3v2_tag* new_tag()
{
    ID3v2_tag* tag = (ID3v2_tag*) malloc(sizeof(ID3v2_tag));
    ID3v2_header* header = (ID3v2_header*) malloc(sizeof(ID3v2_header));

    memcpy(header->tag, "ID3", ID3v2_TAG_HEADER_IDENTIFIER_LENGTH);

    tag->tag_header = header;
    tag->frames = new_frame_list();
    tag->tag = ID3v2_Tag_new_empty();

    return tag;
}

ID3v2_tag* load_tag(const char* file_name)
{
    ID3v2_TagHeader* header = ID3v2_read_tag_header(file_name);
    FILE* fp = fopen(file_name, "rb");
    const int buffer_size = header->tag_size + ID3v2_TAG_HEADER_LENGTH;
    const char* buffer = (char*) malloc(buffer_size * sizeof(char));
    fread(buffer, sizeof(char), buffer_size, fp);

    return load_tag_with_buffer(buffer, buffer_size);
}

ID3v2_tag* load_tag_with_buffer(const char* buffer, int length)
{
    ID3v2_Tag* tag = ID3v2_read_tag_from_buffer(buffer, length);
    ID3v2_tag* compat_tag = (ID3v2_tag*) malloc(sizeof(ID3v2_tag));

    compat_tag->tag_header = (ID3v2_header*) malloc(sizeof(ID3v2_header));
    compat_tag->tag = tag;
    compat_tag->raw = buffer;

    // Header
    memcpy(
        compat_tag->tag_header->tag,
        tag->header->identifier,
        ID3v2_TAG_HEADER_IDENTIFIER_LENGTH
    );
    compat_tag->tag_header->major_version = tag->header->major_version;
    compat_tag->tag_header->minor_version = tag->header->minor_version;
    compat_tag->tag_header->flags = tag->header->flags;
    compat_tag->tag_header->tag_size = tag->header->tag_size;
    compat_tag->tag_header->extended_header_size = tag->header->extended_header_size;

    // Frames
    ID3v2_FrameList* head = tag->frames;
    ID3v2_frame_list* compat_frames = new_frame_list();

    while (head != NULL)
    {
        ID3v2_frame* compat_frame = frame_to_compat_frame(head->frame);
        add_to_list(compat_frames, compat_frame);
        head = head->next;
    }

    compat_tag->frames = compat_frames;

    return compat_tag;
}

void remove_tag(const char* file_name)
{
    ID3v2_delete_tag(file_name);
}

void set_tag(const char* file_name, ID3v2_tag* tag)
{
    ID3v2_write_tag(file_name, tag->tag);
}

void free_tag(ID3v2_tag* tag)
{
    ID3v2_frame_list* list;

    free(tag->raw);
    free(tag->tag_header);
    ID3v2_Tag_free(tag->tag);

    list = tag->frames;

    while (list != NULL)
    {
        if (list->frame)
        {
            free(list->frame->data);
            ID3v2_Frame_free(list->frame->frame);
            free(list->frame);
        }

        list = list->next;
    }

    free(list);
    free(tag);
}

ID3v2_frame* tag_get_title(ID3v2_tag* tag)
{
    ID3v2_Frame* title_frame = ID3v2_Tag_get_title_frame(tag->tag);
    return frame_to_compat_frame(title_frame);
}

ID3v2_frame* tag_get_artist(ID3v2_tag* tag)
{
    ID3v2_Frame* artist_frame = ID3v2_Tag_get_artist_frame(tag->tag);
    return frame_to_compat_frame(artist_frame);
}

ID3v2_frame* tag_get_album(ID3v2_tag* tag)
{
    ID3v2_Frame* album_frame = ID3v2_Tag_get_album_frame(tag->tag);
    return frame_to_compat_frame(album_frame);
}

ID3v2_frame* tag_get_album_artist(ID3v2_tag* tag)
{
    ID3v2_Frame* album_artist_frame = ID3v2_Tag_get_album_artist_frame(tag->tag);
    return frame_to_compat_frame(album_artist_frame);
}

ID3v2_frame* tag_get_genre(ID3v2_tag* tag)
{
    ID3v2_Frame* genre_frame = ID3v2_Tag_get_genre_frame(tag->tag);
    return frame_to_compat_frame(genre_frame);
}

ID3v2_frame* tag_get_track(ID3v2_tag* tag)
{
    ID3v2_Frame* track_frame = ID3v2_Tag_get_track_frame(tag->tag);
    return frame_to_compat_frame(track_frame);
}

ID3v2_frame* tag_get_year(ID3v2_tag* tag)
{
    ID3v2_Frame* year_frame = ID3v2_Tag_get_year_frame(tag->tag);
    return frame_to_compat_frame(year_frame);
}

ID3v2_frame* tag_get_comment(ID3v2_tag* tag)
{
    ID3v2_Frame* comment_frame = ID3v2_Tag_get_comment_frame(tag->tag);
    return frame_to_compat_frame(comment_frame);
}

ID3v2_frame* tag_get_disc_number(ID3v2_tag* tag)
{
    ID3v2_Frame* disc_number_frame = ID3v2_Tag_get_disc_number_frame(tag->tag);
    return frame_to_compat_frame(disc_number_frame);
}

ID3v2_frame* tag_get_composer(ID3v2_tag* tag)
{
    ID3v2_Frame* composer_frame = ID3v2_Tag_get_composer_frame(tag->tag);
    return frame_to_compat_frame(composer_frame);
}

ID3v2_frame* tag_get_album_cover(ID3v2_tag* tag)
{
    ID3v2_Frame* album_cover_frame = ID3v2_Tag_get_album_cover_frame(tag->tag);
    return frame_to_compat_frame(album_cover_frame);
}

ID3v2_frame_text_content* parse_text_frame_content(ID3v2_frame* frame)
{
    ID3v2_frame_text_content* content =
        (ID3v2_frame_text_content*) malloc(sizeof(ID3v2_frame_text_content));
    ID3v2_TextFrame* text_frame = (ID3v2_TextFrame*) frame->frame;

    content->encoding = text_frame->data->encoding;
    content->size = text_frame->data->size;
    content->data = text_frame->data->text;

    return content;
}

ID3v2_frame_comment_content* parse_comment_frame_content(ID3v2_frame* frame)
{
    ID3v2_frame_comment_content* content =
        (ID3v2_frame_comment_content*) malloc(sizeof(ID3v2_frame_comment_content));
    ID3v2_CommentFrame* comment_frame = (ID3v2_CommentFrame*) frame->frame;

    content->language = comment_frame->data->language;
    content->short_description = comment_frame->data->short_description;
    content->text = (ID3v2_frame_text_content*) malloc(sizeof(ID3v2_frame_comment_content));
    content->text->encoding = comment_frame->data->encoding;
    content->text->size = comment_frame->data->size;
    content->text->data = comment_frame->data->comment;

    return content;
}

ID3v2_frame_apic_content* parse_apic_frame_content(ID3v2_frame* frame)
{
    ID3v2_frame_apic_content* content =
        (ID3v2_frame_apic_content*) malloc(sizeof(ID3v2_frame_apic_content));
    ID3v2_ApicFrame* apic_frame = (ID3v2_ApicFrame*) frame->frame;

    content->encoding = apic_frame->data->encoding;
    content->mime_type = apic_frame->data->mime_type;
    content->picture_type = apic_frame->data->picture_type;
    content->description = apic_frame->data->description;
    content->picture_size = apic_frame->data->picture_size;
    content->data = apic_frame->data->data;

    return content;
}

void tag_set_title(char* title, char encoding, ID3v2_tag* tag)
{
}

void tag_set_artist(char* artist, char encoding, ID3v2_tag* tag)
{
}

void tag_set_album(char* album, char encoding, ID3v2_tag* tag)
{
}

void tag_set_album_artist(char* album_artist, char encoding, ID3v2_tag* tag)
{
}

void tag_set_genre(char* genre, char encoding, ID3v2_tag* tag)
{
}

void tag_set_track(char* track, char encoding, ID3v2_tag* tag)
{
}

void tag_set_year(char* year, char encoding, ID3v2_tag* tag)
{
}

void tag_set_comment(char* comment, char encoding, ID3v2_tag* tag)
{
}

void tag_set_disc_number(char* disc_number, char encoding, ID3v2_tag* tag)
{
}

void tag_set_composer(char* composer, char encoding, ID3v2_tag* tag)
{
}

void tag_set_album_cover(const char* filename, ID3v2_tag* tag)
{
}

void tag_set_album_cover_from_bytes(
    char* album_cover_bytes,
    char* mimetype,
    int picture_size,
    ID3v2_tag* tag
)
{
}
