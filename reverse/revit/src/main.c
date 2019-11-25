
#include "cryptor.h"
#include <stdio.h>

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        puts("Usage: ./revit <string to encrypt>");
        return 1;
    }

    #if DEBUG == 1
        printf("Plain message: %s. Plaintext size: %ld\n", argv[1], strlen(argv[1]));
    #endif
    size_t encrypted_len = padded_size(strlen(argv[1]), BLOCKSIZE);
    BYTE* enc = encrypt(argv[1], strlen(argv[1]));

    printf("Encrypted message: ");
    for (size_t i = 0; i < encrypted_len; ++i)
        printf("\\x%02x", enc[i]);
    putchar('\n');

    #ifdef DEBUG == 1
        char* dec = decrypt(enc, encrypted_len);
        printf("Decrypted message: %s\n", dec);
    #endif
    return 0;
}