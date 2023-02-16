#include <assert.h>
#include "../../src/lib/bitmap.h"
#include "test.h"

void some_test() {
    struct bitmap_t* bm1 = bm_new();
    struct bitmap_t* bm2 = bm_new();

    bm_set_bit(bm1, 1); // 0b00010
    bm_set_bit(bm2, 4); // 0b10000

    assert(!bm_eq(bm1, bm2));

    bm_or(bm1, bm2); // bm1 = 0b00010 | 0b10000 = 0b10010
    bm_clear_bit(bm1, 1); // bm1 = 0b10000

    assert(bm_eq(bm1, bm2));
}

void set_bit_test () {
    struct bitmap_t* bm1 = bm_new();
    struct bitmap_t* bm2 = bm_new();

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
}

void is_empty_test() {
    struct bitmap_t* bm1 = bm_new();
    struct bitmap_t* bm2 = bm_new();

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
}



// todo clear_bit_test

void run_all_tests() {
    some_test();
    is_empty_test();
    set_bit_test();
}

int main() {
    run_all_tests();
}