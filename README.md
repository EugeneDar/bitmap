## bitmap

### Quick start

```
    struct bitmap_t* bm1 = bm_new();
    struct bitmap_t* bm2 = bm_new();

    bm_set_bit(bm1, 1); // 0b00010
    bm_set_bit(bm2, 4); // 0b10000

    assert(!bm_eq(bm1, bm2));

    bm_or(bm1, bm2); // bm1 = 0b00010 | 0b10000 = 0b10010
    bm_clear_bit(bm1, 1); // bm1 = 0b10000

    assert(bm_eq(bm1, bm2));
```

### Run tests

    $ bash test.sh 
