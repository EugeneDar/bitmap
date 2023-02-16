#include "test.h"

#include <stdio.h>
#include <assert.h>

#include "../../src/lib/bitmap.h"

void some_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    bm_set_bit(bm1, 1);  // 0b00010
    bm_set_bit(bm2, 4);  // 0b10000

    assert(!bm_eq(bm1, bm2));

    bm_or(bm1, bm2);       // bm1 = 0b00010 | 0b10000 = 0b10010
    bm_clear_bit(bm1, 1);  // bm1 = 0b10000

    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void init_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    bm_set_bit(bm1, 1);
    bm_set_bit(bm1, 3);
    
    assert(!bm_eq(bm1, bm2));

    bm_init(bm1);
    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void set_bit_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    assert(bm_eq(bm1, bm2));

    bm_set_bit(bm1, 1);
    assert(!bm_eq(bm1, bm2));

    bm_set_bit(bm2, 4);
    assert(!bm_eq(bm1, bm2));

    bm_set_bit(bm2, 4);
    assert(!bm_eq(bm1, bm2));

    bm_set_bit(bm1, 4);
    assert(!bm_eq(bm1, bm2));

    bm_set_bit(bm2, 1);
    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void is_empty_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    assert(bm_is_empty(bm1));
    assert(bm_is_empty(bm2));

    bm_set_bit(bm1, 1);
    bm_set_bit(bm2, 4);

    assert(!bm_is_empty(bm1));
    assert(!bm_is_empty(bm2));

    bm_clear_bit(bm1, 1);
    bm_clear_bit(bm2, 4);

    assert(bm_is_empty(bm1));
    assert(bm_is_empty(bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void clear_bit_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    assert(bm_eq(bm1, bm2));

    bm_clear_bit(bm1, 1000);
    assert(bm_eq(bm1, bm2));

    bm_set_bit(bm1, 1000);
    assert(!bm_eq(bm1, bm2));

    bm_clear_bit(bm1, 1000);
    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void or_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    bm_set_bit(bm1, 1);
    bm_set_bit(bm2, 3);
    bm_or(bm1, bm2);
    assert(!bm_eq(bm1, bm2));

    bm_or(bm2, bm1);
    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void and_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    bm_set_bit(bm1, 1);
    bm_set_bit(bm1, 3);
    bm_set_bit(bm2, 2);

    bm_and(bm1, bm2);
    assert(!bm_eq(bm1, bm2));

    bm_clear_bit(bm2, 2);
    assert(bm_eq(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void is_disjoint_test() {
    struct bitmap_t *bm1 = bm_new();
    struct bitmap_t *bm2 = bm_new();

    bm_set_bit(bm1, 1);
    bm_set_bit(bm1, 3);
    bm_set_bit(bm2, 2);

    assert(is_disjoint(bm1, bm2));

    bm_set_bit(bm2, 3);

    assert(!is_disjoint(bm1, bm2));

    bm_free(bm1);
    bm_free(bm2);
}

void run_all_tests() {
    some_test();
    init_test();
    is_empty_test();
    set_bit_test();
    clear_bit_test();
    or_test();
    and_test();
    is_disjoint_test();
}

int main() {
    run_all_tests();
    printf("Completed\n");
}