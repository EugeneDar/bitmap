#include "bitmap.h"

#include <stdlib.h>

void change_size(struct bitmap_t *bm, unsigned new_size) {
    unsigned char *tmp = realloc(bm->map, new_size);
    if (!tmp) {
        free(bm->map);
        free(bm);
        exit(1);
    }
    bm->map = tmp;
    bm->size = new_size;
}

struct bitmap_t *bm_new() {
    struct bitmap_t *bitmap = calloc(1, sizeof(*bitmap));
    bitmap->size = 0;
    bitmap->map = 0;
    return bitmap;
}

void bm_init(struct bitmap_t *ptr) {
    for (unsigned int i = 0; i < ptr->size; ++i) {
        ptr->map[i] = 0;
    }
}

void bm_free(struct bitmap_t *ptr) {
    free(ptr->map);
    free(ptr);
}

// todo use sse
bool bm_is_empty(const struct bitmap_t *bm) {
    for (unsigned int i = 0; i < bm->size; ++i) {
        if (bm->map[i] != 0) {
            return false;
        }
    }
    return true;
}

// todo use sse
void bm_set_bit(struct bitmap_t *bm, unsigned int idx) {
    if ((idx >> 3) + 1 > bm->size) {
        change_size(bm, (idx >> 3) + 1);
    }
    bm->map[idx >> 3] |= ((1 << (idx & 7)));
}

// todo use sse
void bm_clear_bit(struct bitmap_t *bm, unsigned int idx) {
    bm_set_bit(bm, idx);
    bm->map[idx >> 3] ^= ((1 << (idx & 7)));
}

// todo use sse
bool bm_eq(const struct bitmap_t *bm1, const struct bitmap_t *bm2) {
    if (bm1->size >= bm2->size) {

        for (unsigned int i = 0; i < bm2->size; ++i) {
            if (bm1->map[i] != bm2->map[i]) {
                return false;
            }
        }

        for (unsigned int i = bm2->size; i < bm1->size; ++i) {
            if (bm1->map[i] != 0) {
                return false;
            }
        }

    } else {

        for (unsigned int i = 0; i < bm1->size; ++i) {
            if (bm1->map[i] != bm2->map[i]) {
                return false;
            }
        }

        for (unsigned int i = bm1->size; i < bm2->size; ++i) {
            if (bm2->map[i] != 0) {
                return false;
            }
        }
    }

    return true;
}

// todo use sse
void bm_and(struct bitmap_t* bm1, const struct bitmap_t* bm2) {
    unsigned int min_size = bm1->size < bm2->size ? bm1->size : bm2->size;
    for (unsigned int i = 0; i < min_size; ++i) {
        bm1->map[i] = bm1->map[i] & bm2->map[i];
    }
    for (unsigned int i = min_size; i < bm1->size; ++i) {
        bm1->map[i] = 0;
    }
}

// todo use sse
void bm_or(struct bitmap_t* bm1, const struct bitmap_t* bm2) {
    if (bm1->size < bm2->size) {
        change_size(bm1, bm2->size);
    }
    for (unsigned int i = 0; i < bm2->size;++i) {
        bm1->map[i] = bm1->map[i] | bm2->map[i];
    }
}

// todo use sse
bool is_disjoint(const struct bitmap_t* bm1, const struct bitmap_t* bm2) {
    unsigned int min_size = bm1->size < bm2->size ? bm1->size : bm2->size;
    for (unsigned int i = 0; i < min_size; ++i) {
        if ((bm1->map[i] & bm2->map[i]) != 0) {
            return false;
        }
    }
    return true;
}
