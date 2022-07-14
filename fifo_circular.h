#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LEN 4

typedef struct CIRC_BUF {
    double arr[LEN];
    int i, j, maxlen, len;
} CIRC_BUF;

extern void init(CIRC_BUF *cb);
extern void print(CIRC_BUF *cb);
extern void push(CIRC_BUF *cb, double value);
extern bool pop(CIRC_BUF *cb, double *value);
