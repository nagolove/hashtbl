#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef struct HashTable HashTable;
typedef enum HashTableAction{
    HT_ACTION_NEXT,
    HT_ACTION_REMOVE_NEXT,
    HT_ACTION_REMOVE_BREAK,
    HT_ACTION_BREAK,
} HashTableAction;
typedef HashTableAction (*HashTableIterator)(
    const void *key, int key_len, void *value, int value_len, void *data
);
typedef uint32_t Hash_t;
typedef Hash_t (*HashFunction)(const void *key, int key_len);

struct HashSetup {
    int          len;
    HashFunction hasher;
};

HashTable *hashtbl_new(struct HashSetup *setup);
HashTable *hashtbl_clone(HashTable *ht);
void hashtbl_free(HashTable *ht);

bool hashtbl_add(
    HashTable *ht, 
    const void *key, int key_len, const void *value, int value_len
);

#define hashtbl_add_s(ht, key_s, value, value_len) \
    hashtbl_add(ht, key_s, strlen(key_s) + 1, value, value_len)
#define hashtbl_add_ss(ht, key_s, value_s) \
    hashtbl_add(ht, key_s, strlen(key_s) + 1, value_s, strlen(value_s) + 1)

#define hashtbl_remove_s(ht, key_s) \
    hashtbl_remove(ht, key_s, strlen(key_s) + 1)

// Возвращает истину если удаление произошло
bool hashtbl_remove(HashTable *ht, const void *key, int key_len);
void hashtbl_clear(HashTable *ht);

#define hashtbl_get_s(ht, key_s, value_len) \
    hashtbl_get(ht, key_s, strlen(key_s) + 1, value_len)

void *hashtbl_get(HashTable *ht, const void *key, int key_len, int *value_len);
uint32_t hashtbl_get_count(const HashTable *ht);
void hashtbl_each(HashTable *ht, HashTableIterator func, void *data);

void hashtbl_dump_collisions(HashTable *ht, const char *fname);

//uint64_t hasher_fnv64(const void *data, int len);
Hash_t hasher_fnv32(const void *data, int len);
Hash_t hasher_add(const void *key, int key_len);
Hash_t hasher_xor(const void *key, int key_len);
