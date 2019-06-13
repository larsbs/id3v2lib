/*
 * This file is part of the id3v2lib library
 *
 * Copyright (c) 2013, Lorenzo Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

unsigned int btoi(char* bytes, int size, int offset)
{
    unsigned int result = 0x00;
    int i = 0;
    for(i = 0; i < size; i++)
    {
        result = result << 8;
        result = result | (unsigned char) bytes[offset + i];
    }
    
    return result;
}

char* itob(int integer)
{
    int i;
    int size = 4;
    char* result = (char*) malloc(sizeof(char) * size);
    
    // We need to reverse the bytes because Intel uses little endian.
    char* aux = (char*) &integer;
    for(i = size - 1; i >= 0; i--)
    {
        result[size - 1 - i] = aux[i];
    }
    
    return result;
}

int syncint_encode(int value)
{
    int out, mask = 0x7F;
    
    while (mask ^ 0x7FFFFFFF) {
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }
    
    return out;
}

int syncint_decode(int value)
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

void add_to_list(ID3v2_frame_list* main, ID3v2_frame* frame)
{
    ID3v2_frame_list *current;

    // if empty list
    if(main->start == NULL)
    {
        main->start = main;
        main->last = main;
        main->frame = frame;
    }
    else
    {
        current = new_frame_list();
        current->frame = frame;
        current->start = main->start;
        main->last->next = current;
        main->last = current;
    }
}

ID3v2_frame* get_from_list(ID3v2_frame_list* list, char* frame_id)
{
    while(list != NULL && list->frame != NULL)
    {
        if(strncmp(list->frame->frame_id, frame_id, 4) == 0) {
            return list->frame;
        }
        list = list->next;
    }
    return NULL;
}

void free_tag(ID3v2_tag* tag)
{
    ID3v2_frame_list *list;

    free(tag->raw);
    free(tag->tag_header);
    list = tag->frames;
    while(list != NULL)
    {
        if (list->frame) free(list->frame->data);
        free(list->frame);
        list = list->next;
    }
    free(list);
    free(tag);
}

char* get_mime_type_from_filename(const char* filename)
{
    if(strcmp(strrchr(filename, '.') + 1, "png") == 0)
    {
        return PNG_MIME_TYPE;
    }
    else
    {
        return JPG_MIME_TYPE;
    }
}

void genre_num_string(char* genre_data, char* dest)
{
    if (genre_data == NULL)
    {
	printf("genre data is null\n");
        return;
    }

    int length = strlen(genre_data);
    int genre_number_index = 0;
    char genre_number[3];
    int first_parenthesis_found = 1;
    int second_parenthese_found = 1;
    
    for (int i = 0; i < length; ++i)
    {
	printf("Searching the char is %c\n", genre_data[i]);
        if (genre_data[i] == '(' && first_parenthesis_found == 1)
	{
            //printf("First parenthese found\n");
	    first_parenthesis_found = 0;
	    continue;
	}
	if (genre_data[i] == ')' && second_parenthese_found == 1 )//&&
	        //first_parenthesis_found == 0)
	{
	    //printf("Second parenthesis found\n");
	    second_parenthese_found = 0;
	    break;
	}
	if (first_parenthesis_found == 0)
	{
	    genre_number[genre_number_index++] = genre_data[i];
	    printf("%s", genre_number);
	}
    }

    if (first_parenthesis_found == 1 || second_parenthese_found == 1) {
        printf("Genre already set or does not exist\n");
	return;
    }

    printf("%s %d", genre_number, atoi(genre_number));
    char* dest_tmp = convert_genre_number(atoi(genre_number));
    strcpy(dest, dest_tmp);
}

char* convert_genre_number(int number)
{
    char *genre = "dd";
    switch (number)
    {
	case NUM_HIP_HOP:
	    genre = HIP_HOP;
	    break;
        case NUM_R_AND_B:
	    genre = R_AND_B;
	    break;
    }

    return genre;
}

// String functions
int has_bom(uint16_t* string)
{
    if(memcmp("\xFF\xFE", string, 2) == 0 || memcmp("\xFE\xFF", string, 2) == 0)
    {   
        return 1;
    }
    
    return 0;
}

uint16_t* char_to_utf16(char* string, int size)
{
    uint16_t* result = (uint16_t*) malloc(size * sizeof(uint16_t));
    memcpy(result, string, size);
    return result;
}

void println_utf16(uint16_t* string, int size)
{
    int i = 1; // Skip the BOM
    while(1)
    {
        if(size > 0 && i > size)
        {
            break;
        }
        
        if(string[i] == 0x0000)
        {
            break;
        }
        
        printf("%lc", string[i]);
        i++;
    }
    printf("\n");
}

char* get_path_to_file(const char* file)
{
    char* file_name = strrchr(file, '/');
    unsigned long size = strlen(file) - strlen(file_name) + 1; // 1 = trailing '/'
    
    char* file_path = (char*) malloc(size * sizeof(char));
    strncpy(file_path, file, size);
    
    return file_path;
}
