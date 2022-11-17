/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

bool has_bom(char* string)
{
    if(memcmp("\xFF\xFE", string, 2) == 0 || memcmp("\xFE\xFF", string, 2) == 0)
    {
        return true;
    }

    return false;
}

uint16_t* char_to_utf16(char* string, int size)
{
    if (size < 0)
    {
        // If size < 0, compute the size finding the position of the termination marker
        size = 1;
        char prev = string[0];
        while (1)
        {
            char curr = string[size++];

            if (prev == 0x00 && curr == 0x00)
            {
                size++;
                break;
            }

            prev = curr;
        }
    }

    uint16_t* result = (uint16_t*) malloc(size * sizeof(uint16_t));
    memcpy(result, string, size);
    return result;
}

void println_utf16(uint16_t* string, int size)
{
    int i = 1; // Skip the BOM

    // If size < 0, then keep iterating until we find the termination marker,
    // otherwise, use size as a safe limit.
    while(i < size || size < 0)
    {
        // break if we reach the termination marker '0000'
        if(string[i] == 0x0000)
        {
            break;
        }

        printf("%lc", string[i]);
        i++;
    }

    printf("\n");
}

void print_tag_text(char* text, int size)
{
    if (has_bom(text))
    {
        uint16_t* text_utf = char_to_utf16(text, size);
        println_utf16(text_utf, size);
    }
    else
    {
        printf("%s\n", text);
    }
}
