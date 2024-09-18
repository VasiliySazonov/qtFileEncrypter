#ifndef TREEFISH_H
#define TREEFISH_H

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#define KEY_SIZE_WORDS 4
#define KEY_SCHEDULE_CONST 0x1BD11BDAA9FC1A22L

#define BINARY_ROTATE_LEFT_64(x, n) ((x << n) | (x >> (64 - n)))
#define BINARY_ROTATE_RIGHT_64(x, n) ((x >> n) | (x << (64 - n)))

typedef struct _key
{
    uint64_t key[KEY_SIZE_WORDS + 1];
    uint64_t tweak[3];
} threefish_key_t;

void threefish_set_key(threefish_key_t *key, uint64_t *key_data, uint64_t *key_tweak);
void threefish_encrypt(const threefish_key_t *key, const uint64_t *input, uint64_t *output);
void threefish_decrypt(const threefish_key_t *key, const uint64_t *input, uint64_t *output);


#endif // TREEFISH_H
