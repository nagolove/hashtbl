#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define LEN 4
typedef struct CIRC_BUF {
    double arr[LEN];
    int i, j, maxlen, len;
} CIRC_BUF;

void init(CIRC_BUF *cb) {
    memset(cb, 0, sizeof(CIRC_BUF));
    cb->maxlen = LEN;
}

void print(CIRC_BUF *cb) {
    printf(">> ");
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
void push(CIRC_BUF *cb, double value) {
    if (cb->i < cb->maxlen) {
        cb->arr[cb->i++] = value;
    } else if (cb->j > 0) {
        cb->i = 0;
        cb->arr[cb->i++] = value;
    } else {
        print(cb);
        printf("full\n");
        abort();
    }
}

// Контроль опустошения
bool pop(CIRC_BUF *cb, double *value) {
    if (cb->j >= 0) {
        *value = cb->arr[cb->j];
        cb->j++;
        return true;
    }
    return false;
}

int main() {
    CIRC_BUF buf = {0, };
    init(&buf);
    int i = 1;

    push(&buf, i++);
    push(&buf, i++);
    push(&buf, i++);
    push(&buf, i++);

    print(&buf);

    double v = 0.;
    bool res = false;

    res = pop(&buf, &v);
    printf("popped = %d, value = %f\n", (int)res, v);
    print(&buf);

    push(&buf, i++);
    print(&buf);

    res = pop(&buf, &v);
    printf("popped = %d, value = %f\n", (int)res, v);
    print(&buf);

    push(&buf, i++);
    print(&buf);

    res = pop(&buf, &v);
    printf("popped = %d, value = %f\n", (int)res, v);
    print(&buf);

    push(&buf, i++);
    print(&buf);

    res = pop(&buf, &v);
    printf("popped = %d, value = %f\n", (int)res, v);
    print(&buf);

    push(&buf, i++);
    print(&buf);

    return 1;
}
