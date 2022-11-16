#include <stdio.h>

#include "id3v2lib.h"


int main(int argc, char* argv[])
{
    ID3v2_tag* tag = ID3v2_read_tag("./extra/file.mp3");

    char* artist = ID3v2_tag_get_artist(tag);
    printf("Artist: %s\n", artist);

    return 0;
}
