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

void edit_test()
{
    // clone the file
    ID3v2_tag* tag = ID3v2_read_tag("extra/file.mp3");

    if (tag == NULL)
    {
        tag = ID3v2_tag_new();
    }

    ID3v2_tag_set_artist(tag, ID3v2_to_unicode("Artist"));
    assert_text_frame(ID3v2_tag_get_artist_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_ARTIST_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Artist"),
    });

    ID3v2_tag_set_album(tag, ID3v2_to_unicode("Album"));
    assert_text_frame(ID3v2_tag_get_album_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_ALBUM_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Album"),
    });

    ID3v2_tag_set_title(tag, ID3v2_to_unicode("Title"));
    assert_text_frame(ID3v2_tag_get_title_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_TITLE_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Title"),
    });

    ID3v2_tag_set_track(tag, "00");
    assert_text_frame(ID3v2_tag_get_track_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_TRACK_FRAME_ID,
        .flags = "\0\0",
        .text = "00",
    });

    ID3v2_tag_set_album_artist(tag, ID3v2_to_unicode("Album Artist"));
    assert_text_frame(ID3v2_tag_get_album_artist_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_ALBUM_ARTIST_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Album Artist"),
    });

    ID3v2_tag_set_genre(tag, ID3v2_to_unicode("Genre"));
    assert_text_frame(ID3v2_tag_get_genre_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_GENRE_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Genre"),
    });

    ID3v2_tag_set_year(tag, "0000");
    assert_text_frame(ID3v2_tag_get_year_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_YEAR_FRAME_ID,
        .flags = "\0\0",
        .text = "0000",
    });

    ID3v2_tag_set_disc_number(tag, "00");
    assert_text_frame(ID3v2_tag_get_disc_number_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_DISC_NUMBER_FRAME_ID,
        .flags = "\0\0",
        .text = "00",
    });

    ID3v2_tag_set_composer(tag, ID3v2_to_unicode("Writter"));
    assert_text_frame(ID3v2_tag_get_composer_frame(tag), &(ID3v2_text_frame_input) {
        .id = ID3v2_COMPOSER_FRAME_ID,
        .flags = "\0\0",
        .text = ID3v2_to_unicode("Writter"),
    });

    ID3v2_tag_set_comment(tag, "eng", ID3v2_to_unicode("Comment"));
    assert_comment_frame(ID3v2_tag_get_comment_frame(tag), &(ID3v2_comment_frame_input) {
        .flags = "\0\0",
        .language = "eng",
        .short_description = ID3v2_to_unicode(""),
        .comment = ID3v2_to_unicode("Comment"),
    });

    ID3v2_tag_add_comment_frame(tag, &(ID3v2_comment_frame_input) {
        .flags = "\0\0",
        .language = "eng",
        .short_description = ID3v2_to_unicode("Short Description"),
        .comment = ID3v2_to_unicode("Comment"),
    });
    assert_comment_frame(ID3v2_tag_get_comment_frames(tag)->next->frame, &(ID3v2_comment_frame_input) {
        .flags = "\0\0",
        .language = "eng",
        .short_description = ID3v2_to_unicode("Short Description"),
        .comment = ID3v2_to_unicode("Comment"),
    });

    // FILE* album_cover = fopen("extra/album_cover.png", "rb");
    // ID3v2_tag_add_apic_frame(tag, (ID3v2_apic_frame_input) {
    //     .flags = "\0\0",
    //     .mime_type = ID3v2_MIME_TYPE_PNG,
    //     .description = "Description",
    //     .picture_type = ID3v2_PIC_TYPE_FRONT_COVER,
    //     .data = picture_data
    // })
    // ID3v2_tag_set_album_cover(tag, picture_data, mime_type, picture_size);

    // ID3v2_tag_write("extra/file_cloned.mp3");

    // Verify the written data

    // delete cloned file
}

void new_tag_test()
{

}

void set_test_main()
{
    edit_test();
    new_tag_test();
}
