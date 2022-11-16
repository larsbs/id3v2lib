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

#include "utils.private.h"

unsigned int btoi(const char* bytes, int size)
{
    unsigned int result = 0x00;

    for(int i = 0; i < size; i++)
    {
        result = result << 8;
        result = result | (unsigned char) bytes[i];
    }

    return result;
}

char* itob(int integer)
{
    int size = 4;
    char* result = (char*) malloc(sizeof(char) * size);

    // We need to reverse the bytes because Intel uses little endian.
    char* aux = (char*) &integer;

    for(int i = size - 1; i >= 0; i--)
    {
        result[size - 1 - i] = aux[i];
    }

    return result;
}

int syncint_encode(int value)
{
    int out, mask = 0x7F;

    while (mask ^ 0x7FFFFFFF)
    {
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }

    return out;
}

unsigned int syncint_decode(int value)
{
    unsigned int a, b, c, d, result = 0x0;
    a = value & 0xFF;
    b = (value >> 8) & 0xFF;
    c = (value >> 16) & 0xFF;
    d = (value >> 24) & 0xFF;

    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);

    return result;
}