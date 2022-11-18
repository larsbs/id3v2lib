#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "id3v2lib.h"
#include "utils.h"

typedef struct _Text_frame_assertions
{
    char id[ID3v2_FRAME_HEADER_ID_LENGTH];
    int frame_size;
    char flags[ID3v2_FRAME_HEADER_FLAGS_LENGTH];
    int data_size;
    char encoding;
    char* text;
} Text_frame_assertions;

void assert_text_frame(ID3v2_text_frame* frame, Text_frame_assertions compareTo)
{
    // Header
    assert(memcmp(frame->header->id, compareTo.id, ID3v2_FRAME_HEADER_ID_LENGTH) == 0);
    assert(frame->header->size == compareTo.frame_size);
    assert(memcmp(frame->header->flags, compareTo.flags, ID3v2_FRAME_HEADER_FLAGS_LENGTH) == 0);
    // Data
    assert(frame->data->size == compareTo.data_size);
    assert(frame->data->encoding == compareTo.encoding);
    assert(memcmp(frame->data->text, char_to_utf16(compareTo.text, -1), frame->data->size));
}

int main(int argc, char* argv[])
{
    ID3v2_tag* tag = ID3v2_read_tag("./extra/file.mp3");

    ID3v2_text_frame* artist = ID3v2_tag_get_artist_frame(tag);
    assert_text_frame(artist, (Text_frame_assertions) {
        .id = ARTIST_FRAME_ID,
        .frame_size = 39,
        .flags = "\0\0",
        .data_size = 38,
        .encoding = 0x01,
        .text = "Ethereal Darkness",
    });
    printf("ARTIST: ");
    print_text_frame(artist);

    ID3v2_text_frame* album = ID3v2_tag_get_album_frame(tag);
    printf("ALBUM: ");
    print_text_frame(album);

    ID3v2_text_frame* title = ID3v2_tag_get_title_frame(tag);
    printf("TITLE: ");
    print_text_frame(title);

    ID3v2_text_frame* track = ID3v2_tag_get_track_frame(tag);
    printf("TRACK: ");
    print_text_frame(track);

    ID3v2_text_frame* album_artist = ID3v2_tag_get_album_artist_frame(tag);
    printf("ALBUM ARTIST: ");
    print_text_frame(album_artist);

    ID3v2_text_frame* genre = ID3v2_tag_get_genre_frame(tag);
    printf("GENRE: ");
    print_text_frame(genre);

    ID3v2_text_frame* year = ID3v2_tag_get_year_frame(tag);
    printf("YEAR: ");
    print_text_frame(year);

    ID3v2_text_frame* disc_number = ID3v2_tag_get_disc_number_frame(tag);
    printf("DISC NUMBER: ");
    print_text_frame(disc_number);

    ID3v2_text_frame* composer = ID3v2_tag_get_composer_frame(tag);
    printf("COMPOSER: ");
    print_text_frame(composer);

    ID3v2_comment_frame* comment = ID3v2_tag_get_comment_frame(tag);
    printf("COMMENT: ");
    print_comment_frame(comment);

    ID3v2_frame_list* comments = ID3v2_tag_get_comment_frames(tag);
    printf("COMMENTS:\n");
    print_comment_frames(comments);

    ID3v2_apic_frame* album_cover = ID3v2_tag_get_album_cover_frame(tag);
    printf("ALBUM COVER: ");
    save_apic_frame(album_cover, "./extra");

    return 0;
}
