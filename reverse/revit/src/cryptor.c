#include "cryptor.h"

BYTE* encrypt(char* plaintext, size_t plaintext_len)
{
    // pad string if needed
    size_t padded_len    = padded_size(plaintext_len, BLOCKSIZE);
    BYTE*  encrypted_msg = pad_front((BYTE*)plaintext, plaintext_len, '\x00', BLOCKSIZE);
    #if DEBUG == 1
        printf("Padded message size: %zu\n", padded_len);
        dump_hex("1. Padded message" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 2. rotate 8 byte blocks
    for (size_t i = 0; i < padded_len; i += 8)
        rotate(encrypted_msg + i, BLOCKSIZE, SHIFTLEFT);
    #if DEBUG == 1
        dump_hex("2. Rotated by 8-byte blocks" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 3. swap odd/even elements
    for (size_t i = 0; i < padded_len; i += 2)
        swap(&encrypted_msg[i], &encrypted_msg[i + 1], sizeof(BYTE));
    #if DEBUG == 1
        dump_hex("3. Swapped off/even" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 4,5. Xor odd/even elements
    for (size_t i = 0; i < padded_len; ++i)
    {
        if (i % 2 == 0)
            encrypted_msg[i] ^= CHARXORKEYEVEN;
        else 
            encrypted_msg[i] ^= CHARXORKEYODD;
    }
    #if DEBUG == 1
        dump_hex("4,5. Xored off/even" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 6. swap blocks of 2
    for (size_t i = 0; i < padded_len / 2; i += BLOCKSIZE)
    {
        BYTE *pc = encrypted_msg + i;
        #if DEBUG == 1
            printf("swapping elements in block: %zu\n", i / 8);
        #endif
        swap(&pc[0], &pc[4], sizeof(BYTE) * 2);
        swap(&pc[2], &pc[6], sizeof(BYTE) * 2);
    }
    #if DEBUG == 1
        dump_hex("6. Swapped (blocks of 2)" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 7. swap blocks of 1
    for (size_t i = 0; i < padded_len; i += BLOCKSIZE)
    {
        BYTE* pc = encrypted_msg + i;
        #if DEBUG == 1
            printf("swapping elements in block: %p\n", pc);
        #endif
        swap(&pc[0], &pc[6], sizeof(BYTE));
        swap(&pc[3], &pc[4], sizeof(BYTE));
        swap(&pc[2], &pc[7], sizeof(BYTE));
        swap(&pc[1], &pc[5], sizeof(BYTE));
    }
    #if DEBUG == 1
        dump_hex("7. Swapped (blocks of 1)" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 8. Xor blocks of 8 using XORKEY
    for (size_t i = 0; i < padded_len; i += BLOCKSIZE)
    {
        BYTE* pc = encrypted_msg + i;
        #if DEBUG == 1
            dump_hex("xoring elements in block", pc, BLOCKSIZE);
            printf("value: %zx\n", ((QWORD*)(pc))[i]);
        #endif
        ((QWORD*)pc)[i] ^= __builtin_bswap64(XORKEY);
    }
    #if DEBUG == 1
        dump_hex("8. xored blocks" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    // 9. Reverse string
    reverse(encrypted_msg, padded_len);
    #if DEBUG == 1
        dump_hex("9. Reversed string" , encrypted_msg, padded_len);
        putchar('\n');
    #endif

    return encrypted_msg;
}

BYTE* reverse(BYTE *arr, size_t len)
{
    if (arr == NULL)
        return NULL;

    for (size_t i = 0, j = len-1; i < j; i++, j--) 
    {
        swap(&arr[i], &arr[j], sizeof(BYTE));
    }

    return arr;
}

BYTE* rotate(BYTE *str, size_t len, int rot)
{
    if (str == NULL)
        return NULL;

    BYTE* rotated = (BYTE*)calloc(len, sizeof(BYTE));
    if (rotated == NULL)
        return NULL;

    for (int i = 0; i < len; ++i)
    {
        rotated[i] = str[(len - rot + i) % len];
    }
    memcpy(str, rotated, len);
    free(rotated);

    return str;
}

BYTE* pad_front(BYTE* arr, size_t len, BYTE pad_byte, int multiple_of)
{
    if (arr == NULL)
        return NULL;

    // allocate memory, and copy already-existing part
    BYTE* new_mem = (BYTE*)calloc(padded_size(len, multiple_of), sizeof(BYTE));
    memcpy(new_mem + (padded_size(len, multiple_of) - len), arr, len);

    for (size_t cur_idx = 0; len % multiple_of != 0; len++)
    {
        new_mem[cur_idx++] = pad_byte;
    }

    return new_mem;
}

void swap(void* first, void* second, size_t data_size)
{
    if (first == NULL || second == NULL)
        return;
    
    void *tmp_buf = calloc(data_size, sizeof(BYTE));

    memcpy(tmp_buf, first, data_size);
    memcpy(first, second, data_size);
    memcpy(second, tmp_buf, data_size);

    free(tmp_buf);
}

#if DEBUG == 1
    void dump_hex(const char *desc, const void *addr, const size_t len)
    {
        int i;
        unsigned char buff[17];
        const unsigned char *pc = (const unsigned char*)addr;

        // Output description if given.
        if (desc != NULL)
            printf ("%s:\n", desc);

        if (len == 0) {
            printf("  ZERO LENGTH\n");
            return;
        }
        if (len < 0) {
            printf("  NEGATIVE LENGTH: %i\n",len);
            return;
        }

        // Process every byte in the data.
        for (i = 0; i < len; i++) {
            // Multiple of 16 means new line (with line offset).

            if ((i % 16) == 0) {
                // Just don't print ASCII for the zeroth line.
                if (i != 0)
                    printf ("  %s\n", buff);

                // Output the offset.
                printf ("  %04x ", i);
            }

            // Now the hex code for the specific character.
            printf (" %02x", pc[i]);

            // And store a printable ASCII character for later.
            if ((pc[i] < 0x20) || (pc[i] > 0x7e))
                buff[i % 16] = '.';
            else
                buff[i % 16] = pc[i];
            buff[(i % 16) + 1] = '\0';
        }

        // Pad out last line if not exactly 16 characters.
        while ((i % 16) != 0) {
            printf ("   ");
            i++;
        }

        // And print the final ASCII bit.
        printf ("  %s\n", buff);
    }

    char* decrypt(BYTE* ciphertext, size_t len)
    {
        // 9. reverse string
        reverse(ciphertext, len);
        dump_hex("9. Reversed string" , ciphertext, len);
        putchar('\n');

        // 8. xor blocks of 8
        for (size_t i = 0; i < len; i += BLOCKSIZE)
        {
            BYTE* pc = ciphertext + i;
            #if DEBUG == 1
                dump_hex("xoring elements in block", pc, BLOCKSIZE);
                printf("value: %zx\n", ((QWORD*)(pc))[i]);
            #endif
            ((QWORD*)pc)[i] ^= __builtin_bswap64(XORKEY);
        }
        dump_hex("8. xored blocks" , ciphertext, len);
        putchar('\n');
    
        // 7. swap blocks of 1
        for (size_t i = 0; i < len; i += BLOCKSIZE)
        {
            BYTE* pc = ciphertext + i;
            #if DEBUG == 1
                printf("swapping elements in block: %p\n", pc);
            #endif
            swap(&pc[0], &pc[6], sizeof(BYTE));
            swap(&pc[3], &pc[4], sizeof(BYTE));
            swap(&pc[2], &pc[7], sizeof(BYTE));
            swap(&pc[1], &pc[5], sizeof(BYTE));
        }
        dump_hex("7. Swapped (blocks of 1)" , ciphertext, len);
        putchar('\n');
        
        // 6. swap blocks of 2
        for (size_t i = 0; i < len / 2; i += BLOCKSIZE)
        {
            BYTE *pc = ciphertext + i;
            #if DEBUG == 1
                printf("swapping elements in block: %zu\n", i / 8);
            #endif
            swap(&pc[0], &pc[4], sizeof(BYTE) * 2);
            swap(&pc[2], &pc[6], sizeof(BYTE) * 2);
        }
        #if DEBUG == 1
            dump_hex("6. Swapped (blocks of 2)" , ciphertext, len);
            putchar('\n');
        #endif

        // 4,5. Xor odd/even elements
        for (size_t i = 0; i < len; ++i)
        {
            if (i % 2 == 0)
                ciphertext[i] ^= CHARXORKEYEVEN;
            else 
                ciphertext[i] ^= CHARXORKEYODD;
        }
        #if DEBUG == 1
            dump_hex("4,5. Xored off/even" , ciphertext, len);
            putchar('\n');
        #endif

        // 3. swap odd/even elements
        for (size_t i = 0; i < len; i += 2)
            swap(&ciphertext[i], &ciphertext[i + 1], sizeof(BYTE));
        #if DEBUG == 1
            dump_hex("3. Swapped off/even" , ciphertext, len);
            putchar('\n');
        #endif

        // 2. rotate 8 byte blocks
        for (size_t i = 0; i < len; i += 8)
            rotate(ciphertext + i, BLOCKSIZE, -SHIFTLEFT);
        #if DEBUG == 1
            dump_hex("2. Rotated by 8-byte blocks" , ciphertext, len);
            putchar('\n');
        #endif

        size_t i = 0;
        while (*ciphertext == '\0')
        {
            ++i;
            ++ciphertext;
        }
        ciphertext[len-i] = '\x00';

        return ciphertext;
    }
#endif