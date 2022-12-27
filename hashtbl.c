#include "hashtbl.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

//TODO: Как посчитать среднее число коллизий в таблице?
//TODO: Сжимать-ли таблицу при массовом удалении ключей?

/*
Раскладка данных такова:
----------------------------------------------------------------------------
| Node | (up to 16 bytes padding) key data | (16 bytes aligned) value data |
----------------------------------------------------------------------------
*/
struct Node {
    struct Node *prev;
    struct Node *next;
    int key_len, value_len;
};

struct BaseNode {
    struct Node *head;
    int num;
};

struct HashTable {
    struct BaseNode *arr;
    HashFunction    hasher;
    //int             len, longestchain, loaded;
    //XXX: longestchain - вопрос реализации
    //XXX: shortestchain - ???
    int             len, loaded;
};

/*#define default_hasher hasher_add*/
/*#define default_hasher hasher_xor*/
#define default_hasher hasher_fnv32

static inline uint32_t get_aligned_size(uint32_t size) {
    // XXX: Выделение лишних 16 байт при size % 16 == 0
    /*return size + 16 - size % 16;*/
    int mod = size % 16;
    /*return size - mod + (((mod + 15) / 16) << 4);*/
    return size - mod + (((mod + 15) >> 4) << 4);
}

HashTable *hashtbl_new(struct HashSetup *setup) {
    struct HashTable *ht = calloc(1, sizeof(*ht));
    if (!ht) return NULL;

    if (setup) {
        ht->len = setup->len ? setup->len : 1024;
        ht->hasher = setup->hasher ? setup->hasher : default_hasher;
    } else {
        ht->len = 1024;
        /*ht->len = 256;*/
        ht->hasher = default_hasher;
    }

    ht->arr = calloc(sizeof(ht->arr[0]), ht->len);
    return ht;
}

void hashtbl_free(HashTable *ht) {
    if (!ht) return;

    for (int k = 0; k < ht->len; k++) {
        struct BaseNode *bnode = &ht->arr[k];
        struct Node *cur = bnode->head;
        while (cur) {
            struct Node *next = cur->next;
            struct Node *tmp = cur;
            free(tmp);
            cur = next;
        }
    }
    free(ht->arr);
    free(ht);
}

static inline void *get_key(const struct Node *node) {
    assert(node);
    return (char*)node + get_aligned_size(sizeof(*node));
}

static inline void *get_value(const struct Node *node) {
    assert(node);
    assert(node->key_len > 0);
    return (char*)node + get_aligned_size(sizeof(*node)) +
                         get_aligned_size(node->key_len);
}

bool hashtbl_add(
    HashTable *ht, 
    const void *key, int key_len, const void *value, int value_len
) {
    assert(ht);
    assert(key);
    assert(key_len > 0);
    assert(value_len > 0);

    uint32_t size = get_aligned_size(sizeof(struct Node)) + 
                    get_aligned_size(key_len) + value_len;
    struct Node *node = malloc(size);
    if (!node) return false;

    node->value_len = value_len;
    node->key_len = key_len;

    memcpy(get_key(node), key, key_len);
    memcpy(get_value(node), value, value_len);

    Hash_t hash_v = ht->hasher(key, key_len) % ht->len;
    struct BaseNode *bnode = &ht->arr[hash_v];
    //struct Node *head = bnode->head;
    struct Node *cur = ht->arr[hash_v].head;
    while (cur) {

        /*
        printf(
            "key '%s', get_key '%s', cur->key_len %d\n", 
            (char*)key, (char*)get_key(cur), cur->key_len
        );
        // */
        
        if (!memcmp(key, get_key(cur), cur->key_len)) {
            node->next = cur->next;
            node->prev = cur->prev;
            //bnode->num--;
            free(cur);
            return false;
        }
        cur = cur->next;
    }

    if (!bnode->head) {
        node->next = NULL;
        node->prev = NULL;
        ht->loaded++;
    } else {
        bnode->head->prev = node;
        node->prev = NULL;
        node->next = bnode->head;
    }

    bnode->num++;
    bnode->head = node;

    return true;
}

bool hashtbl_remove(HashTable *ht, const void *key, int key_len) {
    assert(ht);
    assert(key);

    Hash_t hash_v = ht->hasher(key, key_len) % ht->len;
    struct BaseNode *bnode = &ht->arr[hash_v];
    struct Node *cur = bnode->head;

    while (cur) {
        if (!memcmp(key, get_key(cur), cur->key_len)) {
            bnode->num--;
            assert(bnode->num >= 0);
            struct Node *tmp = cur;

            if (cur == bnode->head) {
                bnode->head = NULL;
                ht->loaded--;
            } else {

                if (cur->prev)
                    cur->prev->next = cur->next;
                if (cur->next)
                    cur->next->prev = cur->prev;

            }

            free(tmp);

            return true;
        }
        cur = cur->next;
    }

    return false;
}

void hashtbl_clear(HashTable *ht) {
    for (int k = 0; k < ht->len; k++) {
        struct BaseNode *bnode = &ht->arr[k];
        struct Node *cur = bnode->head;
        while (cur) {
            struct Node *next = cur->next;
            free(cur);
            cur = next;
        }
        bnode->head = NULL;
        bnode->num = 0;
    }
    ht->loaded = 0;
}

void *hashtbl_get(HashTable *ht, const void *key, int key_len, int *value_len) {
    assert(ht);
    assert(key);

    if (!ht->len)
        return NULL;

    Hash_t hash_v = ht->hasher(key, key_len) % ht->len;
    struct Node *cur = ht->arr[hash_v].head;

    while (cur) {
        //if (!memcmp(key, get_key(cur), key_len)) {
        /*printf("get_key(cur) %s\n", (char*)get_key(cur));*/
        if (!memcmp(key, get_key(cur), cur->key_len)) {
            if (value_len)
                *value_len = cur->value_len;
            return get_value(cur);
        }
        cur = cur->next;
    }

    return NULL;
}

uint32_t hashtbl_get_count(const HashTable *ht) {
    assert(ht);
    uint32_t count = 0;
    for (int j = 0; j < ht->len; ++j) {
        count += ht->arr[j].num;
    }
    return count;
}

void hashtbl_each(HashTable *ht, HashTableIterator func, void *data) {
    assert(ht);
    assert(func);

    for (int j = 0; j < ht->len; j++) {
        struct Node *cur = ht->arr[j].head;
        while (cur) {
            HashTableAction act = func(
                get_key(cur), cur->key_len, 
                get_value(cur), cur->value_len, 
                data
            );
            switch (act) {
                case HT_ACTION_NEXT:
                      break;
                case HT_ACTION_REMOVE_NEXT: {
                      struct Node *next = cur->next;
                      struct Node *prev = cur->prev;
                      if (next)
                          next->prev = prev;
                      if (prev)
                          prev->next = next;
                      break;
                }
                case HT_ACTION_REMOVE_BREAK: {
                    printf("HT_ACTION_REMOVE_BREAK is not implemented\n");
                    abort();
                    break;
                }
                case HT_ACTION_BREAK:
                      return;
            }
            cur = cur->next;
        }
    }
}

void hashtbl_dump_collisions(HashTable *ht, const char *fname) {
    assert(ht);
    assert(fname);

    FILE *f = fopen(fname, "w");
    if (!f) {
        printf("Could not save to file %s\n", fname);
        exit(EXIT_FAILURE);
    }
    
    fprintf(f, "{\n");
    fprintf(f, "len = %d,\n", ht->len);
    for (int i = 0; i < ht->len; ++i) {
        fprintf(f, "%d, ", ht->arr[i].num);
    }
    fprintf(f, "}\n");

    fclose(f);
}

HashTable *hashtbl_clone(HashTable *ht) {
    assert(ht);
    return NULL;
}

Hash_t hasher_fnv32(const void *data, int len) {
    assert(data);
    assert(len > 0);
    const char *bytes = (char*)data;
    Hash_t h = 0x811c9dc5;

    for (; len >= 8; len -= 8, bytes += 8) {
        h = (h ^ bytes[0]) * 0x01000193;
        h = (h ^ bytes[1]) * 0x01000193;
        h = (h ^ bytes[2]) * 0x01000193;
        h = (h ^ bytes[3]) * 0x01000193;
        h = (h ^ bytes[4]) * 0x01000193;
        h = (h ^ bytes[5]) * 0x01000193;
        h = (h ^ bytes[6]) * 0x01000193;
        h = (h ^ bytes[7]) * 0x01000193;
    }

    while (len--) {
        h = (h ^ *bytes++) * 0x01000193;
    }

    assert(h != 0);
    return h;
}

uint64_t hasher_fnv64(const void *data, int len) {
    const char *bytes = (char*)data;
    uint64_t h = 0xcbf29ce484222325ull;

    for (; len >= 8; len -= 8, bytes += 8) {
        h = (h ^ bytes[0]) * 0x100000001b3ull;
        h = (h ^ bytes[1]) * 0x100000001b3ull;
        h = (h ^ bytes[2]) * 0x100000001b3ull;
        h = (h ^ bytes[3]) * 0x100000001b3ull;
        h = (h ^ bytes[4]) * 0x100000001b3ull;
        h = (h ^ bytes[5]) * 0x100000001b3ull;
        h = (h ^ bytes[6]) * 0x100000001b3ull;
        h = (h ^ bytes[7]) * 0x100000001b3ull;
    }

    while (len--) {
        h = (h ^ *bytes++) * 0x100000001b3ull;
    }
    return h;
}

Hash_t hasher_add(const void *key, int key_len) {
    assert(key);
    assert(key_len > 0);
    const char *s = key;
    Hash_t accum = 0;

    /*for (int i = 0; i < key_len; i++) {*/
    for (; key_len >= 8; key_len -= 8, s += 8) {
        accum += s[0] * 9973;
        accum += s[1] * 9973;
        accum += s[2] * 9973;
        accum += s[3] * 9973;
        accum += s[4] * 9973;
        accum += s[5] * 9973;
        accum += s[6] * 9973;
        accum += s[7] * 9973;
    }

    while (key_len--) {
        accum += *s++;
    }

    return accum;
}

Hash_t hasher_xor(const void *key, int key_len) {
    assert(key);
    assert(key_len > 0);
    const char *s = key;
    Hash_t accum = 0;
    for (int i = 0; i < key_len; i++) {
        accum ^= s[i] * 9973;
    }
    /*accum ^= (accum >> 11) ^ (accum >> 25);*/
    accum = accum * 69069U + 907133923UL;
    return accum;
}

