/*
 * This file is part of id3v2lib library
 *
 * Copyright (c) Lars Ruiz
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#ifndef id3v2lib_utils_private_h
#define id3v2lib_utils_private_h

unsigned int btoi(const char* bytes, int size);
char* itob(int integer);
int syncint_encode(int value);
unsigned int syncint_decode(int value);

#endif