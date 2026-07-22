#ifndef VEC_H
#define VEC_H

#include <stddef.h>

#define VEC_NPOS ((size_t)-1)

typedef struct {
    void *data;
    size_t length;
    size_t capacity;
    size_t elem_size;
} Vec;

int vec_init(Vec *vec, size_t elem_size);
void vec_free(Vec *vec);
void vec_clear(Vec *vec);

int vec_reserve(Vec *vec, size_t minimum_capacity);
int vec_shrink_to_fit(Vec *vec);

int vec_assign(Vec *vec, const void *data, size_t count);
int vec_clone(Vec *dest, const Vec *src);

void *vec_at(Vec *vec, size_t index);
const void *vec_at_const(const Vec *vec, size_t index);
int vec_get(const Vec *vec, size_t index, void *out);
int vec_set(Vec *vec, size_t index, const void *element);

int vec_push_back(Vec *vec, const void *element);
int vec_pop_back(Vec *vec, void *out);

int vec_insert(Vec *vec, size_t position, const void *element);
int vec_erase(Vec *vec, size_t position, size_t count);

size_t vec_find(const Vec *vec, const void *element, int (*compare)(const void *a, const void *b));
void vec_sort(Vec *vec, int (*compare)(const void *a, const void *b));

#endif
