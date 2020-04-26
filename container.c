#include "kwcc.h"

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

Vec *vec_create(int cap) {
    Vec *vec = calloc(1, sizeof(Vec));
    vec->fst = calloc(cap, sizeof(void *));
    vec->size = 0;
    vec->cap = cap;
    return vec;
}

void vec_push_back(Vec* vec, void *elem) {
    if (vec->size >= vec->cap) {
        int new_cap = vec->cap * 2;
        void *new_area = calloc(new_cap, sizeof(void *));
        memcpy(new_area, vec->fst, vec->cap * sizeof(void *));
        free(vec->fst);
        vec->fst = new_area;
        vec->cap = new_cap;
    }
    vec->fst[vec->size] = elem;
    vec->size++;
}

void *vec_at(Vec* vec, int index) {
    if (index < 0 || vec->size < index) {
        return NULL;
    }
    return vec->fst[index];
}

int vec_size(Vec* vec) {
    return vec->size;
}

void vec_release(Vec* vec) {
    free(vec->fst);
    free(vec);
}

int vec_test() {
    char *str = "Hello, World!";

    Vec *vec = vec_create(4);
    if (vec_size(vec) != 0) error("初期サイズが不正");
    if (vec_at(vec, 0) != NULL) error("不正な要素取り出し");

    vec_push_back(vec, str);
    if (vec_size(vec) != 1) error("サイズが変更されていない");
    if (vec_at(vec, 0) != str) error("要素が入ってない");

    vec_push_back(vec, str + 1);
    vec_push_back(vec, str + 2);
    vec_push_back(vec, str + 3);
    if (vec_size(vec) != 4) error("サイズが不正");
    if (vec_at(vec, 2) != str + 2) error("ランダムアクセスした要素が不正");

    vec_push_back(vec, str + 4);
    if (vec_size(vec) != 5) error("追加確保後のサイズが不正");
    if (vec_at(vec, 4) != str + 4) error("追加確保した要素が不正");

    vec_release(vec);

    printf("vec test successfully\n");
    return 0;
}
