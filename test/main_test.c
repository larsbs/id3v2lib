#include <stdio.h>

#include "id3v2lib.h"
#include "utils.h"

int main(int argc, char* argv[])
{
    ID3v2_tag* tag = ID3v2_read_tag("./extra/file.mp3");

    ID3v2_text_frame* artist = ID3v2_tag_get_artist(tag);
    printf("ARTIST: ");
    print_text_frame(artist);

    ID3v2_text_frame* album = ID3v2_tag_get_album(tag);
    printf("ALBUM: ");
    print_text_frame(album);

    ID3v2_text_frame* title = ID3v2_tag_get_title(tag);
    printf("TITLE: ");
    print_text_frame(title);

    ID3v2_text_frame* track = ID3v2_tag_get_track(tag);
    printf("TRACK: ");
    print_text_frame(track);

    ID3v2_text_frame* album_artist = ID3v2_tag_get_album_artist(tag);
    printf("ALBUM ARTIST: ");
    print_text_frame(album_artist);

    ID3v2_text_frame* genre = ID3v2_tag_get_genre(tag);
    printf("GENRE: ");
    print_text_frame(genre);

    ID3v2_text_frame* year = ID3v2_tag_get_year(tag);
    printf("YEAR: ");
    print_text_frame(year);

    ID3v2_text_frame* disc_number = ID3v2_tag_get_disc_number(tag);
    printf("DISC NUMBER: ");
    print_text_frame(disc_number);

    ID3v2_text_frame* composer = ID3v2_tag_get_composer(tag);
    printf("COMPOSER: ");
    print_text_frame(composer);

    // ID3v2_comment_frame* comment = ID3v2_tag_get_comment(tag);
    // printf("COMMENT: ");
    // print_comment_frame(comment);

    ID3v2_apic_frame* album_cover = ID3v2_tag_get_album_cover(tag);
    printf("ALBUM COVER: ");
    save_apic_frame(album_cover, "./extra");

    return 0;
}
