/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_constants_h
#define id3v2lib_constants_h

/**
 * TAG_HEADER CONSTANTS
 */
#define ID3_HEADER 10
#define ID3_HEADER_TAG 3
#define ID3_HEADER_VERSION 1
#define ID3_HEADER_REVISION 1
#define ID3_HEADER_FLAGS 1
#define ID3_HEADER_SIZE 4

#define NO_COMPATIBLE_TAG 0
#define ID3v23  1
#define ID3v24  2
// END TAG_HEADER CONSTANTS

/**
 * TAG_FRAME CONSTANTS
 */
#define ID3_FRAME 10
#define ID3_FRAME_ID 4
#define ID3_FRAME_SIZE 4
#define ID3_FRAME_FLAGS 2
#define ID3_FRAME_ENCODING 1
#define ID3_FRAME_LANGUAGE 3
#define ID3_FRAME_SHORT_DESCRIPTION 1

#define INVALID_FRAME 0
#define TEXT_FRAME 1
#define COMMENT_FRAME 2
#define APIC_FRAME 3

#define ISO_ENCODING 0
#define UTF_16_ENCODING 1
// END TAG_FRAME CONSTANTS

/**
 * FRAME IDs
 */
#define TITLE_FRAME_ID "TIT2"
#define ARTIST_FRAME_ID "TPE1"
#define ALBUM_FRAME_ID "TALB"
#define ALBUM_ARTIST_FRAME_ID "TPE2"
#define GENRE_FRAME_ID "TCON"
#define TRACK_FRAME_ID "TRCK"
#define YEAR_FRAME_ID "TYER"
#define COMMENT_FRAME_ID "COMM"
#define DISC_NUMBER_FRAME_ID "TPOS"
#define COMPOSER_FRAME_ID "TCOM"
#define ALBUM_COVER_FRAME_ID "APIC"
// END FRAME IDs


/**
 * APIC FRAME CONSTANTS
 */
#define ID3_FRAME_PICTURE_TYPE 1
#define JPG_MIME_TYPE "image/jpeg"
#define PNG_MIME_TYPE "image/png"

// Picture types:
#define OTHER 0x00
#define FILE_ICON 0x01
#define OTHER_FILE_ICON 0x02
#define FRONT_COVER 0x03
#define BACK_COVER 0x04
#define LEAFLET_PAGE 0x05
#define MEDIA 0x06
#define LEAD_ARTIST 0x07
#define ARTIST 0x08
#define CONDUCTOR 0x09
#define BAND 0x0A
#define COMPOSER 0x0B
#define LYRICIST 0x0C
#define RECORDING_LOCATION 0x0D
#define DURING_RECORDING 0x0E
#define DURING_PERFORMANCE 0x0F
#define VIDEO_SCREEN_CAPTURE 0x10
#define A_BRIGHT_COLOURED_FISH 0x11
#define ILLUSTRATION 0x12
#define ARTIST_LOGOTYPE 0x13
#define PUBLISHER_LOGOTYPE 0x14
// END APIC FRAME CONSTANTS

#endif
