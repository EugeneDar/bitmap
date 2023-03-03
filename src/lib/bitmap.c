#include "bitmap.h"

void change_size(struct bitmap_t *bm, uint64_t new_size) {
    new_size = ((new_size + 15) >> 4) << 4;
    uint8_t *tmp = realloc(bm->map, new_size);
    if (!tmp) {
        free(bm->map);
        free(bm);
        exit(1);
    }
    bm->map = tmp;

    for (uint64_t i = bm->size; i < new_size; i += 16) {
        asm volatile (
                "xorps %%xmm0, %%xmm0\n"
                "movups %%xmm0, (%0)\n"
                :
                : "rm"(bm->map + i)
                : "xmm0"
                );
    }

    bm->size = new_size;
}

struct bitmap_t *bm_new() {
    struct bitmap_t *bitmap = calloc(1, sizeof(*bitmap));
    bitmap->size = 0;
    bitmap->map = 0;
    return bitmap;
}

void bm_init(struct bitmap_t *ptr) {
    for (uint64_t i = 0; i < ptr->size; ++i) {
        ptr->map[i] = 0;
    }
}

void bm_free(struct bitmap_t *ptr) {
    free(ptr->map);
    free(ptr);
}

bool bm_is_empty(const struct bitmap_t *bm) {
    for (uint64_t i = 0; i < bm->size; i += 16) {
        int result;
        asm volatile (
                "movups (%1), %%xmm0\n"
                "xorps %%xmm1, %%xmm1\n"
                "mov $0, %0\n"
                "comisd %%xmm1, %%xmm0\n"
                "jz end_position_2\n"
                "mov $1, %0\n"
                "end_position_2:\n"
                : "=rm"(result)
                : "rm"(bm->map + i)
                : "xmm0", "xmm1"
                );
        if (result) {
            return false;
        }
    }
    return true;
}

void bm_set_bit(struct bitmap_t *bm, uint64_t idx) {
    if ((idx >> 3) + 1 > bm->size) {
        change_size(bm, (idx >> 3) + 1);
    }

    uint8_t value = ((1 << (idx & 7)));

    bm->map[idx >> 3] |= value;

}

void bm_clear_bit(struct bitmap_t *bm, uint64_t idx) {
    bm_set_bit(bm, idx);
    bm->map[idx >> 3] ^= ((1 << (idx & 7)));
}

// todo use sse
bool bm_eq(const struct bitmap_t *bm1, const struct bitmap_t *bm2) {
    if (bm1->size >= bm2->size) {

        for (uint64_t i = 0; i < bm2->size; i += 16) {
            int result;
            asm volatile (
                    "movups (%1), %%xmm0\n"
                    "movups (%2), %%xmm1\n"
                    "mov $0, %0\n"
                    "comisd %%xmm0, %%xmm1\n"
                    "jz end_position_3\n"
                    "mov $1, %0\n"
                    "end_position_3:\n"
                    : "=rm"(result)
                    : "rm"(bm1->map + i), "rm"(bm2->map + i)
                    : "xmm0", "xmm1"
                    );
            if (result) {
                return false;
            }
        }

        for (uint64_t i = bm2->size; i < bm1->size; i += 1) {
            if (bm1->map[i] != 0) {
                return false;
            }
        }

    } else {

        for (uint64_t i = 0; i < bm1->size; i += 16) {
            int result;
            asm volatile (
                    "movups (%1), %%xmm0\n"
                    "movups (%2), %%xmm1\n"
                    "mov $0, %0\n"
                    "comisd %%xmm0, %%xmm1\n"
                    "jz end_position_5\n"
                    "mov $1, %0\n"
                    "end_position_5:\n"
                    : "=rm"(result)
                    : "rm"(bm1->map + i), "rm"(bm2->map + i)
                    : "xmm0", "xmm1"
                    );
            if (result) {
                return false;
            }
        }

        for (uint64_t i = bm1->size; i < bm2->size; ++i) {
            if (bm2->map[i] != 0) {
                return false;
            }
        }
    }

    return true;
}

void bm_and(struct bitmap_t *bm1, const struct bitmap_t *bm2) {
    uint64_t min_size = bm1->size < bm2->size ? bm1->size : bm2->size;
    for (uint64_t i = 0; i < min_size; i += 16) {
        asm volatile (
                "movups (%0), %%xmm0\n"
                "movups (%1), %%xmm1\n"
                "andps %%xmm1, %%xmm0\n"
                "movups %%xmm0, (%0)\n"
                :
                : "rm"(bm1->map + i), "rm"(bm2->map + i)
                : "xmm0", "xmm1"
                );
    }
    for (uint64_t i = min_size; i < bm1->size; i += 16) {
        asm volatile (
                "xorps %%xmm0, %%xmm0\n"
                "movups %%xmm0, (%0)\n"
                :
                : "rm"(bm1->map + i)
                : "xmm0"
                );
    }
}

void bm_or(struct bitmap_t *bm1, const struct bitmap_t *bm2) {
    if (bm1->size < bm2->size) {
        change_size(bm1, bm2->size);
    }
    for (uint64_t i = 0; i < bm2->size; i += 16) {
        asm volatile (
                "movups (%0), %%xmm0\n"
                "movups (%1), %%xmm1\n"
                "orps %%xmm1, %%xmm0\n"
                "movups %%xmm0, (%0)\n"
                :
                : "rm"(bm1->map + i), "rm"(bm2->map + i)
                : "xmm0", "xmm1"
                );
    }
}

bool is_disjoint(const struct bitmap_t *bm1, const struct bitmap_t *bm2) {
    uint64_t min_size = bm1->size < bm2->size ? bm1->size : bm2->size;
    for (uint64_t i = 0; i < min_size; i += 16) {
        int result;
        asm volatile (
                "movups (%1), %%xmm0\n"
                "movups (%2), %%xmm1\n"
                "andps %%xmm1, %%xmm0\n"
                "xorps %%xmm1, %%xmm1\n"
                "mov $0, %0\n"
                "comisd %%xmm1, %%xmm0\n"
                "jz end_position\n"
                "mov $1, %0\n"
                "end_position:\n"
                : "=rm"(result)
                : "rm"(bm1->map + i), "rm"(bm2->map + i)
                : "xmm0", "xmm1"
                );
        if (result) {
            return false;
        }
    }
    return true;
}
