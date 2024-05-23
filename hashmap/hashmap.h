#define BUCKETS_SIZE 256

typedef struct {
    void* key;
    size_t key_len;
    void* value;
    size_t value_len;
} bucket;

typedef struct {
    bucket* buckets;
} hashmap;

hashmap new_hashmap(void);

void hashmap_insert(hashmap* obj, void* p_key, size_t p_key_len, void* p_value, size_t p_value_len);

void* hashmap_get(hashmap* obj, void* p_key, size_t p_key_len);
