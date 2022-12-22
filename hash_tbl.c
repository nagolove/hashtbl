#include "hash_tbl.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

/*
    Раскладка данных такова:
    --------------------------------
    | Node | key data | value data |
    --------------------------------
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
    int len;
};

/*#define hashhh hashhh_xor*/
#define hashhh hashhh_add

static uint32_t hashhh_add(void *key, int key_len) {
    assert(key);
    assert(key_len > 0);
    char *s = key;
    uint32_t accum = 0;
    for (int i = 0; i < key_len; i++) {
        accum += s[i] * 9973;
        /*accum += s[i];*/
    }
    return accum;
    /*return 5;*/
}

static uint32_t hashhh_xor(void *key, int key_len) {
    assert(key);
    assert(key_len > 0);
    char *s = key;
    uint32_t accum = 0;
    for (int i = 0; i < key_len; i++) {
        accum ^= s[i] * 9973;
    }
    /*accum ^= (accum >> 11) ^ (accum >> 25);*/
    accum = accum * 69069U + 907133923UL;
    return accum;
}

HashTable *hashtbl_new() {
    struct HashTable *ht = calloc(1, sizeof(*ht));
    if (!ht) return NULL;

    ht->len = 1024 * 5;
    /*ht->len = 256;*/
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
            free(cur);
            cur = next;
        }
    }
    free(ht->arr);
    free(ht);
}

static inline void *get_key(struct Node *node) {
    assert(node);
    return (char*)node + sizeof(*node);
}

static inline void *get_value(struct Node *node) {
    assert(node);
    assert(node->key_len > 0);
    return (char*)node + sizeof(*node) + node->key_len;
}

bool hashtbl_add(
    HashTable *ht, void *key, int key_len, void *value, int value_len
) {
    assert(ht);
    assert(key);
    assert(key_len > 0);
    assert(value_len > 0);

    struct Node *node = calloc(1, sizeof(*node) + key_len + value_len);
    if (!node) return false;

    node->value_len = value_len;
    node->key_len = key_len;

    /*printf("key %s, %d\n", (char*)key, key_len);*/
    memcpy(get_key(node), key, key_len);
    memcpy(get_value(node), value, value_len);

    uint32_t hash_v = hashhh(key, key_len) % ht->len;
    struct BaseNode *bnode = &ht->arr[hash_v];
    struct Node *head = bnode->head;

    struct Node *cur = ht->arr[hash_v].head;
    while (cur) {
        //if (!memcmp(key, get_key(cur), key_len)) {
        if (!memcmp(key, get_key(cur), cur->key_len)) {
            node->next = cur->next;
            node->prev = cur->prev;
            free(cur);
            return false;
        }
        cur = cur->next;
    }

    if (!head) {
        node->next = NULL;
        node->prev = NULL;
    } else {
        head->prev = node;
        node->prev = NULL;
        node->next = head;
    }

    bnode->num++;
    bnode->head = node;

    return true;
}

bool hashtbl_remove(HashTable *ht, void *key, int key_len) {
    assert(ht);
    assert(key);

    uint32_t hash_v = hashhh(key, key_len) % ht->len;
    struct BaseNode *bnode = &ht->arr[hash_v];
    struct Node *cur = bnode->head;

    while (cur) {
        if (!memcmp(key, get_key(cur), cur->key_len)) {
            bnode->num--;
            assert(bnode->num >= 0);
            struct Node *tmp = cur;

            if (cur == bnode->head) {
                bnode->head = NULL;
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
    }
    ht->len = 0;
}

void *hashtbl_get(HashTable *ht, void *key, int key_len, int *value_len) {
    assert(ht);
    assert(key);

    if (!ht->len)
        return NULL;

    uint32_t hash_v = hashhh(key, key_len) % ht->len;
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

uint32_t hashtbl_get_count(HashTable *ht) {
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

void hashtbl_iter_begin(HashTable *ht) {
}

bool hashtbl_iter_next(
    HashTable *ht, void **key, int *key_len, void **value, int *value_len
) {
    return false;
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
