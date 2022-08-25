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
    if (cb->arr) free(cb->arr);
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

void cb_print_circ(CIRC_BUF *cb) {
    /*
    printf("i = %d\n", cb->i);
    printf("j = %d\n", cb->j);

    for(int i = 0; i < cb->maxlen; i++) {
        printf("%f\n", cb->arr[i]);
    }
    printf("----------------------------\n");
    */

    for(int k = cb->i; k < cb->maxlen; k++) {
        printf("%f\n", cb->arr[k]);
    }
    for(int k = cb->j; k < cb->i; k++) {
        printf("%f\n", cb->arr[k]);
    }

}

void cb_push_circ(CIRC_BUF *cb, double value) {
    assert(cb);

    cb->arr[cb->i] = value;
    cb->i = (cb->i + 1) % cb->maxlen;
    cb->len++;
    if (cb->len == cb->maxlen) {
        cb->len = cb->maxlen - 1;
    }

}


bool cb_push(CIRC_BUF *cb, double value) {
    bool notfull = true;
    assert(cb);

    if (cb->len < cb->maxlen) {
        cb->arr[cb->i] = value;
        cb->i = (cb->i + 1) % cb->maxlen;
        cb->len++;
    } else 
        notfull = false;

    return notfull;
}

// Контроль опустошения
bool cb_pop(CIRC_BUF *cb, double *value) {
    assert(cb);
    assert(value);

    bool popped = false;

    if (cb->len > 0) {
        *value = cb->arr[cb->j];
        cb->j = (cb->j + 1) % cb->maxlen;
        cb->len--;
        popped = true;
    }

    return popped;
}
