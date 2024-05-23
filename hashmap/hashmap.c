#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

//#define DEBUG_HASHMAP

hashmap new_hashmap(void)
{
    bucket* buckets = malloc(BUCKETS_SIZE * sizeof(bucket));
    if (buckets == NULL)
    {
        printf("malloc failure at new_hashmap.\n");
        exit(1);
    }
    assert(NULL == 0);
    memset((void*)buckets, 0, BUCKETS_SIZE * sizeof(bucket));
#ifdef DEBUG_HASHMAP
    printf("BEGIN NEW HASHMAP BUCKETS CHECK...\n");
    for (size_t i = 0; i < BUCKETS_SIZE; ++i)
    {
        if (
            buckets[i].key != NULL &&
            buckets[i].key_len != 0 &&
            buckets[i].value != NULL &&
            buckets[i].value_len != 0
        )
        {
            abort();
        }
    }
    printf("...CHECK PASSED!\n");
#endif
    hashmap result;
    result.buckets = buckets;
    return result;
}

uint8_t hash(void* key_in, size_t key_len)
{
    uint8_t* key = (uint8_t*)key_in;
    uint8_t result = 0;
    for (size_t i = 0; i < key_len; ++i)
    {
#ifdef DEBUG_HASHMAP
        printf("%d\n", key[i]);
        fflush(stdout);
#endif
        result += key[i];
    }
    return result;
}

void hashmap_insert(hashmap* obj, void* key, size_t key_len, void* value, size_t value_len)
{
    uint8_t hash_result = hash(key, key_len);

    // collision?
    if (key_len == obj->buckets[hash_result].key_len)
    {
        // memcmp can fail due to padding of structs not necessarily being zero filled, but oh well.
        // could fix that with a function pointer that retrieves the hash-pertinent values;
        // but this is a silly implementation anyway.
        if (memcmp(key, obj->buckets[hash_result].key, key_len))
        {
            bool free_spot_found = false;
            for (size_t i = 1; i < BUCKETS_SIZE; ++i)
            {
                if (
                    key_len != obj->buckets[(hash_result + i) % BUCKETS_SIZE].key_len ||
                    !memcmp(key, obj->buckets[(hash_result + i) % BUCKETS_SIZE].key, key_len))
                {
                    hash_result = (hash_result + i) % BUCKETS_SIZE;
                    free_spot_found = true;
#ifdef DEBUG_HASHMAP
                    printf("collision resolution i: %ld\n", i);
                    fflush(stdout);
#endif
                    break;
                }
            }
            if (!free_spot_found)
            {
                printf("No free spot found during collision resolution!\n");
                exit(2);
            }
        }
    }
    bucket b;
    b.key = key;
    b.key_len = key_len;
    b.value = value;
    b.value_len = value_len;

    obj->buckets[hash_result] = b;
}

int main()
{
    hashmap test = new_hashmap();
#ifdef DEBUG_HASHMAP
    printf("So far so good...\n");
    fflush(stdout);
#endif
    short key = 2;
    short value = 0;
    short key2 = 257;
    short value2 = 1;
    short key3 = 512;
    short value3 = 2;
#ifdef DEBUG_HASHMAP
    printf("KEY 1 HASH: %d\n", hash((void*)&key, sizeof(key)));
    printf("KEY 2 HASH: %d\n", hash((void*)&key2, sizeof(key2)));
    printf("KEY 3 HASH: %d\n", hash((void*)&key3, sizeof(key3)));
    fflush(stdout);
#endif
    // force collisions
    hashmap_insert(&test, &key, sizeof(key), &value, sizeof(value));
    hashmap_insert(&test, &key2, sizeof(key2), &value2, sizeof(value2));
    hashmap_insert(&test, &key3, sizeof(key3), &value3, sizeof(value3));
    assert(*(short*)test.buckets[2].value == 0);
    assert(*(short*)test.buckets[3].value == 1);
    assert(*(short*)test.buckets[4].value == 2);
}
