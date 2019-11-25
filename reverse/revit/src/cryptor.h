#ifndef CRYPTOR_H
#define CRYPTOR_H

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <ctype.h>

typedef unsigned char BYTE;
typedef unsigned long long QWORD;

#define XORKEY 0xd86d363d9ceb447a
#define CHARXORKEYEVEN 0xf4
#define CHARXORKEYODD  0xd3
#define SHIFTLEFT -3
#define BLOCKSIZE 8

#define padded_size(LEN, MULTIPLE_OF) ((LEN) + (((~LEN) + 1) & ((MULTIPLE_OF) - 1)))

/*
    1. Divide string in blocks of 8, if size isn't multiple of 8, pad string with null bytes at the start
    2. Rotate every 8-byte block by 3 to the left:                     'ABCDEFGH' -> 'DEFGHABC'
    3. swap elements which are placed at even indexes, with odd one's: 'DEFGHABC' -> 'EDGFAHCB'
    4. XOR elements at even indexes with CHARXORKEYEVEN (0xd3):        'EDGFAHCB' -> '\xb1D\xb3F\xb5H\xb7B'
    5. XOR elements at odd  indexes with CHARXORKEYODD  (0xf4):        '\xb1D\xb3F\xb5H\xb7B' -> '\xb1\x97\xb3\x95\xb5\x9b\xb7\x91'
    6. Divide big block (8) into small blocks (2), and swap them by this rule:
        0 <-> 2
        1 <-> 3
        '[0     ][1     ][2     ][3     ]' -> '[0     ][1     ][2     ][3     ]' 
        '\xb1\x97\xb3\x95\xb5\x9b\xb7\x91' -> '\xb5\x9b\xb7\x91\xb1\x97\xb3\x95'
    7. Divide big block (8) into small blocks (1), and swap them by this rule:
        0 <-> 6
        1 <-> 5
        2 <-> 7
        3 <-> 4
        '[0 ][1 ][2 ][3 ][4 ][5 ][6 ][7 ]' -> '[0 ][1 ][2 ][3 ][4 ][5 ][6 ][7 ]'
        '\xb5\x9b\xb7\x91\xb1\x97\xb3\x95' -> '\xb3\x97\x95\xb1\x91\x9b\xb5\xb7'
    8. Xor blocks of 8 with XORKEY (0xd86d363d9ceb447a): '\xb3\x97\x95\xb1\x91\x9b\xb5\xb7' -> '\x6b\xfa\xa3\x8c\x0d\x70\xf1\xcd'
    9. Finally reverse complete string '\x6b\xfa\xa3\x8c\x0d\x70\xf1\xcd' -> '\xcd\xf1\x70\x0d\x8c\xa3\xfa\x6b'
*/
BYTE* encrypt(char* plaintext, size_t plaintext_len);

// reverse BYTE array=arr of size=len
BYTE* reverse(BYTE *arr, size_t len);

// rotate BYTE string=str of size=len to left/right based on rot (rotate left - negative, rotate right - positive)
BYTE* rotate(BYTE *str, size_t len, int rot);

/*pad BYTE array=arr of size=len with appropriate character=pad_byte, to be multiple of 'multiple_of'
Note: it will allocate new buffer using calloc, so if you are allocated buffer for previous value using malloc, you must free it 
*/
BYTE* pad_front(BYTE *arr, size_t len, BYTE pad_byte, int multiple_of);

// swap 2 elements (first, second) of size=data_size
void swap(void* first, void* second, size_t data_size);

#if DEBUG == 1
    // copy-pasted from: https://stackoverflow.com/questions/7775991/how-to-get-hexdump-of-a-structure-data
    // write memory region as hex
    void dump_hex(const char *desc, const void *addr, const size_t len);

    // decrypt encrypted message
    char* decrypt(BYTE* ciphertext, size_t len);
#endif

#endif