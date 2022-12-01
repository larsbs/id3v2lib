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

#include "id3v2lib.compat.h"

#include "compat_test.h"

void compat_test_main()
{
    ID3v2_tag* tag = load_tag("extra/file.mp3");

    if (tag == NULL)
    {
        tag = new_tag();
    }

    // Load the fields from the tag
    ID3v2_frame* artist_frame = tag_get_artist(tag); // Get the full artist frame
    // We need to parse the frame content to make readable
    ID3v2_frame_text_content* artist_content = parse_text_frame_content(artist_frame);
    printf("ARTIST: %s\n", artist_content->data); // Show the artist info

    ID3v2_frame* title_frame = tag_get_title(tag);
    ID3v2_frame_text_content* title_content = parse_text_frame_content(title_frame);
    printf("TITLE: %s\n", title_content->data);
}
