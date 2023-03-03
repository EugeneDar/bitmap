#include <stdbool.h>
#include <smmintrin.h>
#include <immintrin.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <stdio.h>

struct bitmap_t {
    uint8_t* map;
    uint64_t size;
};

// Allocate and initialize a new bitmap.
struct bitmap_t* bm_new();

// Initialize (also can be used for reset) the given bitmap.
void bm_init(struct bitmap_t* ptr);

// Frees the specified bitmap.
void bm_free(struct bitmap_t* ptr);

// Returns true if all bits are 0.
bool bm_is_empty(const struct bitmap_t* bm);

// Sets the specified bit in bitmap to one.
void bm_set_bit(struct bitmap_t* bm, uint64_t idx);

// Clears the specified bit in bitmap.
void bm_clear_bit(struct bitmap_t* bm, uint64_t idx);

// Returns true if two bitmaps are equal.
bool bm_eq(const struct bitmap_t* bm1, const struct bitmap_t* bm2);

// bm1 = bm1 & bm2
void bm_and(struct bitmap_t* bm1, const struct bitmap_t* bm2);

// bm1 = bm1 | bm2
void bm_or(struct bitmap_t* bm1, const struct bitmap_t* bm2);

// Returns true if the set has no elements in common with other one.
// This is equivalent to checking for an empty intersection.
bool is_disjoint(const struct bitmap_t* bm1, const struct bitmap_t* bm2);
