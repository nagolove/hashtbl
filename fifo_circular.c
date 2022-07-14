#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "fifo_circular.h"

void cb_init(CIRC_BUF *cb, int maxlen) {
    assert(maxlen > 0);
    assert(cb);
    memset(cb, 0, sizeof(CIRC_BUF));
    cb->arr = calloc(maxlen, sizeof(cb->arr[0]));
    cb->maxlen = maxlen;
}

void cb_free(CIRC_BUF *cb) {
    assert(cb);
    free(cb->arr);
}

void cb_print(CIRC_BUF *cb) {
    assert(cb);
    printf(">> ");
    if (cb->len == 0) {
        printf("\n");
        return;
    }
    if (cb->i > cb->j) {
        int k = cb->j;
        while (k < cb->i) {
            printf("%f ", cb->arr[k]);
            k++;
        }
    } else {
        int k = cb->j;
        while (k < cb->maxlen) {
            printf("%f ", cb->arr[k]);
            k++;
        }
        k = 0;
        while (k < cb->i) {
            printf("%f ", cb->arr[k]);
            k++;
        }
    }

    printf("\n");
}

// TODO Контроль переполнения
bool cb_push(CIRC_BUF *cb, double value) {
    bool full = false;
    assert(cb);
    if (cb->i < cb->maxlen) {
        cb->arr[cb->i++] = value;
        cb->len++;
    } else if (cb->j > 0) {
        cb->i = 0;
        cb->arr[cb->i++] = value;
        cb->len++;
    } else {
        full = true;
    }
    return full;
}

// Контроль опустошения
bool cb_pop(CIRC_BUF *cb, double *value) {
    assert(cb);
    if (cb->len > 0) {
        if (cb->j >= 0) {
            *value = cb->arr[cb->j];
            cb->j++;
            cb->len--;
            return true;
        }
        return false;
    }
    return false;
}
