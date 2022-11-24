/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>

#include "id3v2lib.h"
#include "test_utils.h"
#include "assertion_utils.h"

void get_test_main()
{
    ID3v2_tag* tag = ID3v2_read_tag("./extra/file.mp3");

    ID3v2_text_frame* artist = ID3v2_tag_get_artist_frame(tag);
    assert_text_frame(artist, &(ID3v2_text_frame_input) {
        .id = ID3v2_ARTIST_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Ethereal Darkness"),
    });
    printf("ARTIST: ");
    print_text_frame(artist);

    ID3v2_text_frame* album = ID3v2_tag_get_album_frame(tag);
    assert_text_frame(album, &(ID3v2_text_frame_input) {
        .id = ID3v2_ALBUM_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Smoke And Shadows"),
    });
    printf("ALBUM: ");
    print_text_frame(album);

    ID3v2_text_frame* title = ID3v2_tag_get_title_frame(tag);
    assert_text_frame(title, &(ID3v2_text_frame_input) {
        .id = ID3v2_TITLE_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Rivers"),
    });
    printf("TITLE: ");
    print_text_frame(title);

    ID3v2_text_frame* track = ID3v2_tag_get_track_frame(tag);
    assert_text_frame(track, &(ID3v2_text_frame_input) {
        .id = ID3v2_TRACK_FRAME_ID,
        .flags = "\0\0",
        .text = "1",
    });
    printf("TRACK: ");
    print_text_frame(track);

    ID3v2_text_frame* album_artist = ID3v2_tag_get_album_artist_frame(tag);
    assert_text_frame(album_artist, &(ID3v2_text_frame_input) {
        .id = ID3v2_ALBUM_ARTIST_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Ethereal Darkness"),
    });
    printf("ALBUM ARTIST: ");
    print_text_frame(album_artist);

    ID3v2_text_frame* genre = ID3v2_tag_get_genre_frame(tag);
    assert_text_frame(genre, &(ID3v2_text_frame_input) {
        .id = ID3v2_GENRE_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Melodic Death Metal"),
    });
    printf("GENRE: ");
    print_text_frame(genre);

    ID3v2_text_frame* year = ID3v2_tag_get_year_frame(tag);
    assert_text_frame(year, &(ID3v2_text_frame_input) {
        .id = ID3v2_YEAR_FRAME_ID,
        .flags = "\0\0",
        .text = "2019",
    });
    printf("YEAR: ");
    print_text_frame(year);

    ID3v2_text_frame* disc_number = ID3v2_tag_get_disc_number_frame(tag);
    assert_text_frame(disc_number, &(ID3v2_text_frame_input) {
        .id = ID3v2_DISC_NUMBER_FRAME_ID,
        .flags = "\0\0",
        .text = "1",
    });
    printf("DISC NUMBER: ");
    print_text_frame(disc_number);

    ID3v2_text_frame* composer = ID3v2_tag_get_composer_frame(tag);
    assert_text_frame(composer, &(ID3v2_text_frame_input) {
        .id = ID3v2_COMPOSER_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Composer"),
    });
    printf("COMPOSER: ");
    print_text_frame(composer);

    ID3v2_comment_frame* comment = ID3v2_tag_get_comment_frame(tag);
    assert_comment_frame(comment, &(ID3v2_comment_frame_input) {
        .flags = "\0\0",
        .comment = ID3v2_to_unicode("This is a comment"),
        .language = "eng",
        .short_description = ID3v2_to_unicode(""),
    });
    printf("COMMENT: ");
    print_comment_frame(comment);

    ID3v2_frame_list* comments = ID3v2_tag_get_comment_frames(tag);
    assert_comment_frame((ID3v2_comment_frame*) comments->frame, &(ID3v2_comment_frame_input) {
        .flags = "\0\0",
        .comment = ID3v2_to_unicode("This is a comment"),
        .language = "eng",
        .short_description = ID3v2_to_unicode(""),
    });
    printf("COMMENTS:\n");
    print_comment_frames(comments);

    ID3v2_apic_frame* album_cover = ID3v2_tag_get_album_cover_frame(tag);
    assert_apic_frame(album_cover, (Apic_frame_assertion) {
        .flags = "\0\0",
        .encoding = 0x00,
        .mime_type = ID3v2_MIME_TYPE_PNG,
        .description = "",
        .picture_type = 0x03,
        .album_cover_file_path = "extra/album_cover.png",
    });
    printf("ALBUM COVER: ");
    save_apic_frame(album_cover, "./extra");

    ID3v2_tag_free(tag);
}
