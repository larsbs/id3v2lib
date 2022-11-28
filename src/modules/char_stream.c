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

#include "char_stream.private.h"

CharStream* CharStream_new(const int size)
{
    CharStream* cs = (CharStream*) malloc(sizeof(CharStream));
    cs->stream = (char*) calloc(size, sizeof(char));
    cs->cursor = 0;
    cs->size = size;
    return cs;
}

CharStream* CharStream_from_buffer(const char* buffer, const int size)
{
    CharStream* cs = (CharStream*) malloc(sizeof(CharStream));
    cs->stream = (char*) calloc(size, sizeof(char));
    memcpy(cs->stream, buffer, size);
    cs->cursor = 0;
    cs->size = size;
    return cs;
}

void CharStream_write(CharStream* cs, const char* data, const int size)
{
    memcpy(cs->stream + cs->cursor, data, size);
    cs->cursor += size;
}

int CharStream_read(CharStream* cs, char* dest, const int size)
{
    memcpy(dest, cs->stream + cs->cursor, size);
    cs->cursor += size;
}

int CharStream_getc(CharStream* cs)
{
    if (cs->cursor >= cs->size) return EOF;
    int c = cs->stream[cs->cursor];
    cs->cursor++;
    return c;
}

void CharStream_free(CharStream* cs)
{
    free(cs->stream);
    free(cs);
}