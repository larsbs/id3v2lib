/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "test_utils.h"

bool has_bom(char* string)
{
    if (string == NULL)
    {
        return false;
    }

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

char* to_unicode(char* string)
{
    if (has_bom(string))
    {
        return string;
    }

    int size = (strlen(string) * 2) + 4; // +4 to take the BOM and the termination into account;
    char* result = (char*) malloc(size * sizeof(char));

    result[0] = 0xFF;
    result[1] = 0xFE;

    for (int i = 1; i <= strlen(string); i++)
    {
        result[i * 2] = string[i - 1];
        result[(i * 2) + 1] = 0x00;
    }

    result[size - 2] = 0x00;
    result[size - 1] = 0x00;

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

void print_text_frame(ID3v2_text_frame* frame)
{
    if (frame == NULL)
    {
        printf("None\n");
        return;
    }

    print_text_frame_text(frame->data->text, frame->data->size);
}

void print_text_frame_text(char* text, int size)
{
    if (text == NULL)
    {
        printf("None\n");
    }
    else if (has_bom(text))
    {
        uint16_t* text_utf = char_to_utf16(text, size);
        println_utf16(text_utf, size);
    }
    else
    {
        printf("%s\n", text);
    }
}

void print_comment_frame(ID3v2_comment_frame* frame)
{
    if (frame == NULL)
    {
        printf("None\n");
        return;
    }

    print_text_frame_text(frame->data->comment, frame->data->size);
}

void print_comment_frames(ID3v2_frame_list* frames)
{
    if (frames == NULL)
    {
        printf("None\n");
        return;
    }

    while (frames != NULL)
    {
        ID3v2_comment_frame* comment = (ID3v2_comment_frame*) frames->frame;
        print_comment_frame(comment);
        frames = frames->next;
    }
}

void save_apic_frame(ID3v2_apic_frame* frame, char* dir_path)
{
    if (frame == NULL)
    {
        printf("None\n");
        return;
    }

    char* extension = strcmp(frame->data->mime_type, ID3v2_MIME_TYPE_PNG) == 0 ? ".png" : ".jpeg";
    int file_name_size = strlen(dir_path) + strlen("/album_cover") + strlen(extension);
    char* file_name = (char*) malloc((file_name_size) * sizeof(char));
    strcpy(file_name, dir_path);
    strcat(file_name, "/album_cover");
    strcat(file_name, extension);

    FILE* file = fopen(file_name, "wb");
    fwrite(frame->data->data, 1, frame->data->picture_size, file);
    fclose(file);

    printf("Saved in %s", file_name);
}
