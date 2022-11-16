#include <stdio.h>

#include "id3v2lib.h"


int main(int argc, char* argv[])
{
    ID3v2_tag* tag = ID3v2_read_tag("./extra/file.mp3");

    return 0;
}
