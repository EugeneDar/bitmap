cmake_minimum_required(VERSION 3.23)
project(bitmap_structure C)

set(CMAKE_C_STANDARD 99)

add_executable(bitmap_structure src/lib/bitmap.h src/lib/bitmap.c src/test/test.h src/test/test.c)
