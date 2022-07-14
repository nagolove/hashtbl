#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct CIRC_BUF {
    double* arr;
    int i, j, maxlen, len;
} CIRC_BUF;

extern void cb_init(CIRC_BUF *cb, int maxlen);
extern void cb_free(CIRC_BUF *cb);
extern void cb_print(CIRC_BUF *cb);
// Возвращает истину если значение было добавлено
extern bool cb_push(CIRC_BUF *cb, double value);
// Возвращает истину если удалось снять значение
extern bool cb_pop(CIRC_BUF *cb, double *value);
